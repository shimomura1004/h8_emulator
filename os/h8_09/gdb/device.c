#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "sim-main.h"
#include "gdb/sim-h8300.h"

/* #define USE_CALLBACK */

#define VECTOR_NOINTR 0xffff

#define H8_3069F_SCI0 ((volatile struct h8_3069f_sci *)&memory[0xffffb0])
#define H8_3069F_SCI1 ((volatile struct h8_3069f_sci *)&memory[0xffffb8])
#define H8_3069F_SCI2 ((volatile struct h8_3069f_sci *)&memory[0xffffc0])

#define DEFAULT_SCI H8_3069F_SCI1

struct h8_3069f_sci {
  volatile unsigned char smr;
  volatile unsigned char brr;
  volatile unsigned char scr;
  volatile unsigned char tdr;
  volatile unsigned char ssr;
  volatile unsigned char rdr;
  volatile unsigned char scmr;
};

#define H8_3069F_SCI_SCR_CKE0   (1<<0)
#define H8_3069F_SCI_SCR_CKE1   (1<<1)
#define H8_3069F_SCI_SCR_TEIE   (1<<2)
#define H8_3069F_SCI_SCR_MPIE   (1<<3)
#define H8_3069F_SCI_SCR_RE     (1<<4) /* 受信有効 */
#define H8_3069F_SCI_SCR_TE     (1<<5) /* 送信有効 */
#define H8_3069F_SCI_SCR_RIE    (1<<6) /* 受信割込み有効 */
#define H8_3069F_SCI_SCR_TIE    (1<<7) /* 送信割込み有効 */

#define H8_3069F_SCI_SSR_MPBT   (1<<0)
#define H8_3069F_SCI_SSR_MPB    (1<<1)
#define H8_3069F_SCI_SSR_TEND   (1<<2)
#define H8_3069F_SCI_SSR_PER    (1<<3)
#define H8_3069F_SCI_SSR_FERERS (1<<4)
#define H8_3069F_SCI_SSR_ORER   (1<<5)
#define H8_3069F_SCI_SSR_RDRF   (1<<6) /* 受信完了 */
#define H8_3069F_SCI_SSR_TDRE   (1<<7) /* 送信完了 */

static int serial_fd = -1;

static int search_freepty(host_callback *sim_callback)
{
  char ttydev[] = "/dev/ptyXX";
  int fd, i0 = 0, i1 = 0;
  char c0, c1;

  while ((c0 = "pqrsPQRS"[i0++]))
    {
      while ((c1 = "0123456789abcdefghijklmnopqrstuv"[i1++]))
	{
	  ttydev[8] = c0;
	  ttydev[9] = c1;
#ifdef USE_CALLBACK
	  fd = sim_callback->open (sim_callback, ttydev, O_RDWR);
#else
	  fd = open (ttydev, O_RDWR);
#endif
	  if (fd < 0)
	    continue;
	  ttydev[5] = 't';
	  (*sim_callback->printf_filtered) 
	    (sim_callback, "connect to %s\n", ttydev);
	  return fd;
	}
    }
  (*sim_callback->printf_filtered) (sim_callback,
				    "Free pty not found.\n");
  return -1;
}

static void serial_init(host_callback *sim_callback, unsigned char *memory)
{
  volatile struct h8_3069f_sci *sci = DEFAULT_SCI;

  sci->smr  = 0x00;
  sci->brr  = 0xff;
  sci->scr  = 0x00;
  sci->tdr  = 0xff;
  sci->ssr  = 0x84;
  sci->rdr  = 0x00;
  sci->scmr = 0xf2;

  serial_fd = search_freepty(sim_callback);
}

static int is_send_enable(host_callback *sim_callback, int fd)
{
  fd_set fds;
  struct timeval tm;
  int ret;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  tm.tv_sec  = 0;
  tm.tv_usec = 0;

  ret = select(fd + 1, NULL, &fds, NULL, &tm);
  if (ret > 0) {
    if (FD_ISSET(fd, &fds)) {
      return 1;
    }
  }

  return 0;
}

static int is_recv_enable(host_callback *sim_callback, int fd)
{
  fd_set fds;
  struct timeval tm;
  int ret;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  tm.tv_sec  = 0;
  tm.tv_usec = 0;

  ret = select(fd + 1, &fds, NULL, NULL, &tm);
  if (ret > 0) {
    if (FD_ISSET(fd, &fds)) {
      return 1;
    }
  }

  return 0;
}

static void send_byte(host_callback *sim_callback, int fd, unsigned char c)
{
#ifdef USE_CALLBACK
  sim_callback->write (sim_callback, fd, &c, 1);
#else
  write (fd, &c, 1);
#endif
  fsync(fd);
}

static unsigned char recv_byte(host_callback *sim_callback, int fd)
{
  unsigned char c;
  int size;
#ifdef USE_CALLBACK
  size = sim_callback->read (sim_callback, fd, &c, 1);
#else
  size = read (fd, &c, 1);
#endif
  if (size == 1) {
    return c;
  }
  return 0;
}

static int serial_run(host_callback *sim_callback, unsigned char *memory)
{
  volatile struct h8_3069f_sci *sci = DEFAULT_SCI;
  unsigned char c;
  static int send_intr = 0, recv_intr = 0;
  int vector = VECTOR_NOINTR;

  if (sci->scr & H8_3069F_SCI_SCR_TE) {
    if (!(sci->ssr & H8_3069F_SCI_SSR_TDRE)) {
      if (is_send_enable(sim_callback, serial_fd)) {
	send_byte(sim_callback, serial_fd, sci->tdr);
	sci->ssr |= H8_3069F_SCI_SSR_TDRE;
	if (sci->scr & H8_3069F_SCI_SCR_TIE)
	  send_intr++;
      } else {
	send_intr = 0;
      }
    }
  }
  send_intr =
    (sci->ssr & H8_3069F_SCI_SSR_TDRE) && (sci->scr & H8_3069F_SCI_SCR_TIE);

  if (sci->scr & H8_3069F_SCI_SCR_RE) {
    if (!(sci->ssr & H8_3069F_SCI_SSR_RDRF)) {
      if (is_recv_enable(sim_callback, serial_fd)) {
	c = recv_byte(sim_callback, serial_fd);
	sci->rdr = c;
	sci->ssr |= H8_3069F_SCI_SSR_RDRF;
	if (sci->scr & H8_3069F_SCI_SCR_RIE)
	  recv_intr++;
      } else {
	recv_intr = 0;
      }
    }
  }
  recv_intr =
    (sci->ssr & H8_3069F_SCI_SSR_RDRF) && (sci->scr & H8_3069F_SCI_SCR_RIE);

  if (send_intr) vector = 54;
  if (recv_intr) vector = 53;

  return vector;
}

#define VECTYPE_IMIA0 24
#define VECTYPE_IMIB0 25
#define VECTYPE_OVI0  26
#define VECTYPE_IMIA1 28
#define VECTYPE_IMIB1 29
#define VECTYPE_OVI1  30
#define VECTYPE_IMIA2 32
#define VECTYPE_IMIB2 33
#define VECTYPE_OVI2  34

#define H8_3069F_TIM16 ((volatile struct h8_3069f_tim16 *)&memory[0xffff60])

struct h8_3069f_tim16 {
  volatile unsigned char tstr;
  volatile unsigned char tsnc;
  volatile unsigned char tmdr;
  volatile unsigned char tolr;
  volatile unsigned char tisra;
  volatile unsigned char tisrb;
  volatile unsigned char tisrc;
};

#define H8_3069F_TIM16_REGS0 0xffff68
#define H8_3069F_TIM16_REGS1 0xffff70
#define H8_3069F_TIM16_REGS2 0xffff78
#define H8_3069F_TIM16_NUM 3

struct h8_3069f_tim16_ch {
  volatile unsigned char tcr;
  volatile unsigned char tior;
  volatile unsigned char tcnth;
  volatile unsigned char tcntl;
  volatile unsigned char grah;
  volatile unsigned char gral;
  volatile unsigned char grbh;
  volatile unsigned char grbl;
};

#define H8_3069F_TIM16_TSTR_STR0 (1<<0)
#define H8_3069F_TIM16_TSTR_STR1 (1<<1)
#define H8_3069F_TIM16_TSTR_STR2 (1<<2)

#define H8_3069F_TIM16_TSNC_SYNC0 (1<<0)
#define H8_3069F_TIM16_TSNC_SYNC1 (1<<1)
#define H8_3069F_TIM16_TSNC_SYNC2 (1<<2)

#define H8_3069F_TIM16_TMDR_PWM0 (1<<0)
#define H8_3069F_TIM16_TMDR_PWM1 (1<<1)
#define H8_3069F_TIM16_TMDR_PWM2 (1<<2)
#define H8_3069F_TIM16_TMDR_FDIR (1<<5)
#define H8_3069F_TIM16_TMDR_MDF  (1<<6)

#define H8_3069F_TIM16_TOLR_TOA0 (1<<0)
#define H8_3069F_TIM16_TOLR_TOB0 (1<<1)
#define H8_3069F_TIM16_TOLR_TOA1 (1<<2)
#define H8_3069F_TIM16_TOLR_TOB1 (1<<3)
#define H8_3069F_TIM16_TOLR_TOA2 (1<<4)
#define H8_3069F_TIM16_TOLR_TOB2 (1<<5)

#define H8_3069F_TIM16_TISRX_IMFX0  (1<<0)
#define H8_3069F_TIM16_TISRX_IMFX1  (1<<1)
#define H8_3069F_TIM16_TISRX_IMFX2  (1<<2)
#define H8_3069F_TIM16_TISRX_IMIEX0 (1<<4)
#define H8_3069F_TIM16_TISRX_IMIEX1 (1<<5)
#define H8_3069F_TIM16_TISRX_IMIEX2 (1<<6)

#define H8_3069F_TIM16_TISRC_OVF0   (1<<0)
#define H8_3069F_TIM16_TISRC_OVF1   (1<<1)
#define H8_3069F_TIM16_TISRC_OVF2   (1<<2)
#define H8_3069F_TIM16_TISRC_OVIE0  (1<<4)
#define H8_3069F_TIM16_TISRC_OVIE1  (1<<5)
#define H8_3069F_TIM16_TISRC_OVIE2  (1<<6)

#define H8_3069F_TIM16_CH_TCR_TPSC_PER1 0
#define H8_3069F_TIM16_CH_TCR_TPSC_PER2 1
#define H8_3069F_TIM16_CH_TCR_TPSC_PER4 2
#define H8_3069F_TIM16_CH_TCR_TPSC_PER8 3
#define H8_3069F_TIM16_CH_TCR_CKEG_UP    0
#define H8_3069F_TIM16_CH_TCR_CKEG_DOWN (1<<3)
#define H8_3069F_TIM16_CH_TCR_CKEG_EACH (2<<3)
#define H8_3069F_TIM16_CH_TCR_CCLR_GRA  (1<<5)
#define H8_3069F_TIM16_CH_TCR_CCLR_GRB  (2<<5)
#define H8_3069F_TIM16_CH_TCR_CCLR_SYNC (3<<5)

#define H8_3069F_TIM16_CH_TIOR_IOA_NOUT   0
#define H8_3069F_TIM16_CH_TIOR_IOA_OUT0   1
#define H8_3069F_TIM16_CH_TIOR_IOA_OUT1   2
#define H8_3069F_TIM16_CH_TIOR_IOA_TOGGLE 3
#define H8_3069F_TIM16_CH_TIOR_IOB_NOUT   0
#define H8_3069F_TIM16_CH_TIOR_IOB_OUT0   (1<<4)
#define H8_3069F_TIM16_CH_TIOR_IOB_OUT1   (2<<4)
#define H8_3069F_TIM16_CH_TIOR_IOB_TOGGLE (3<<4)

struct timer16_params {
  int regs;
  int tstr_str;
  int tisrx_imfx;
  int tisrx_imiex;
  int tisrc_ovf;
  int tisrc_ovie;
  int vectype_imia;
  int vectype_imib;
  int vectype_ovi;
} timer16_channels[] = {
  {
    H8_3069F_TIM16_REGS0,
    H8_3069F_TIM16_TSTR_STR0,
    H8_3069F_TIM16_TISRX_IMFX0, H8_3069F_TIM16_TISRX_IMIEX0,
    H8_3069F_TIM16_TISRC_OVF0,  H8_3069F_TIM16_TISRC_OVIE0,
    VECTYPE_IMIA0, VECTYPE_IMIB0, VECTYPE_OVI0,
  },
  {
    H8_3069F_TIM16_REGS1,
    H8_3069F_TIM16_TSTR_STR1,
    H8_3069F_TIM16_TISRX_IMFX1, H8_3069F_TIM16_TISRX_IMIEX1,
    H8_3069F_TIM16_TISRC_OVF1,  H8_3069F_TIM16_TISRC_OVIE1,
    VECTYPE_IMIA1, VECTYPE_IMIB1, VECTYPE_OVI1,
  },
  {
    H8_3069F_TIM16_REGS2,
    H8_3069F_TIM16_TSTR_STR2,
    H8_3069F_TIM16_TISRX_IMFX2, H8_3069F_TIM16_TISRX_IMIEX2,
    H8_3069F_TIM16_TISRC_OVF2,  H8_3069F_TIM16_TISRC_OVIE2,
    VECTYPE_IMIA2, VECTYPE_IMIB2, VECTYPE_OVI2,
  },
};

static int timer16_init(host_callback *sim_callback, unsigned char *memory)
{
  volatile struct h8_3069f_tim16 *tim16 = H8_3069F_TIM16;
  struct h8_3069f_tim16_ch *ch;
  int i;

  tim16->tstr  = 0xf8;
  tim16->tisra = 0x88;
  tim16->tisrb = 0x88;
  tim16->tisrc = 0x88;

  for (i = 0; i < H8_3069F_TIM16_NUM; i++) {
    ch = (struct h8_3069f_tim16_ch *)&memory[timer16_channels[i].regs];
    ch->tcr   = 0x80;
    ch->tcnth = 0x00;
    ch->tcntl = 0x00;
    ch->grah  = 0x00;
    ch->gral  = 0x00;
    ch->grbh  = 0x00;
    ch->grbl  = 0x00;
  }

  return 0;
}

int timer16_run(host_callback *sim_callback, unsigned char *memory)
{
  int vector = VECTOR_NOINTR, i, per;
  volatile struct h8_3069f_tim16 *tim16 = H8_3069F_TIM16;
  struct h8_3069f_tim16_ch *ch;
  struct timer16_params *params;
  short tcnt, gra, grb;
  static int clk = 0;

  tim16 = H8_3069F_TIM16;
  clk++;

  for (i = 0; i < H8_3069F_TIM16_NUM; i++) {
    params = &timer16_channels[i];
    ch = (struct h8_3069f_tim16_ch *)&memory[params->regs];

    if (!(tim16->tstr & params->tstr_str))
      continue;

    switch (ch->tcr & 0x3) {
    case H8_3069F_TIM16_CH_TCR_TPSC_PER1: per = 1; break;
    case H8_3069F_TIM16_CH_TCR_TPSC_PER2: per = 2; break;
    case H8_3069F_TIM16_CH_TCR_TPSC_PER4: per = 4; break;
    case H8_3069F_TIM16_CH_TCR_TPSC_PER8: per = 8; break;
    }

    if (!(ch->tcr & H8_3069F_TIM16_CH_TCR_CKEG_EACH))
      per *= 2;

    if ((clk % per) != 0)
      continue;

    tcnt = ch->tcnth << 8 | ch->tcntl;
    gra  = ch->grah  << 8 | ch->gral;
    grb  = ch->grbh  << 8 | ch->grbl;
    tcnt++;

    if (tcnt == gra) {
      tim16->tisra |= params->tisrx_imfx;
      if ((ch->tcr & 0x60) == H8_3069F_TIM16_CH_TCR_CCLR_GRA)
        tcnt = 0;
    }

    if (tcnt == grb) {
      tim16->tisrb |= params->tisrx_imfx;
      if ((ch->tcr & 0x60) == H8_3069F_TIM16_CH_TCR_CCLR_GRB)
        tcnt = 0;
    }

    if (tcnt == 0) {
      tim16->tisrc |= params->tisrc_ovf;
    }

    ch->tcnth = (tcnt >> 8) & 0xff;
    ch->tcntl = tcnt & 0xff;

    if (vector == VECTOR_NOINTR) {
      if (tim16->tisra & params->tisrx_imiex)
        if (tim16->tisra & params->tisrx_imfx)
          vector = params->vectype_imia;
    }

    if (vector == VECTOR_NOINTR) {
      if (tim16->tisrb & params->tisrx_imiex)
        if (tim16->tisrb & params->tisrx_imfx)
          vector = params->vectype_imib;
    }

    if (vector == VECTOR_NOINTR) {
      if (tim16->tisrc & params->tisrc_ovie)
        vector = params->vectype_ovi;
    }
  }

  return vector;
}

static int get_free_dev(char *devname, char *ifname)
{
  int i, fd;
  char name[20];

  for (i = 0; i < 16; i++) {
    sprintf(name, "/dev/%s%d", devname, i);
    fd = open(name, O_RDWR);
    if (fd >= 0) {
      if (ifname) sprintf(ifname, "%s%d", devname, i);
      return (fd);
    }
  }

  return (-1);
}

static int ether_fd = -1;
static char ether_buf[256 * 0x80];

static void ether_init(host_callback *sim_callback, unsigned char *memory)
{
  char ifname[16];

  ether_fd = get_free_dev("tap", ifname);
  if (ether_fd < 0) {
    (*sim_callback->printf_filtered) 
      (sim_callback, "Free tap not found.\n");
    (*sim_callback->printf_filtered) 
      (sim_callback, "Need executing kldload if_tap.\n");
    return;
  }

  /* MACアドレス */
  memcpy(ether_buf, "\x00\x00\x11\x00\x22\x00\x33\x00\x44\x00\x55\x00", 12);

  (*sim_callback->printf_filtered) 
    (sim_callback, "interface: %s\n", ifname);

  return;
}

#define RTL8019_ADDR 0x200000
#define NE2000_CR      (memory[RTL8019_ADDR + 0x00])
#define RTL8019_RDMAP  (memory[RTL8019_ADDR + 0x10])
#define NE2000_PSTART  (registers[0][0x01])
#define NE2000_PSTOP   (registers[0][0x02])
#define NE2000_BNRY    (registers[0][0x03])
#define NE2000_TPSR    (registers[0][0x04])
#define NE2000_TBCR0   (registers[0][0x05])
#define NE2000_TBCR1   (registers[0][0x06])
#define NE2000_ISR     (registers[0][0x07])
#define NE2000_RSAR0   (registers[0][0x08])
#define NE2000_RSAR1   (registers[0][0x09])
#define NE2000_RBCR0   (registers[0][0x0a])
#define NE2000_RBCR1   (registers[0][0x0b])
#define NE2000_RCR     (registers[0][0x0c])
#define NE2000_TCR     (registers[0][0x0d])
#define NE2000_DCR     (registers[0][0x0e])
#define NE2000_IMR     (registers[0][0x0f])
#define NE2000_CURR    (registers[1][0x07])

#define RTL8019_SIM_CR  (memory[RTL8019_ADDR + 0xfe])
#define RTL8019_SIM_SR  (memory[RTL8019_ADDR + 0xff])
#define RTL8019_SIM_SR_NRDY (1 << 0)

#define NE2000_CR_P0       (0 << 6)
#define NE2000_CR_P1       (1 << 6)
#define NE2000_CR_P2       (2 << 6)
#define NE2000_CR_P3       (3 << 6)
#define NE2000_CR_RD_WRITE (2 << 3)
#define NE2000_CR_RD_READ  (1 << 3)
#define NE2000_CR_TXP      (1 << 2)
#define NE2000_ISR_RDC     (1 << 6)

#define NE2000_RCR_MON     (1 << 5)
#define NE2000_RCR_PRO     (1 << 4)
#define NE2000_RCR_AM      (1 << 3)
#define NE2000_RCR_AB      (1 << 2)

#define NE2000_TCR_ELB     (2 << 1)
#define NE2000_TCR_ILB     (1 << 1)
#define NE2000_TCR_NORMAL  (0 << 1)

static unsigned char registers[4][0x10];

static int ether_run(host_callback *sim_callback, unsigned char *memory)
{
  int i, c, ret;
  int size, addr, count = 0;
  int new_curr;
  int vector = VECTOR_NOINTR;
  char buf[4096];
  static int page = 0;

  memcpy(&registers[page][1], &memory[RTL8019_ADDR + 1], 0x0f);

  switch (NE2000_CR & 0xc0) {
  case NE2000_CR_P0: page = 0; break;
  case NE2000_CR_P1: page = 1; break;
  case NE2000_CR_P2: page = 2; break;
  case NE2000_CR_P3: page = 3; break;
  default: page = 0; break;
  }

  /* 割り込みのクリア */
  if (NE2000_ISR == 0xff)
    NE2000_ISR = 0;

  /* 受信した */
  if (is_recv_enable(sim_callback, ether_fd)) {
    size = read(ether_fd, buf, sizeof(buf));
    if (size == sizeof(buf)) {
      fprintf(stderr, "buffer over %d\n", size);
      exit (1);
    }

    if ((NE2000_RCR & NE2000_RCR_PRO) ||
	((NE2000_RCR & NE2000_RCR_MON) &&
	 !memcmp(buf, "\x00\x11\x22\x33\x44\x55", 6)) ||
	((NE2000_RCR & NE2000_RCR_AB) &&
	 !memcmp(buf, "\xff\xff\xff\xff\xff\xff", 6)) ||
	((NE2000_RCR & NE2000_RCR_AM) && (buf[0] & 1))
	) {

      /* 必要ブロック数を加算(ヘッダがあるので＋４している) */
      new_curr = NE2000_CURR + ((size + 4 + 255) / 256);
      if (new_curr >= NE2000_PSTOP)
	new_curr -= (NE2000_PSTOP - NE2000_PSTART);

      size += 4; /* CRCのぶんだけサイズを追加 */

      /* ヘッダ */
      ether_buf[(NE2000_CURR * 256) + 0] = 0;
      ether_buf[(NE2000_CURR * 256) + 1] = new_curr;
      ether_buf[(NE2000_CURR * 256) + 2] = size & 0xff;
      ether_buf[(NE2000_CURR * 256) + 3] = (size >> 8) & 0xff;

      size -= 4;

      c = NE2000_CURR * 256 + 4; /* ヘッダがあるので＋４している */
      for (i = 0; i < size; i++) {
	ether_buf[c] = buf[i];
	if (++c >= NE2000_PSTOP * 256)
	  c = NE2000_PSTART * 256;
      }

      NE2000_CURR = new_curr;

      NE2000_ISR |= 0x01;
    }
  }

  if ((NE2000_ISR & 0x01) && (NE2000_IMR & 0x01)) {
    vector = 17; /* IRQ5 */
  }

  /* 送信処理 */
  if (NE2000_CR & NE2000_CR_TXP) {
    size = NE2000_TBCR1 << 8 | NE2000_TBCR0;
    c = NE2000_TPSR * 256;

    for (i = 0; i < size; i++) {
      buf[i] = ether_buf[c];
      if (++c >= NE2000_PSTART * 256)
	c = NE2000_TPSR * 256;
    }

    write(ether_fd, buf, size);

    NE2000_CR &= ~NE2000_CR_TXP;
  }

  /* バッファのリード／ライト処理 */
  if (RTL8019_SIM_SR & RTL8019_SIM_SR_NRDY) {
    size = (NE2000_RBCR1 << 8) | NE2000_RBCR0;
    addr = (NE2000_RSAR1 << 8) | NE2000_RSAR0;

    if (NE2000_CR & NE2000_CR_RD_READ)
      RTL8019_RDMAP = ether_buf[addr];

    if (NE2000_CR & NE2000_CR_RD_WRITE)
      ether_buf[addr] = RTL8019_RDMAP;

    addr++;
    if (addr == NE2000_TPSR * 256)
      addr = 0;
    if (addr == NE2000_PSTART * 256)
      addr = NE2000_TPSR * 256;
    if (addr == NE2000_PSTOP * 256)
      addr = NE2000_PSTART * 256;

    NE2000_RSAR0 = addr & 0xff;
    NE2000_RSAR1 = (addr >> 8) & 0xff;

    /* ほんとはsizeぶん読み書きしたら立てるべきなのかも */
    NE2000_ISR |= NE2000_ISR_RDC;

    RTL8019_SIM_SR &= ~RTL8019_SIM_SR_NRDY;
  }

  memcpy(&memory[RTL8019_ADDR + 1], &registers[page][1], 0x0f);

  return vector;
}

void sim_device_init(host_callback *sim_callback, unsigned char *memory)
{
  static int initialized = 0;

  if (initialized)
    return;

  serial_init(sim_callback, memory);
  timer16_init(sim_callback, memory);
  ether_init(sim_callback, memory);

  initialized++;
}

int sim_device_sleep(host_callback *sim_callback, unsigned char *memory)
{
  fd_set fds;
  int ret, fd;

  FD_ZERO(&fds);
  FD_SET(serial_fd, &fds);
  FD_SET(ether_fd, &fds);

  fd = serial_fd;
  if (fd < ether_fd) fd = ether_fd;

  ret = select(fd + 1, &fds, NULL, NULL, NULL);
  if (ret > 0) {
    if (FD_ISSET(fd, &fds)) {
      return 1;
    }
  }

  return 0;
}

int sim_device_run(host_callback *sim_callback, unsigned char *memory)
{
  int vec, vector = VECTOR_NOINTR;

  vec    = timer16_run(sim_callback, memory);
  vector = (vec < vector) ? vec : vector;

  vec    = serial_run(sim_callback, memory);
  vector = (vec < vector) ? vec : vector;

  vec    = ether_run(sim_callback, memory);
  vector = (vec < vector) ? vec : vector;

  return (vector == VECTOR_NOINTR) ? 0 : vector;
}
