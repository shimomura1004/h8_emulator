#include "defines.h"
#include "kozos.h"
#include "intr.h"
#include "interrupt.h"
#include "timer.h"
#include "lib.h"
#include "timerdrv.h"

struct timerbuf {
  struct timerbuf *next;
  kz_msgbox_id_t id; /* タイマ満了時のメッセージ送信先 */
  // 前のタイマからの差分の時間
  int msec;
};

static struct timerreg {
  struct timerbuf *timers; /* タイマ・バッファのリンクリスト */
  int index; /* 利用するタイマの番号 */
} timerreg;

/*
 * 以下は割込みハンドラであり，非同期で呼ばれるので，ライブラリ関数などを
 * 呼び出す場合には注意が必要．
 * 基本として，以下のいずれかに当てはまる関数しか呼び出してはいけない．
 * ・再入可能である．
 * ・スレッドから呼ばれることは無い関数である．
 * ・スレッドから呼ばれることがあるが，割込み禁止で呼び出している．
 * また非コンテキスト状態で呼ばれるため，システム・コールは利用してはいけない．
 * (サービス・コールを利用すること)
 */
static void timerdrv_intr(void)
{
  struct timerreg *tim = &timerreg;

  if (timer_is_expired(tim->index)) { /* タイマ割込み */
    timer_cancel(tim->index);
    kx_send(MSGBOX_ID_TIMDRIVE, TIMERDRV_CMD_EXPIRE, NULL);
  }
}

// 初期化時にはタイマを1つ作成
static int timerdrv_init(void)
{
  memset(&timerreg, 0, sizeof(timerreg));
  timerreg.index = TIMER_DEFAULT_DEVICE;
  return 0;
}

/* スレッドからの要求を処理する */
static int timerdrv_command(struct timerreg *tim, int cmd, char *p)
{
  struct timerbuf *tmbuf;
  struct timerbuf **tmbufp;
  struct timerreq *req;
  int t, msec;

  switch (cmd) {
  case TIMERDRV_CMD_EXPIRE: /* タイマ満了 */
    // 先頭のタイマを tmbuf に取り出し
    tmbuf = tim->timers;
    // 空ならなにもしない
    if (tmbuf) {
      // リストを1つすすめる
      tim->timers = tmbuf->next;
      // タイマが完了したことをメッセージボックスに通知
      kz_send(tmbuf->id, 0, NULL);
      kz_kmfree(tmbuf);
      // まだリストにタイマがあれば、開始する
      if (tim->timers)
        timer_start(tim->index, tim->timers->msec, 0);
    }
    break;

  case TIMERDRV_CMD_START: /* タイマのスタート */
    req = (struct timerreq *)p;

    // 新しいタイマへのエントリを作成
    tmbuf = kz_kmalloc(sizeof(*tmbuf));
    tmbuf->next = NULL;
    tmbuf->id   = req->id;
    tmbuf->msec = req->msec;

    t = 0;
    // もしリストに要素があれば
    if (tim->timers) {
      // タイマの現在値を取得しておく
      t = timer_gettime(tim->index);
    }

    for (tmbufp = &tim->timers;; tmbufp = &(*tmbufp)->next) {
      // リンク末尾に到達した
      if (*tmbufp == NULL) {
        // 新たなエントリを末尾につなぐ
        *tmbufp = tmbuf;
        // 先頭につないだあと、先頭のタイマ(つまり今つないだタイマ)を開始させる
        if (tmbufp == &tim->timers)
          timer_start(tim->index, tim->timers->msec, 0);
        break;
      }
      // 今見ているタイマエントリの秒数から現在の値を引き、残りの秒数(msec)を計算
      msec = (*tmbufp)->msec - t;
      // もう超えてしまっている場合は 0 に
      if (msec < 0) msec = 0;
      // 新しく作るタイマエントリと今見ているタイマエントリの残り時間を比較
      // 新しく作るタイマエントリのほうが先に通知されるなら
      if (tmbuf->msec < msec) {
        // 新しく作るタイマエントリの秒数分、今のエントリの秒数を減らす
        (*tmbufp)->msec = msec - tmbuf->msec;
        // リンクのつなぎかえ
        tmbuf->next = *tmbufp;
        *tmbufp = tmbuf;
        // リストの先頭のタイマを開始
        timer_start(tim->index, tim->timers->msec, 0);
        break;
      }
      // つなごうとしているタイマのほうが遅い場合は次へ
      // ただしつなごうとしているタイマの残り時間を減らしておく
      t = 0;
      tmbuf->msec -= msec;
    }

    kz_kmfree(p);
    break;

  default:
    break;
  }

  return 0;
}

int timerdrv_main(int argc, char *argv[])
{
  int cmd;
  char *p;

  timerdrv_init();
  kz_setintr(SOFTVEC_TYPE_TIMINTR, timerdrv_intr); /* 割込みハンドラ設定 */

  while (1) {
    // timerdrv は、メッセージを受信したら処理することを繰り返すタスク
    kz_recv(MSGBOX_ID_TIMDRIVE, &cmd, &p);
    timerdrv_command(&timerreg, cmd, p);
  }

  return 0;
}
