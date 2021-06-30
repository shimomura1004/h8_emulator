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
    if (handler == btst::btst_register_direct_imm) return "btst::btst_register_direct_imm";

    if (handler == dec::dec_b) return "dec::dec_b";
    if (handler == dec::dec_w) return "dec::dec_w";
    if (handler == dec::dec_l) return "dec::dec_l";

    if (handler == exts::exts_w) return "exts::exts_w";
    if (handler == exts::exts_l) return "exts::exts_l";

    if (handler == extu::extu_w) return "extu::extu_w";
    if (handler == extu::extu_l) return "extu::extu_l";

    if (handler == neg::neg_w) return "neg::neg_w";

    if (handler == shar::shar_w) return "shar::shar_w";

    if (handler == shll::shll_b) return "shll::shll_b";
    if (handler == shll::shll_w) return "shll::shll_w";
    if (handler == shll::shll_l) return "shll::shll_l";
    
    if (handler == shlr::shlr_b) return "shlr::shlr_b";
    if (handler == shlr::shlr_w) return "shlr::shlr_w";
    if (handler == shlr::shlr_l) return "shlr::shlr_l";

    if (handler == sub::sub_b) return "sub::sub_b";
    if (handler == sub::sub_w) return "sub::sub_w";
    if (handler == sub::sub_l) return "sub::sub_l";
    if (handler == sub::sub_immediate_l) return "sub::sub_immediate_l";

    if (handler == subs::subs) return "subs::subs";

    fprintf(stderr, "Unknown instruction: 0x%p\n", handler);
    return nullptr;
}
