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
    if (handler == dec::dec_b) return "dec::dec_b";
    if (handler == dec::dec_w) return "dec::dec_w";
    if (handler == dec::dec_l) return "dec::dec_l";

    if (handler == exts::exts_w) return "exts::exts_w";
    if (handler == exts::exts_l) return "exts::exts_l";

    if (handler == extu::extu_w) return "extu::extu_w";
    if (handler == extu::extu_l) return "extu::extu_l";

    if (handler == neg::neg_w) return "neg::neg_w";

    if (handler == shar::shar_w) return "shar::shar_w";
    
    if (handler == subs::subs) return "subs::subs";

    fprintf(stderr, "Unknown instruction: 0x%p\n", handler);
    return nullptr;
}
