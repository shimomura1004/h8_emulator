#include "kozos.h"
#include "ether.h"
#include "etherdrv.h"

#include "lib.h"

uint32 network_id;

#define MACADDR "\x00\x11\x22\x33\x44\x55"
#define IPADDR  0xc0a80010 /* 192.168.0.16 */
static uint32 ipaddr = IPADDR;

#define MACADDR_SIZE 6
#define IPADDR_SIZE  4

// 宛先 MAC アドレス 6byte
// 送信元 MAC アドレス 6byte
// タイプ 2byte
#define ETHERNET_HEADER_SIZE 14

struct ethernet_header {
  uint8 dst_addr[MACADDR_SIZE];
  uint8 src_addr[MACADDR_SIZE];
  uint16 type;
#define ETHERNET_TYPE_IP  0x0800
#define ETHERNET_TYPE_ARP 0x0806
};

struct arp_header {
  // ネットワークプロトコルの種類: Ethernet の場合は 1
  uint16 hardware;
#define ARP_HARDWARE_ETHER 1
  // プロトコルタイプ: IPv4 の場合は 0x0800 以降の値
  uint16 protocol;
  // ハードウェアアドレス(MAC アドレス)のサイズ: Ethernet の場合は 6
  uint8  hardware_addr_size;
  // protocol のアドレスのサイズ: IPv4 の場合は 4
  uint8  protocol_addr_size;
  // 送信者が実行している動作: 1 が要求、2 が返信
  uint16 operation;
#define ARP_OPERATION_REQUEST 1
#define ARP_OPERATION_REPLY   2

  // 送信元のハードウェアアドレス: Ethernet の場合は送信元の MAC アドレス
  uint8 sender_hardware_addr[MACADDR_SIZE];
  // 送信元の protocol のアドレス: IPv4 の場合は送信元の IPv4 アドレス
  uint8 sender_protocol_addr[IPADDR_SIZE];
  // 宛先のハードウェアアドレス
  uint8 target_hardware_addr[MACADDR_SIZE];
  // 宛先の protocol のアドレス
  uint8 target_protocol_addr[IPADDR_SIZE];
};

struct ip_header {
  // バージョン(4bit)とヘッダ長(4bit): ヘッダ長は4オクテット(32ビット)単位
  uint8 v_hl;
  // サービスの種別: パケット転送時に重視するサービスを指定
  // 優先度や信頼性など
  uint8 tos;
  // ヘッダを含むパケット全体の長さ: 4オクテット単位
  uint16 total_length;
  // 識別子: パケット送信元が一意な値を指定し、パケットが経路上で分割されたときにこれを手がかりに戻す
  uint16 id;
  // フラグメント: 上位3ビットは制御用のフラグ、id と fragment を読めば断片化したパケットを復元できる
  uint16 fragment;
  // TTL: 経由できるルータの上限
  uint8  ttl;
  // TCP や UDP など
  uint8  protocol;
#define IP_PROTOCOL_ICMP  1
#define IP_PROTOCOL_TCP   6
#define IP_PROTOCOL_UDP  17
  // ヘッダ部分のみが対象のチェックサム
  uint16 checksum;
  // 送信元アドレス
  uint8 src_addr[IPADDR_SIZE];
  // 宛先アドレス
  uint8 dst_addr[IPADDR_SIZE];
};

struct icmp_header {
  // タイプ: echo reply などの他、unreachable の通知などのタイプがある
  uint8 type;
#define ICMP_TYPE_REPLY   0
#define ICMP_TYPE_REQUEST 8
  // TYPE が 0/8 のときは常に 0
  uint8 code;
  uint16 checksum;
};

// ICMP 応答用のチェックサムを計算
static uint16 calc_checksum(int size, void *buf)
{
  int i;
  uint16 val;
  uint32 sum = 0;
  uint16 *p = buf;

  for (i = 0; i < size; i += sizeof(uint16)) {
    if (size - i == 1)
      val = *(unsigned char *)p << 8;
    else
      val = *p;
    sum += val;
    p++;
  }

  while (sum > 0xffff)
    sum = ((sum >> 16) & 0xffff) + (sum & 0xffff);

  return ~sum & 0xffff;
}

// ICMP パケットを処理
static int icmp_proc(int size, struct icmp_header *hdr)
{
  int ret = 0;

  switch (hdr->type) {
  case ICMP_TYPE_REQUEST:
    hdr->type = ICMP_TYPE_REPLY;
    hdr->checksum = 0;
    hdr->checksum = calc_checksum(size, hdr);
    ret = size;
    break;
  default:
    break;
  }

  return ret;
}

// 通常の IP パケットなら
static int ip_proc(int size, struct ip_header *hdr)
{
  int ret = 0, hdrlen, nextsize;
  void *nexthdr;

  // IPv4 でなければ無視
  if (((hdr->v_hl >> 4) & 0xf) != 4)
    return 0;
  // 宛先アドレスが自分でなければ無視
  if (memcmp(hdr->dst_addr, &ipaddr, IPADDR_SIZE))
    return 0;

  // 4倍？
  hdrlen = (hdr->v_hl & 0xf) << 2;

  // total_length はオクテット単位だけど、いいのか？
  if (size > hdr->total_length)
    size = hdr->total_length;

  // IP ヘッダ分を削る
  nextsize = size - hdrlen;
  nexthdr = (char *)hdr + hdrlen;

  switch (hdr->protocol) {
  case IP_PROTOCOL_ICMP:
    // ping に対して応答する(ICMP は IP 層のパケット)
    ret = icmp_proc(nextsize, nexthdr);
    break;
  default:
    break;
  }

  if (ret > 0) {
    // 返送用パケットを準備
    memcpy(hdr->dst_addr, hdr->src_addr, IPADDR_SIZE);
    memcpy(hdr->src_addr, &ipaddr, IPADDR_SIZE);
    hdr->checksum = 0;
    hdr->checksum = calc_checksum(hdrlen, hdr);
    ret += hdrlen;
  }

  return ret;
}

// arp 要求を受信したら、arp 応答を返す
static int arp_proc(int size, struct arp_header *hdr)
{
  int ret = 0;

  // Ethernet じゃないフレームなら無視
  if (hdr->hardware != ARP_HARDWARE_ETHER)
    return 0;
  // IP じゃないパケットなら無視
  if (hdr->protocol != ETHERNET_TYPE_IP)
    return 0;

  switch (hdr->operation) {
  case ARP_OPERATION_REQUEST:
    // arp 要求だったら、自分の mac アドレスを応答しないといけない

    // 問い合わせされている IP アドレスが自分と違ったら無視する
    if (memcmp(hdr->target_protocol_addr, &ipaddr, IPADDR_SIZE))
      break;

    // 受信したパケットを流用し、自分の MAC/IP アドレスを詰めて応答する
    memcpy(hdr->target_hardware_addr, hdr->sender_hardware_addr, MACADDR_SIZE);
    memcpy(hdr->target_protocol_addr, hdr->sender_protocol_addr, IPADDR_SIZE);
    memcpy(hdr->sender_hardware_addr, MACADDR, MACADDR_SIZE);
    memcpy(hdr->sender_protocol_addr, &ipaddr, IPADDR_SIZE);
    hdr->operation = ARP_OPERATION_REPLY;
    ret = size;
  default:
    break;
  }

  return ret;
}

// 受信した Ethernet フレームを処理する
// 応答が必要な場合は戻り値として 0 以上のデータの大きさを返す
static int ethernet_proc(int size, struct ethernet_header *hdr)
{
  int ret = 0, nextsize;
  void *nexthdr;

  // 自分宛のパケットでなかったら破棄
  if (!(hdr->dst_addr[0] & 1) && memcmp(hdr->dst_addr, MACADDR, MACADDR_SIZE))
    return 0;

  // Ethernet ヘッダを取り除く(Ethernet ヘッダは IP ヘッダの前にある)
  nextsize = size - ETHERNET_HEADER_SIZE;
  nexthdr = (char *)hdr + ETHERNET_HEADER_SIZE;

  // Ethernet フレームの種類に応じて処理を実行
  switch (hdr->type) {
  case ETHERNET_TYPE_ARP:
    // arp の場合は応答する
    ret = arp_proc(nextsize, nexthdr);
    break;
  case ETHERNET_TYPE_IP:
    // IP パケットであれば IP レイヤで処理する
    ret = ip_proc(nextsize, nexthdr);
    break;
  default:
    break;
  }

  if (ret > 0) {
    // DST に元のパケットの SRC を設定し、送信元に送り返すようにする
    memcpy(hdr->dst_addr, hdr->src_addr, MACADDR_SIZE);
    memcpy(hdr->src_addr, MACADDR, MACADDR_SIZE);
    // Ethernet ヘッダ分のサイズを追加し、Ethernet フレームを返すようにする
    ret += ETHERNET_HEADER_SIZE;
  }

  return ret;
}

// Ethernet を初期化
// ドライバ初期化を指示するメッセージを etherdrv に送る
static void send_use(void)
{
  char *p;
  p = kz_kmalloc(1);
  p[0] = ETHERDRV_CMD_USE;
  kz_send(MSGBOX_ID_ETHOUTPUT, 1, p);
}

// データを送信
// データ送信を指示するメッセージを etherdrv に送る
static void send_send(int size, char *buf)
{
  char *p;
  p = kz_kmalloc(size + 1);
  p[0] = ETHERDRV_CMD_SEND;
  memcpy(&p[1], buf, size);
  kz_send(MSGBOX_ID_ETHOUTPUT, size + 1, p);
}

// IP 通信を処理するためのユーザタスク
// 割り込みが etherdrv で処理され、メッセージとして送信されてきたあとの処理
int ip_main(int argc, char *argv[])
{
  char *p;
  int size;

  send_use();
  puts("network ready.\n");

  while (1) {
    // Ethernet からデータが受信されるのを待つ
    kz_recv(MSGBOX_ID_ETHINPUT, &size, &p);

    // Ethernet フレームを処理する(ARP or IP)
    // Ethernet フレームを送り返す必要がある場合は 0 より大きい size が返され
    // p の中身が書き換えられている
    // 今回は ICMP echo reply のみに対応するため、これで十分
    size = ethernet_proc(size, (struct ethernet_header *)p);

    puts("received: 0x");
    putxval(size, 0);
    puts("bytes\n");
    if (size > 0) {
      send_send(size, p);
      puts("replyed.\n");
    } else {
      puts("no reply.\n");
    }
    kz_kmfree(p);
  }

  return 0;
}
