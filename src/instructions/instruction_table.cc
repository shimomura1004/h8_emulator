#include "instruction_table.h"
#include "add.h"
#include "adds.h"
#include "and.h"
#include "andc.h"
#include "bcc.h"
#include "bclr.h"
#include "bset.h"
#include "btst.h"
#include "cmp.h"
#include "dec.h"
#include "exts.h"
#include "extu.h"
#include "jsr.h"
#include "mulxs.h"
#include "neg.h"
#include "nop.h"
#include "orc.h"
#include "rte.h"
#include "rts.h"
#include "shar.h"
#include "shll.h"
#include "shlr.h"
#include "sleep.h"
#include "sub.h"
#include "subs.h"
#include "trapa.h"
#include "xor.h"

using namespace h8instructions;

// TODO: 削除する
const char* lookup_instruction_name(instruction_handler_t handler)
{
    if (handler == subs::subs) return "subs::subs";

    fprintf(stderr, "Unknown instruction: 0x%p\n", handler);
    return nullptr;
}
