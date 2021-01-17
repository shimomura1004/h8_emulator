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

// #define debug_log
#ifdef debug_log
#define log(...) std::printf(__VA_ARGS__)
#else
#define log(...)
#endif

bool SCIRegister::get(uint8_t byte, uint8_t bit_index)
{
    return byte & (1 << bit_index);
}

uint8_t SCIRegister::set(uint8_t byte, uint8_t bit_index, bool b)
{
    return b ? (byte | (1 << bit_index)) : (byte & ~(1 << bit_index));
}

SCIRegister::SCIRegister() {
    // H8 仕様書より
    regs[SSR] = 0x84;
}

uint8_t SCIRegister::read(uint32_t index)
{
    return regs[index];
}

// void SCIRegister::write(uint32_t index, uint8_t value)
// {
//     std::lock_guard<std::mutex> lock(rdrf_mut);
//     uint8_t prev_value = regs[index];
//     regs[index] = value;

//     switch (index) {
//     case SCI::SSR: {
//         uint8_t prev_tdre = (prev_value & (1 << SCI_SSR::TDRE));
//         uint8_t new_tdre = (value & (1 << SCI_SSR::TDRE));
//         if (prev_tdre && !new_tdre) {
//             // TDRE が 1->0 と変化したら通知
// // printf(" NOTIFY by write (%d)\n", std::this_thread::get_id());
//             tdre_cv.notify_all();
//         }

//         uint8_t prev_rdrf = (prev_value & (1 << SCI_SSR::RDRF));
//         uint8_t new_rdrf = (value & (1 << SCI_SSR::RDRF));
//         if (prev_rdrf && !new_rdrf) {
//             rdrf_cv.notify_all();
//             // printf(" notify RDRF by write\n");
//             // printf("[0x%x->0x%x]\n", prev_value, regs[index]);
//         }
//         break;
//     }
//     default:
//         break;
//     }
// }

// todo: CPU 側の関数を使って設定するほうがいい
void SCIRegister::write(uint32_t index, uint8_t value)
{
    switch (index) {
    case SCI::TDR: {
        std::lock_guard<std::mutex> lock(tdr_mutex);
        regs[index] = value;
        break;
    }
    case SCI::SSR: {
        {
            std::lock_guard<std::mutex> lock(tdre_mut);
            uint8_t hoge = regs[index];
            bool prev_tdre = SCIRegister::get(regs[index], SCI_SSR::TDRE);
            bool new_tdre = SCIRegister::get(value, SCI_SSR::TDRE);
            regs[index] = SCIRegister::set(regs[index], SCI_SSR::TDRE, new_tdre);
            if (prev_tdre && !new_tdre) {
                // TDRE が 1->0 と変化したら通知
                tdre_cv.notify_all();
log(" NOTIFY BY write (%x -> %x (%x -> %x))\n", prev_tdre, new_tdre, hoge, regs[index]);
            }
        }

        {
            std::lock_guard<std::mutex> lock(rdrf_mut);
            {
                // std::lock_guard<std::mutex> ssr_lock(ssr_mutex);
                uint8_t prev_rdrf = SCIRegister::get(regs[index], SCI_SSR::RDRF);
                uint8_t new_rdrf = SCIRegister::get(value, SCI_SSR::RDRF);
                regs[index] = SCIRegister::set(regs[index], SCI_SSR::RDRF, new_rdrf);
                if (prev_rdrf && !new_rdrf) {
                    rdrf_cv.notify_all();
                }
            }
        }
        break;
    }
    default:
        // regs[index] = value;
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
    std::lock_guard<std::mutex> lock(tdr_mutex);
    return regs[TDR];
}

bool SCIRegister::get_ssr_rdrf()
{
    std::lock_guard<std::mutex> lock(rdrf_mut);
    // printf("set rdrf!\n");
    return regs[SSR] & (1 << SCI_SSR::RDRF);
}

void SCIRegister::set_ssr_rdrf(bool b)
{
    std::lock_guard<std::mutex> lock(rdrf_mut);
    if (b) {
        regs[SSR] |= (1 << SCI_SSR::RDRF);
    } else {
        regs[SSR] &= ~(1 << SCI_SSR::RDRF);
        // printf(" notify RDRF by set\n");
        rdrf_cv.notify_all();
    }
}

void SCIRegister::wait_rdrf()
{
    // // ビジーループなら問題なし
    // while (regs[SSR] & (1 << SCI_SSR::RDRF)) {}
    // return;
    
    std::unique_lock<std::mutex> rdrf_lock(rdrf_mut);
    // printf(" wait RDRF\n");
    rdrf_cv.wait(rdrf_lock, [this]{
        return !(regs[SSR] & (1 << SCI_SSR::RDRF));
    });
    // printf(" restart RDRF\n");
}

bool SCIRegister::get_ssr_tdre()
{
log("before tdre lock\n");
    std::lock_guard<std::mutex> lock(tdre_mut);
    return regs[SSR] & (1 << SCI_SSR::TDRE);
}

void SCIRegister::set_ssr_tdre(bool b)
{
log("before tdre lock (set ssr)\n");
    std::lock_guard<std::mutex> lock(tdre_mut);
    if (b) {
        regs[SSR] |= (1 << SCI_SSR::TDRE);
    } else {
        regs[SSR] &= ~(1 << SCI_SSR::TDRE);
log(" NOTIFY BY set\n");
        tdre_cv.notify_all();
    }
log("end tdre lock (set ssr)\n");
}

void SCIRegister::wait_tdre()
{
    log("before tdre lock\n");
    std::unique_lock<std::mutex> tdre_lock(tdre_mut);
    log("SLEEP\n");
    tdre_cv.wait(tdre_lock, [this]{
        return !(regs[SSR] & (1 << SCI_SSR::TDRE));
    });

    log("WAKEUP!\n");
}

void SCIRegister::wait_tdre_if_up()
{
    std::unique_lock<std::mutex> tdre_lock(tdre_mut);
    tdre_cv.wait(tdre_lock, [this]{
        return !(regs[SSR] & (1 << SCI_SSR::TDRE));
    });
}

void SCIRegister::dump(FILE* fp)
{
    for (int i = 0; i < SCIRegister::SCI::SIZE; i++) {
        fputc(regs[i], fp);
    }
    fputc(0, fp);
}
