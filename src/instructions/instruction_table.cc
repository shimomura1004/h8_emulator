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

// todo: 削除する
const char* lookup_instruction_name(instruction_handler_t handler)
{
    if (handler == adds::adds) return "adds::adds";

    if (handler == andl::and_immediate_b) return "andl::and_immediate_b";
    if (handler == andl::and_immediate_w) return "andl::and_immediate_w";

    if (handler == andc::andc) return "andc::andc";

    if (handler == bcc::bra_8) return "bcc::bra_8";
    if (handler == bcc::bra_16) return "bcc::bra_16";
    if (handler == bcc::bhi_16) return "bcc::bhi_16";
    if (handler == bcc::bls_8) return "bcc::bls_8";
    if (handler == bcc::bcc_8) return "bcc::bcc_8";
    if (handler == bcc::bcs_8) return "bcc::bcs_8";
    if (handler == bcc::bne_8) return "bcc::bne_8";
    if (handler == bcc::bne_16) return "bcc::bne_16";
    if (handler == bcc::beq_8) return "bcc::beq_8";
    if (handler == bcc::beq_16) return "bcc::beq_16";
    if (handler == bcc::bge_8) return "bcc::bge_8";
    if (handler == bcc::bge_16) return "bcc::bge_16";
    if (handler == bcc::blt_8) return "bcc::blt_8";
    if (handler == bcc::blt_16) return "bcc::blt_16";
    if (handler == bcc::bgt_8) return "bcc::bgt_8";
    if (handler == bcc::bgt_16) return "bcc::bgt_16";
    if (handler == bcc::ble_8) return "bcc::ble_8";

    if (handler == bclr::bclr_register_direct_imm) return "bclr::bclr_register_direct_imm";
    if (handler == bset::bset_register_direct_imm) return "bset::bset_register_direct_imm";
    if (handler == btst::btst_register_direct_imm) return "btst::btst_register_direct_imm";

    if (handler == cmp::cmp_immediate_b) return "cmp::cmp_immediate_b";
    if (handler == cmp::cmp_register_direct_b) return "cmp::cmp_register_direct_b";
    if (handler == cmp::cmp_immediate_w) return "cmp::cmp_immediate_w";
    if (handler == cmp::cmp_register_direct_w) return "cmp::cmp_register_direct_w";
    if (handler == cmp::cmp_immediate_l) return "cmp::cmp_immediate_l";
    if (handler == cmp::cmp_register_direct_l) return "cmp::cmp_register_direct_l";

    if (handler == dec::dec_b) return "dec::dec_b";
    if (handler == dec::dec_w) return "dec::dec_w";
    if (handler == dec::dec_l) return "dec::dec_l";

    if (handler == exts::exts_w) return "exts::exts_w";
    if (handler == exts::exts_l) return "exts::exts_l";

    if (handler == extu::extu_w) return "extu::extu_w";
    if (handler == extu::extu_l) return "extu::extu_l";

    if (handler == jsr::jsr_register_indirect) return "jsr::jsr_register_indirect";
    if (handler == jsr::jsr_absolute_address) return "jsr::jsr_absolute_address";

    if (handler == mulxs::mulxs_b) return "mulxs::mulxs_b";
    if (handler == mulxs::mulxs_w) return "mulxs::mulxs_w";

    if (handler == neg::neg_w) return "neg::neg_w";

    if (handler == nop::nop) return "nop::nop";

    if (handler == orc::orc) return "orc::orc";

    if (handler == rte::rte) return "rte::rte";

    if (handler == rts::rts) return "rts::rts";

    if (handler == shar::shar_w) return "shar::shar_w";

    if (handler == shll::shll_b) return "shll::shll_b";
    if (handler == shll::shll_w) return "shll::shll_w";
    if (handler == shll::shll_l) return "shll::shll_l";
    
    if (handler == shlr::shlr_b) return "shlr::shlr_b";
    if (handler == shlr::shlr_w) return "shlr::shlr_w";
    if (handler == shlr::shlr_l) return "shlr::shlr_l";

    if (handler == sleep::sleep) return "sleep::sleep";

    if (handler == sub::sub_b) return "sub::sub_b";
    if (handler == sub::sub_w) return "sub::sub_w";
    if (handler == sub::sub_l) return "sub::sub_l";
    if (handler == sub::sub_immediate_l) return "sub::sub_immediate_l";

    if (handler == subs::subs) return "subs::subs";

    if (handler == trapa::trapa) return "trapa::trapa";

    if (handler == xorl::xor_immediate_b) return "xorl::xor_immediate_b";
    if (handler == xorl::xor_register_direct_b) return "xorl::xor_register_direct_b";

    fprintf(stderr, "Unknown instruction: 0x%p\n", handler);
    return nullptr;
}
