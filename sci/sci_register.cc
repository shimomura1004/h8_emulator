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

SCIRegister::SCIRegister() {
    regs[SSR] = 0x84;
}

uint8_t SCIRegister::read(uint32_t index)
{
    return regs[index];
}

void SCIRegister::write(uint32_t index, uint8_t value)
{

    uint8_t prev_value = regs[index];
    regs[index] = value;

    switch (index) {
    case SCI::SSR: {
        uint8_t prev_tdre = (prev_value & (1 << SCI_SSR::TDRE));
        uint8_t new_tdre = (value & (1 << SCI_SSR::TDRE));
        if (prev_tdre && !new_tdre) {
            // TDRE が 1->0 と変化したら通知
// printf(" NOTIFY by write (%d)\n", std::this_thread::get_id());
            tdre_cv.notify_all();
        }
        break;
    }
    default:
        break;
    }
}

bool SCIRegister::get_scr_re()
{
    return regs[SCR] & (1 << SCI_SCR::RE);
}

bool SCIRegister::get_scr_te()
{
    return regs[SCR] & (1 << SCI_SCR::TE);
}

bool SCIRegister::get_scr_rie()
{
    return regs[SCR] & (1 << SCI_SCR::RIE);
}

void SCIRegister::set_scr_rie(bool b)
{
    if (b) {
        regs[SCR] |= (1 << SCI_SCR::RIE);
    } else {
        regs[SCR] &= ~(1 << SCI_SCR::RIE);
    }
}

bool SCIRegister::get_scr_tie()
{
    return regs[SCR] & (1 << SCI_SCR::TIE);
}

void SCIRegister::set_scr_tie(bool b)
{
    if (b) {
        regs[SCR] |= (1 << SCI_SCR::TIE);
    } else {
        regs[SCR] &= ~(1 << SCI_SCR::TIE);
    }
}

void SCIRegister::set_rdr(uint8_t data)
{
    regs[RDR] = data;
}

uint8_t SCIRegister::get_tdr() {
    return regs[TDR];
}

bool SCIRegister::get_ssr_rdrf()
{
    return regs[SSR] & (1 << SCI_SSR::RDRF);
}

void SCIRegister::set_ssr_rdrf(bool b)
{
    if (b) {
        regs[SSR] |= (1 << SCI_SSR::RDRF);
    } else {
        regs[SSR] &= ~(1 << SCI_SSR::RDRF);
        rdrf_cv.notify_all();
    }
}

void SCIRegister::wait_rdrf()
{
    // ビジーループなら問題なし
    while (regs[SSR] & (1 << SCI_SSR::RDRF)) {}
    return;

    std::unique_lock<std::mutex> rdrf_lock(rdrf_mut);
    rdrf_cv.wait(rdrf_lock);
}

bool SCIRegister::get_ssr_tdre()
{
    return regs[SSR] & (1 << SCI_SSR::TDRE);
}

void SCIRegister::set_ssr_tdre(bool b)
{
    if (b) {
        regs[SSR] |= (1 << SCI_SSR::TDRE);
    } else {
        regs[SSR] &= ~(1 << SCI_SSR::TDRE);
// printf(" NOTIFY BY set (%d)\n", std::this_thread::get_id());
        tdre_cv.notify_all();
    }
}

void SCIRegister::wait_tdre()
{
    std::unique_lock<std::mutex> tdre_lock(tdre_mut);
    tdre_cv.wait(tdre_lock);
}

void SCIRegister::wait_tdre_if_up()
{
    while(regs[SSR] & (1 << SCI_SSR::TDRE)){
    }
    return;

    if (regs[SSR] & (1 << SCI_SSR::TDRE)) {
        std::unique_lock<std::mutex> lock(tdre_mut);
        tdre_cv.wait(lock);
    }
}

void SCIRegister::dump(FILE* fp)
{
    for (int i = 0; i < SCIRegister::SCI::SIZE; i++) {
        fputc(regs[i], fp);
    }
    fputc(0, fp);
}
