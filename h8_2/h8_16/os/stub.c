#include "defines.h"
#include "kozos.h"
#include "intr.h"
#include "stub.h"

enum {
  STUB_SIGHUP  =  1,
  STUB_SIGILL  =  4,
  STUB_SIGTRAP =  5,
  STUB_SIGEMT  =  7,
  STUB_SIGBUS  = 10,
  STUB_SIGSEGV = 11,
};

#define BUFFER_SIZE 2048
static char recvbuf[BUFFER_SIZE];
static char sendbuf[BUFFER_SIZE];

#define REGISTERS_NUM 10
#define REGISTERS_SIZE (4 * REGISTERS_NUM)

/* gdb/include/gdb/sim-h8300.h */
/* gdb/h8300-tdep.c */
enum {
  REG_ER0, REG_ER1, REG_ER2, REG_ER3,
  REG_ER4, REG_ER5, REG_ER6, REG_ER7,
  REG_CCR, REG_PC,
};

static unsigned long registers[REGISTERS_NUM];

/* 独立したシリアルドライバを利用するため，別名を定義してインポートする */
#define serial_init debug_serial_init
#define serial_is_send_enable debug_serial_is_send_enable
#define serial_send_byte debug_serial_send_byte
#define serial_is_recv_enable debug_serial_is_recv_enable
#define serial_recv_byte debug_serial_recv_byte
#define serial_intr_is_send_enable debug_serial_intr_is_send_enable
#define serial_intr_send_enable debug_serial_intr_send_enable
#define serial_intr_send_disable debug_serial_intr_send_disable
#define serial_intr_is_recv_enable debug_serial_intr_is_recv_enable
#define serial_intr_recv_enable debug_serial_intr_recv_enable
#define serial_intr_recv_disable debug_serial_intr_recv_disable
#include "serial.c"

static void putDebugChar(char c)
{
  serial_send_byte(SERIAL_DEFAULT_DEVICE, c);
}

static int getDebugChar()
{
  return serial_recv_byte(SERIAL_DEFAULT_DEVICE);
}

static char *stub_strcpy(char *dst, const char *src)
{
  char *d = dst;
  for (;; dst++, src++) {
    *dst = *src;
    if (!*src) break;
  }
  return d;
}

static int a2h(unsigned char c)
{
  if ((c >= '0') && (c <= '9'))
    return c - '0';
  if ((c >= 'a') && (c <= 'f'))
    return c - 'a' + 10;
  if ((c >= 'A') && (c <= 'F'))
    return c - 'A' + 10;
  return -1;
}

static char h2a(int val)
{
  if ((val >= 0) && (val < 16))
    return "0123456789abcdef"[val];
  return '\0';
}

static unsigned char *stub_gets(unsigned char *buffer)
{
  int i = 0;
  unsigned char c = 0, sum = 0, datasum = 0;
  enum {
    GETS_STATUS_WAIT_DOLLAR,
    GETS_STATUS_READY_READ,
    GETS_STATUS_READ_DATA,
    GETS_STATUS_READ_SUM_UPPER,
    GETS_STATUS_READ_SUM_LOWER,
    GETS_STATUS_END,
  } status;

  status = GETS_STATUS_WAIT_DOLLAR;

  while (status != GETS_STATUS_END) {
    if (status != GETS_STATUS_READY_READ)
      c = getDebugChar();
    switch (status) {
    case GETS_STATUS_WAIT_DOLLAR:
      if (c == '$') status = GETS_STATUS_READY_READ;
      break;
    case GETS_STATUS_READY_READ:
      i = 0;
      sum = datasum = 0;
      status = GETS_STATUS_READ_DATA;
      break;
    case GETS_STATUS_READ_DATA:
      if (c == '$') {
	status = GETS_STATUS_READY_READ;
	break;
      }
      if (c == '#') {
	buffer[i] = '\0';
	status = GETS_STATUS_READ_SUM_UPPER;
	break;
      }
      sum += c;
      buffer[i++] = c;
      break;
    case GETS_STATUS_READ_SUM_UPPER:
      datasum = a2h(c) << 4;
      status = GETS_STATUS_READ_SUM_LOWER;
      break;
    case GETS_STATUS_READ_SUM_LOWER:
      datasum |= a2h(c);
      if (sum != datasum) {
	putDebugChar('-');
	status = GETS_STATUS_WAIT_DOLLAR;
      } else {
	status = GETS_STATUS_END;
      }
      break;
    default:
      break;
    }
  }

  putDebugChar('+');
  if ((i > 2) && (buffer[2] == ':')) {
    putDebugChar(buffer[0]);
    putDebugChar(buffer[1]);
    return &buffer[3];
  }
  return buffer;
}

static unsigned char *stub_puts(unsigned char *buffer)
{
  unsigned char c, sum;
  int i;

  while (1) {
    putDebugChar('$');

    i = 0;
    sum = 0;
    while ((c = buffer[i++]) != '\0') {
      putDebugChar(c);
      sum += c;
    }

    putDebugChar('#');
    putDebugChar(h2a((sum >> 4) & 0xf));
    putDebugChar(h2a(sum & 0xf));

    if (getDebugChar() == '+')
      break;
  }

  return buffer;
}

static unsigned char *read_memory(void *addr, unsigned char *buffer, int size)
{
  unsigned char c;
  int i;

  for (i = 0; i < size; i++) {
    c = ((unsigned char *)addr)[i];
    *(buffer++) = h2a((c >> 4) & 0xf);
    *(buffer++) = h2a(c & 0xf);
  }
  *buffer = 0;

  return buffer;
}

static void *write_memory(unsigned char *buffer, void *addr, int size)
{
  unsigned char c;
  unsigned char *cp = addr;
  int i;

  for (i = 0; i < size; i++) {
    c  = a2h(*(buffer++)) << 4;
    c |= a2h(*(buffer++));
    *(cp++) = c;
  }

  return cp;
}

static int a2val(unsigned char **pp, long *valp)
{
  int size = 0;
  long v, val = 0;
  unsigned char *p;

  p = *pp;
  while (1) {
    v = a2h(*p);
    if (v < 0) break;
    val = (val << 4) | v;
    p++;
    size++;
  }
  *valp = val;
  *pp = p;

  return size;
}

static void handle_exception()
{
  int sig = STUB_SIGTRAP;
  unsigned char *p;
  long addr, size;

  p = sendbuf;
  *(p++) = 'T';
  *(p++) = h2a((sig >> 4) & 0xf);
  *(p++) = h2a(sig & 0xf);

#if 0
  *(p++) = h2a((REG_PC >> 4) & 0xf);
  *(p++) = h2a(REG_PC & 0xf);
  *(p++) = ':';
  p = read_memory((char *)&registers[REG_PC], p, 4);
  *(p++) = ';';
#endif
  *(p++) = '\0';

  stub_puts(sendbuf);

  while (1) {
    stub_gets(recvbuf);

    sendbuf[0] = '\0'; /* not support */

    p = recvbuf;
    switch (*(p++)) {
    case '?':
      sendbuf[0] = 'S';
      sendbuf[1] = h2a((sig >> 4) & 0xf);
      sendbuf[2] = h2a(sig & 0xf);
      sendbuf[3] = '\0';
      break;

    case 'g':
      read_memory(registers, sendbuf, REGISTERS_SIZE);
      break;

    case 'G':
      break;
    case 'm':
      a2val(&p, &addr);
      if (*(p++) != ',') {
	stub_strcpy(sendbuf, "E01");
	break;
      }
      a2val(&p, &size);
      read_memory((void *)addr, sendbuf, size);
      break;
    case 'M':
      a2val(&p, &addr);
      if (*(p++) != ',') {
	stub_strcpy(sendbuf, "E01");
	break;
      }
      a2val(&p, &size);
      if (*(p++) != ':') {
	stub_strcpy(sendbuf, "E02");
	break;
      }
      write_memory(p, (void *)addr, size);
      break;
    case 'c':

      /*
       * breakpoint:
       remote.c:remote_insert_breakpoint()
       ->
       memory_insert_breakpoint()
       ->
       mem-break.c:default_memory_insert_breakpoint()
       ->
       gdbarch_breakpoint_from_pc()
       ->
       h8300-tdep.c:h8300_breakpoint_from_pc()

       h8300_breakpoint_from_pc()
       breakpoint[] = { 0x7A, 0xFF }; bpt instruction???
       breakpoint[] = { 0x01, 0x80 }; ??? sleep instruction???
       */

      return;
    default:
      break;
    }
    stub_puts(sendbuf);
  }

  return;
}

static void store_registers(unsigned long sp)
{
  registers[REG_ER0] = ((unsigned long *)sp)[0];
  registers[REG_ER1] = ((unsigned long *)sp)[1];
  registers[REG_ER2] = ((unsigned long *)sp)[2];
  registers[REG_ER3] = ((unsigned long *)sp)[3];
  registers[REG_ER4] = ((unsigned long *)sp)[4];
  registers[REG_ER5] = ((unsigned long *)sp)[5];
  registers[REG_ER6] = ((unsigned long *)sp)[6];
  registers[REG_ER7] = sp + sizeof(int) * 8;

  registers[REG_CCR] = ((((unsigned long *)sp)[7]) >> 24) & 0xff;
  registers[REG_PC ] = ((unsigned long *)sp)[7] & 0xffffff;
}

static void restore_registers(unsigned long sp)
{
  ((unsigned long *)sp)[0] = registers[REG_ER0];
  ((unsigned long *)sp)[1] = registers[REG_ER1];
  ((unsigned long *)sp)[2] = registers[REG_ER2];
  ((unsigned long *)sp)[3] = registers[REG_ER3];
  ((unsigned long *)sp)[4] = registers[REG_ER4];
  ((unsigned long *)sp)[5] = registers[REG_ER5];
  ((unsigned long *)sp)[6] = registers[REG_ER6];
  /* ((unsigned long *)sp)[7] = sp; */

  ((unsigned long *)sp)[7]  = registers[REG_CCR] << 24;
  ((unsigned long *)sp)[7] |= registers[REG_PC ];
}

static void break_intr(softvec_type_t type, unsigned long sp)
{
  int send_enable;
  int recv_enable;

  send_enable = serial_intr_is_send_enable(SERIAL_DEFAULT_DEVICE);
  recv_enable = serial_intr_is_recv_enable(SERIAL_DEFAULT_DEVICE);
  serial_intr_send_disable(SERIAL_DEFAULT_DEVICE);
  serial_intr_recv_disable(SERIAL_DEFAULT_DEVICE);

  store_registers(sp);
  handle_exception();
  restore_registers(sp);

  if (send_enable) serial_intr_send_enable(SERIAL_DEFAULT_DEVICE);
  if (recv_enable) serial_intr_recv_enable(SERIAL_DEFAULT_DEVICE);
}

void set_debug_traps()
{
  /* set interrupt vector for handling breakpoint. */
  softvec_setintr(SOFTVEC_TYPE_BKPOINT, break_intr);
}

void force_break()
{
  asm volatile ("trapa #3");
}
