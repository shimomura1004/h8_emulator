#include "sci_register.h"

// #define debug_log
#ifdef debug_log
#define log(...) std::printf(__VA_ARGS__)
#else
#define log(...)
#endif

uint8_t SCIRegister::get(uint8_t register_index)
{
    std::lock_guard<std::recursive_mutex> lock(sci_mutex);
    return regs[register_index];
}

void SCIRegister::set(uint8_t register_index, uint8_t byte)
{
    std::lock_guard<std::recursive_mutex> lock(sci_mutex);
    regs[register_index] = byte;
}

bool SCIRegister::get_bit(uint8_t register_index, uint8_t bit_index)
{
    std::lock_guard<std::recursive_mutex> lock(sci_mutex);
    return get(register_index) & (1 << bit_index);
}

void SCIRegister::set_bit(uint8_t register_index, uint8_t bit_index, bool b)
{
    std::lock_guard<std::recursive_mutex> lock(sci_mutex);
    uint8_t byte = get(register_index);
    byte = b ? (byte | (1 << bit_index)) : (byte & ~(1 << bit_index));
    set(register_index, byte);
}

bool SCIRegister::get_bit_from(uint8_t value, uint8_t bit_index)
{
    return value & (1 << bit_index);
}

SCIRegister::SCIRegister() {
    // H8 仕様書より
    regs[SCI::SMR] = 0x00;
    regs[SCI::BRR] = 0xff;
    regs[SCI::SCR] = 0x00;
    regs[SCI::TDR] = 0xff;
    regs[SCI::SSR] = 0x84;
    regs[SCI::RDR] = 0x00;
    regs[SCI::SCMR] = 0xf2;
}

uint8_t SCIRegister::read(uint32_t register_index)
{
    std::lock_guard<std::recursive_mutex> lock(sci_mutex);
    return regs[register_index];
}

// todo: CPU 側の関数を使って設定するほうがいい
void SCIRegister::write(uint32_t register_index, uint8_t byte)
{
    std::lock_guard<std::recursive_mutex> lock(sci_mutex);

    switch (register_index) {
    case SCI::SMR: {
        set(register_index, byte);
        break;
    }
    case SCI::BRR: {
        set(register_index, byte);
        break;
    }
    case SCI::SCR: {
        set(register_index, byte);
        break;
    }
    case SCI::TDR: {
        set(register_index, byte);
        break;
    }
    case SCI::SSR: {
        {
            std::lock_guard<std::mutex> lock(tdre_mut);
            bool prev_tdre = get_bit(register_index, SCI_SSR::TDRE);
            // SCR::TE ビットが 0 のときは SSR::TDRE は 1 に固定される
            // SSR::TDRE が 1 のときは、有効なデータがないことを示す
            bool next_tdre = get_bit_from(byte, SCI_SSR::TDRE);
            set_bit(register_index, SCI_SSR::TDRE, next_tdre);
            if (prev_tdre && !next_tdre) {
                tdre_cv.notify_all();
            }
        }
        {
            std::lock_guard<std::mutex> lock(rdrf_mut);
            bool prev_rdrf = get_bit(register_index, SCI_SSR::RDRF);
            bool next_rdrf = get_bit_from(byte, SCI_SSR::RDRF);
            set_bit(register_index, SCI_SSR::RDRF, next_rdrf);
            if (prev_rdrf && !next_rdrf) {
                rdrf_cv.notify_all();
            }
        }

        // todo: 他のビットも反映する
        // set(register_index, byte);

        break;
    }
    case SCI::RDR: {
        set(register_index, byte);
        break;
    }
    case SCI::SCMR: {
        set(register_index, byte);
        break;
    }
    default:
        fprintf(stderr, "Unknown SCI register (%d)\n", register_index);
        break;
    }
}

void SCIRegister::wait_rdrf()
{
    // ビジーループなら問題なし
    while (get_bit(SSR, SCI_SSR::RDRF))
        ;
    return;
    
    // std::unique_lock<std::mutex> rdrf_lock(rdrf_mut);
    // // printf(" wait RDRF\n");
    // rdrf_cv.wait(rdrf_lock, [this]{
    //     return !(regs[SSR] & (1 << SCI_SSR::RDRF));
    // });
    // // printf(" restart RDRF\n");
}

void SCIRegister::wait_tdre()
{
    // ビジーループなら問題なし
    while (get_bit(SSR, SCI_SSR::TDRE))
        ;
    return;

    // std::unique_lock<std::mutex> tdre_lock(tdre_mut);
    // tdre_cv.wait(tdre_lock, [this]{
    //     return !(regs[SSR] & (1 << SCI_SSR::TDRE));
    // });
}

void SCIRegister::dump(FILE* fp)
{
    for (int i = 0; i < SCIRegister::SCI::SIZE; i++) {
        fputc(regs[i], fp);
    }
    fputc(0, fp);
}
