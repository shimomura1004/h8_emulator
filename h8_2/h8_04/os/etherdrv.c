#include "defines.h"
#include "kozos.h"
#include "intr.h"
#include "interrupt.h"
#include "ether.h"
#include "lib.h"
#include "etherdrv.h"

#define ETHER_BUFFER_SIZE 1800

/* 割込みハンドラ */
static void etherdrv_intr(void)
{
  // Ethernet Phy から割込みが発生した場合
  // つまり Ethernet フレームを受信した場合
  int size;
  char *buffer;
  if (ether_checkintr(0)) {
    buffer = kx_kmalloc(ETHER_BUFFER_SIZE);
    size = ether_recv(0, buffer);
    if (size > 0)
      // Ethernet フレームを受信したというメッセージを送る
      kx_send(MSGBOX_ID_ETHINPUT, size, buffer);
    else
      kx_kmfree(buffer);
  }
}

static int etherdrv_init(void)
{
  return 0;
}

/* スレッドからの要求を処理する */
static int etherdrv_command(int size, char *command)
{
  // 1文字目がコマンド
  switch (command[0]) {
  case ETHERDRV_CMD_USE: /* イーサネット・ドライバの使用開始 */
    ether_init(0);
    ether_intr_enable(0); /* 受信割込み有効化(受信開始) */
    break;

  case ETHERDRV_CMD_SEND: /* イーサネットへのフレーム出力 */
    // Ethernet フレームを出力
    ether_send(0, size - 1, command + 1);
    break;

  default:
    break;
  }

  return 0;
}

int etherdrv_main(int argc, char *argv[])
{
  int size;
  char *p;

  etherdrv_init();
  kz_setintr(SOFTVEC_TYPE_ETHINTR, etherdrv_intr); /* 割込みハンドラ設定 */

  while (1) {
    // Ethernet で送信したいデータを受け取る
    kz_recv(MSGBOX_ID_ETHOUTPUT, &size, &p);
    // 内容に応じて処理し、メモリを解放
    etherdrv_command(size, p);
    kz_kmfree(p);
  }

  return 0;
}
