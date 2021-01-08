#include "sci_register.h"

// #define H8_3069F_SCI_SMR_CKS_PER1   (0<<0)
// #define H8_3069F_SCI_SMR_CKS_PER4   (1<<0)
// #define H8_3069F_SCI_SMR_CKS_PER16  (2<<0)
// #define H8_3069F_SCI_SMR_CKS_PER64  (3<<0)
// #define H8_3069F_SCI_SMR_MP     (1<<2)
// #define H8_3069F_SCI_SMR_STOP   (1<<3)
// #define H8_3069F_SCI_SMR_OE     (1<<4)
// #define H8_3069F_SCI_SMR_PE     (1<<5)
// #define H8_3069F_SCI_SMR_CHR    (1<<6)
// #define H8_3069F_SCI_SMR_CA     (1<<7)

// #define H8_3069F_SCI_SCR_CKE0   (1<<0)
// #define H8_3069F_SCI_SCR_CKE1   (1<<1)
// #define H8_3069F_SCI_SCR_TEIE   (1<<2)
// #define H8_3069F_SCI_SCR_MPIE   (1<<3)
// #define H8_3069F_SCI_SCR_RE     (1<<4)
// #define H8_3069F_SCI_SCR_TE     (1<<5)
// #define H8_3069F_SCI_SCR_RIE    (1<<6)
// #define H8_3069F_SCI_SCR_TIE    (1<<7)

// #define H8_3069F_SCI_SSR_MPBT   (1<<0)
// #define H8_3069F_SCI_SSR_MPB    (1<<1)
// #define H8_3069F_SCI_SSR_TEND   (1<<2)
// #define H8_3069F_SCI_SSR_PER    (1<<3)
// #define H8_3069F_SCI_SSR_FERERS (1<<4)
// #define H8_3069F_SCI_SSR_ORER   (1<<5)
// #define H8_3069F_SCI_SSR_RDRF   (1<<6)
// #define H8_3069F_SCI_SSR_TDRE   (1<<7)

bool SciRegister::get_scr_re()
{
    return memory.read_uint8(scr_address) & (1<<4);
}

bool SciRegister::get_scr_te()
{
    return memory.read_uint8(scr_address) & (1<<5);
}

void SciRegister::set_rdr(uint8_t data)
{
    memory.write_uint8(rdr_address, data);
}

uint8_t SciRegister::get_tdr()
{
    return memory.read_uint8(tdr_address);
}

bool SciRegister::get_ssr_rdrf()
{
    return memory.read_uint8(ssr_address) & (1<<6);
}

void SciRegister::set_ssr_rdrf(bool b) {
    uint8_t rdrf = get_ssr_rdrf();
    if (b) {
        memory.write_uint8(ssr_address, rdrf | (1<<6));
    } else {
        memory.write_uint8(ssr_address, rdrf & ~(1<<6));
    }
}

bool SciRegister::get_ssr_tdre()
{
    return memory.read_uint8(ssr_address) & (1<<7);
}

void SciRegister::set_ssr_tdre(bool b) {
    uint8_t tdre = get_ssr_tdre();
    if (b) {
        memory.write_uint8(ssr_address, tdre | (1<<7));
    } else {
        memory.write_uint8(ssr_address, tdre & ~(1<<7));
    }
}

SciRegister::SciRegister(uint8_t index, InnerMemory& memory)
    : smr_address(sci_base_address[index] + 0)
    , brr_address(sci_base_address[index] + 1)
    , scr_address(sci_base_address[index] + 2)
    , tdr_address(sci_base_address[index] + 3)
    , ssr_address(sci_base_address[index] + 4)
    , rdr_address(sci_base_address[index] + 5)
    , scmr_address(sci_base_address[index] + 6)
    , memory(memory)
{
    memory.write_uint8(smr_address, 0);
    memory.write_uint8(brr_address, 0);
    memory.write_uint8(scr_address, 0);
    memory.write_uint8(tdr_address, 0);
    memory.write_uint8(ssr_address, 0xff);
    memory.write_uint8(rdr_address, 0);
    memory.write_uint8(scmr_address, 0);
}
