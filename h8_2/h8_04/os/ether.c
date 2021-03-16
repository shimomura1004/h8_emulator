#include "defines.h"
#include "lib.h"
#include "ether.h"

// IO ポートのアドレス
#define H8_3069F_P1DDR  ((volatile uint8 *)0xfee000)
#define H8_3069F_P3DDR  ((volatile uint8 *)0xfee002)
#define H8_3069F_P6DDR  ((volatile uint8 *)0xfee005)
#define H8_3069F_P8DDR  ((volatile uint8 *)0xfee007)
#define H8_3069F_P9DDR  ((volatile uint8 *)0xfee008)

#define H8_3069F_SYSCR  ((volatile uint8 *)0xfee012)
#define H8_3069F_ISCR   ((volatile uint8 *)0xfee014)
#define H8_3069F_IER    ((volatile uint8 *)0xfee015)
#define H8_3069F_ISR    ((volatile uint8 *)0xfee016)
#define H8_3069F_IPRA   ((volatile uint8 *)0xfee018)
#define H8_3069F_IPRB   ((volatile uint8 *)0xfee019)

// H8/3069F では RTL は 0x20000 に配置されている
#define RTL8019_ADDR 0x200000

#define NE2000_CR      ((volatile uint8 *)(RTL8019_ADDR + 0x00))
#define RTL8019_RDMAP  ((volatile uint8 *)(RTL8019_ADDR + 0x10))  // Remote DMA port
#define RTL8019_RP     ((volatile uint8 *)(RTL8019_ADDR + 0x18))

// 同じアドレスでも、ページ設定ごとに機能が変わるので注意
/* Page0 */
#define NE2000_PSTART  ((volatile uint8 *)(RTL8019_ADDR + 0x01))
#define NE2000_PSTOP   ((volatile uint8 *)(RTL8019_ADDR + 0x02))
#define NE2000_BNRY    ((volatile uint8 *)(RTL8019_ADDR + 0x03))
#define NE2000_TPSR    ((volatile uint8 *)(RTL8019_ADDR + 0x04))
#define NE2000_TBCR0   ((volatile uint8 *)(RTL8019_ADDR + 0x05))
#define NE2000_TBCR1   ((volatile uint8 *)(RTL8019_ADDR + 0x06))
#define NE2000_ISR     ((volatile uint8 *)(RTL8019_ADDR + 0x07))
#define NE2000_RSAR0   ((volatile uint8 *)(RTL8019_ADDR + 0x08))
#define NE2000_RSAR1   ((volatile uint8 *)(RTL8019_ADDR + 0x09))
#define NE2000_RBCR0   ((volatile uint8 *)(RTL8019_ADDR + 0x0a))
#define NE2000_RBCR1   ((volatile uint8 *)(RTL8019_ADDR + 0x0b))
#define NE2000_RCR     ((volatile uint8 *)(RTL8019_ADDR + 0x0c))
#define NE2000_TCR     ((volatile uint8 *)(RTL8019_ADDR + 0x0d))
#define NE2000_DCR     ((volatile uint8 *)(RTL8019_ADDR + 0x0e))
#define NE2000_IMR     ((volatile uint8 *)(RTL8019_ADDR + 0x0f))

/* Page1 */
#define NE2000_PAR0    ((volatile uint8 *)(RTL8019_ADDR + 0x01))
#define NE2000_PAR1    ((volatile uint8 *)(RTL8019_ADDR + 0x02))
#define NE2000_PAR2    ((volatile uint8 *)(RTL8019_ADDR + 0x03))
#define NE2000_PAR3    ((volatile uint8 *)(RTL8019_ADDR + 0x04))
#define NE2000_PAR4    ((volatile uint8 *)(RTL8019_ADDR + 0x05))
#define NE2000_PAR5    ((volatile uint8 *)(RTL8019_ADDR + 0x06))
#define NE2000_CURR    ((volatile uint8 *)(RTL8019_ADDR + 0x07))
#define NE2000_MAR0    ((volatile uint8 *)(RTL8019_ADDR + 0x08))
#define NE2000_MAR1    ((volatile uint8 *)(RTL8019_ADDR + 0x09))
#define NE2000_MAR2    ((volatile uint8 *)(RTL8019_ADDR + 0x0a))
#define NE2000_MAR3    ((volatile uint8 *)(RTL8019_ADDR + 0x0b))
#define NE2000_MAR4    ((volatile uint8 *)(RTL8019_ADDR + 0x0c))
#define NE2000_MAR5    ((volatile uint8 *)(RTL8019_ADDR + 0x0d))
#define NE2000_MAR6    ((volatile uint8 *)(RTL8019_ADDR + 0x0e))
#define NE2000_MAR7    ((volatile uint8 *)(RTL8019_ADDR + 0x0f))

/* Page3 (for RTL8019) */
#define RTL8019_9346CR  ((volatile uint8 *)(RTL8019_ADDR + 0x01))
#define RTL8019_BPAGE   ((volatile uint8 *)(RTL8019_ADDR + 0x02))
#define RTL8019_CONFIG0 ((volatile uint8 *)(RTL8019_ADDR + 0x03))
#define RTL8019_CONFIG1 ((volatile uint8 *)(RTL8019_ADDR + 0x04))
#define RTL8019_CONFIG2 ((volatile uint8 *)(RTL8019_ADDR + 0x05))
#define RTL8019_CONFIG3 ((volatile uint8 *)(RTL8019_ADDR + 0x06))
#define RTL8019_TEST    ((volatile uint8 *)(RTL8019_ADDR + 0x07))
#define RTL8019_CSNSAV  ((volatile uint8 *)(RTL8019_ADDR + 0x08))
#define RTL8019_HLTCLK  ((volatile uint8 *)(RTL8019_ADDR + 0x09))
#define RTL8019_INTR    ((volatile uint8 *)(RTL8019_ADDR + 0x0b))
#define RTL8019_CONFIG4 ((volatile uint8 *)(RTL8019_ADDR + 0x0d))

#define NE2000_CR_P0       (0 << 6)
#define NE2000_CR_P1       (1 << 6)
#define NE2000_CR_P2       (2 << 6)
#define NE2000_CR_P3       (3 << 6)
#define NE2000_CR_RD_ABORT (4 << 3)
#define NE2000_CR_RD_WRITE (2 << 3)
#define NE2000_CR_RD_READ  (1 << 3)
#define NE2000_CR_TXP      (1 << 2)
#define NE2000_CR_STA      (1 << 1)
#define NE2000_CR_STP      (1 << 0)

#define NE2000_ISR_RDC     (1 << 6)

#define NE2000_RCR_MON     (1 << 5)
#define NE2000_RCR_PRO     (1 << 4)
#define NE2000_RCR_AM      (1 << 3)
#define NE2000_RCR_AB      (1 << 2)

#define NE2000_TCR_ELB     (2 << 1)
#define NE2000_TCR_ILB     (1 << 1)
#define NE2000_TCR_NORMAL  (0 << 1)

#define NE2000_DCR_F1      (1 << 6)
#define NE2000_DCR_F0      (1 << 5)
#define NE2000_DCR_LS      (1 << 3)
#define NE2000_DCR_BOS     (1 << 1)

// RTL8019AS は、いくつかのページを持っているが、その詳細が不明
// 1ページは256Byteらしい。送信のリングバッファには6ページ分(256*6 = 1,536Byte)しかない
// Ethernet フレームは最大で1500バイトなので、1パケット分？
// 受信のリングバッファのページ数は 0x80-0x46=58 なので、14,848Byte
// TP = Transmit Page
#define NE2000_TP_START 0x40
#define NE2000_RP_START 0x46
#define NE2000_RP_STOP  0x80

static void udelay(int usec)
{
  volatile int i;
  for (i = 0; i < 20; i++) /* 20MHz */
    ;
}

static void mdelay(int msec)
{
  volatile int i;
  for (i = 0; i < msec; i++) {
    udelay(1000);
  }
}

// Ethernet フレームを Ethernet Phy から読み込む
static int read_data(int addr, int size, char *buf)
{
  int i;

  // CR(command register) PS1, PS0, RD2, RD1, RD0, TXP, STA, STP
  // PS: P0 => ページ0を選択
  // RD: ABORT => Abort/Complete remote DMA
  // STA=1, STP=0 => Start command (起動時はストップ)
  *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;

  // RBCR(Remote Byte Count Registers)、書き込み用
  // レジスタに何バイトのデータが溜まっているか？を記録
  // 送信するデータのサイズを2バイトで表現(RBCR1 が上位バイト)
  *NE2000_RBCR0 = size & 0xff;
  *NE2000_RBCR1 = (size >> 8) & 0xff;

  // RSAR(Remote Start Address Registers)、書き込み用
  // リモート DMA の開始アドレスを設定する(RSAR1 が上位バイト)
  *NE2000_RSAR0 = addr & 0xff;
  *NE2000_RSAR1 = (addr >> 8) & 0xff;

  // command register を操作し、remote read を行う
  *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_READ | NE2000_CR_STA;
  for (i = 0; i < size; i++) {
    // RDMAP = remote DMA port で、RTL8019 に接続されている
    // ウェイトも入れずにどんどん読んでいいのか？
    buf[i] = *RTL8019_RDMAP;
  }
  // ISR の RDC ビットは、Remote DMA operation が完了するとセットされる
  while ((*NE2000_ISR & NE2000_ISR_RDC) == 0)
    ;

  return 0;
}

// Ethernet フレームを Ethernet Phy に書き込む
static int write_data(int addr, int size, char *buf)
{
  int i;
  
  // ページ0を選択しコマンドを開始する
  *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;

  // データサイズを指定
  *NE2000_RBCR0 = size & 0xff;
  *NE2000_RBCR1 = (size >> 8) & 0xff;

  // Remote DMA の開始アドレスを指定
  *NE2000_RSAR0 = addr & 0xff;
  *NE2000_RSAR1 = (addr >> 8) & 0xff;

  // command resiger を操作し、remote write を行う
  *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_WRITE | NE2000_CR_STA;
  for (i = 0; i < size; i++) {
    // ウェイトも入れずにどんどん書いていいのか？
    *RTL8019_RDMAP = buf[i];
  }
  // リモートコマンドの完了を待つ
  while ((*NE2000_ISR & NE2000_ISR_RDC) == 0)
    ;
}

static int port_init()
{
#if 0 /* DRAMで設定しているので不要 */
  *H8_3069F_P1DDR = 0x1f; /* A0 - A4 */
  *H8_3069F_P3DDR = ...; /* モード５では D8-D15 の設定は不要 */
  *H8_3069F_P6DDR = ...; /* モード５では HWR,RD の設定は不要 */
#endif

#if 0
  *H8_3069F_P8DDR = 0xe8; /* CS1 */
#else
  *H8_3069F_P8DDR = 0xec; /* CS1(ether) and CS2(DRAM) */
#endif

#if 0
  *H8_3069F_P9DDR = ...; /* IERの設定によるので設定不要 */
#endif

  *H8_3069F_ISCR = 0x00; /* lowレベルで割り込み */
  *H8_3069F_IPRA = 0x00;
  *H8_3069F_IER  = 0x20; /* IRQ5割り込み有効化 */

  return 0;
}

int ether_init(int index)
{
  unsigned char t1;
  unsigned char t2[12];
  unsigned char macaddr[6];
  int i;

  // 外部 IO を使えるようにするための設定など
  port_init();

  // どんな意味が？ 0x18 は、Reset port とあるが…
  t1 = *RTL8019_RP;
  *RTL8019_RP = t1;

  mdelay(10);

  // リモートコマンドを停止(STP)
  *NE2000_CR     = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STP;

  // DCR(data configuration register)、ページ0なので write できる
  // F1: FIFO threshold
  // LS: loopback select, セットすると normal operation となる
  // BOS: byte order select, not impelemented とあるので意味がない？
  *NE2000_DCR    = NE2000_DCR_F1 | NE2000_DCR_LS | NE2000_DCR_BOS;

  // データサイズは 0 に
  *NE2000_RBCR0  = 0x00;
  *NE2000_RBCR1  = 0x00;

  // RCR(receive configuration register)
  // monitor mode、受信したパケットのチェックはするが、バッファしない？
  *NE2000_RCR    = NE2000_RCR_MON;

  // TCR(transmit configuration register)
  // ILB = internal loopback
  *NE2000_TCR    = NE2000_TCR_ILB;

  // TPSR(transmit page start register)
  // 送信するパケットのスタートページのアドレスを指定する？
  // http://contiki.sourceforge.net/docs/2.6/a00626_source.html
  // buffer boundaries - transmit has 6 256-byte pages
  //   receive has 26 256-byte pages
  // NE2000_TP_START は 0x40 なので、64番目のページを使うという宣言？
  *NE2000_TPSR   = NE2000_TP_START;

  // PSTART(page start resgister)
  // 受信用のリングバッファの開始アドレス
  *NE2000_PSTART = NE2000_RP_START;

  // BNRY(boundary register)
  // リングバッファ領域の末尾のアドレスを入れる(オーバーランを防ぐために使う)
  *NE2000_BNRY   = NE2000_RP_START;

  // PSTOP(page stop register)
  // 受信リングバッファが置かれたページアドレスを指定する
  *NE2000_PSTOP  = NE2000_RP_STOP;

  // IMR(Interrupt mask register)
  // ISR の割込みを有効にするかどうかを決める
  *NE2000_IMR    = 0x00;

  // ISR(Interrupt status register)
  // NIC の状態と対応する
  *NE2000_ISR    = 0xff;

  // Phy から 12 バイト読み込む(最初に MAC アドレスが入っている？)
  read_data(0, 12, t2);
  puts("MAC: ");
  for (i = 0; i < 6; i++) {
    macaddr[i] = t2[i * 2];
    putxval((unsigned char)macaddr[i], 2);
  }
  puts("\n");

  *NE2000_CR   = NE2000_CR_P1 | NE2000_CR_RD_ABORT | NE2000_CR_STP;

  // PAR0-5 に MAC アドレスを書き込む
  *NE2000_PAR0 = macaddr[0];
  *NE2000_PAR1 = macaddr[1];
  *NE2000_PAR2 = macaddr[2];
  *NE2000_PAR3 = macaddr[3];
  *NE2000_PAR4 = macaddr[4];
  *NE2000_PAR5 = macaddr[5];

  *NE2000_CURR = NE2000_RP_START + 1;
  *NE2000_MAR0 = 0x00;
  *NE2000_MAR1 = 0x00;
  *NE2000_MAR2 = 0x00;
  *NE2000_MAR3 = 0x00;
  *NE2000_MAR4 = 0x00;
  *NE2000_MAR5 = 0x00;
  *NE2000_MAR6 = 0x00;
  *NE2000_MAR7 = 0x00;

#if 0
  *NE2000_CR   = NE2000_CR_P3 | NE2000_CR_RD_ABORT | NE2000_CR_STP;
  *RTL8019_9346CR  = 0xc0;
  *RTL8019_CONFIG1 = 0x80 | 0x40;
  *RTL8019_CONFIG2 = 0x00;
  *RTL8019_9346CR  = 0x00;
#endif

  *NE2000_CR   = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STP;
  *NE2000_RCR  = NE2000_RCR_AM | NE2000_RCR_AB | NE2000_RCR_PRO;
  *NE2000_CR   = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  *NE2000_TCR  = NE2000_TCR_NORMAL;
  *NE2000_IMR  = 0x00;

  return 0;
}

// RTL からの割込みを有効にする
void ether_intr_enable(int index)
{
  *NE2000_CR  = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  *NE2000_IMR = 0x01;
}

// RTL からの割込みを無効にする
void ether_intr_disable(int index)
{
  *NE2000_CR  = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STP;
  *NE2000_IMR = 0x00;
}

int ether_checkintr(int index)
{
  unsigned char status;
#if 0
  status = *H8_3069F_ISR;
  return (status & 0x20) ? 1 : 0;
#else
  *NE2000_CR = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  status = *NE2000_ISR;
  return (status & 0x01) ? 1 : 0;
#endif
}

// Ethernet Phy の割込みフラグをクリア
static int clear_irq(int index)
{
  if (ether_checkintr(index)) {
    // H8 のほうの ISR レジスタをクリアする
    *H8_3069F_ISR = 0x00;
    // ページ0にしてコマンドを準備
    *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
    // ステータスレジスタをクリア
    *NE2000_ISR   = 0xff;
  }
  return 0;
}

// Ethernet フレームを受信する
int ether_recv(int index, char *buf)
{
  unsigned char start, curr;
  unsigned char header[4];
  int size;

  // ページ0を選択しコマンドを開始
  *NE2000_CR = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  // リングバッファの次の領域を開始アドレスにする
  start = *NE2000_BNRY + 1;

  // ページ1を選択しコマンドを開始
  *NE2000_CR = NE2000_CR_P1 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  // CURR(current page register): パケットを受信するために使う最初のバッファのページアドレス
  curr = *NE2000_CURR;

  if (curr < start)
    // これはどういう意味？
    curr += NE2000_RP_STOP - NE2000_RP_START;
  if (start == curr)
    return 0;
  if (start == NE2000_RP_STOP)
    // リングバッファを戻す処理？
    start = NE2000_RP_START;

  // まずパケットのサイズを受信
  read_data(start * 256, 4, header);
  size = ((int)header[3] << 8) + header[2] - 4;

  // パケットの本体を受信
  read_data((start * 256) + 4, size, buf);

  // ページを0に
  *NE2000_CR = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  if (header[1] == NE2000_RP_START)
    header[1] = NE2000_RP_STOP;
  *NE2000_BNRY = header[1] - 1;

  clear_irq(index);

  return size;
}

// Ethernet フレームを送信する
int ether_send(int index, int size, char *buf)
{
  write_data(NE2000_TP_START * 256, size, buf);
        
  if(size < 60)
    size = 60;

  // ページ0でコマンドを準備
  *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_STA;
  // パケットサイズを2バイトで指定
  *NE2000_TBCR0 = size & 0xff;
  *NE2000_TBCR1 = (size >> 8) & 0xff;

  // TPSR(transmit page start register): 送信されるパケットのスタートページアドレス
  *NE2000_TPSR  = NE2000_TP_START;
  // TXP をセットし送信を開始
  *NE2000_CR    = NE2000_CR_P0 | NE2000_CR_RD_ABORT | NE2000_CR_TXP | NE2000_CR_STA;

  // 送信が完了するまで(TXP ビットがクリアされるまで)ビジーループで待つ
  while ((*NE2000_CR & NE2000_CR_TXP) != 0)
    ;

  return 0;
}
