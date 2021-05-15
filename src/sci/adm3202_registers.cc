#include "adm3202_registers.h"

bool ADM3202_Registers::get_bit_from_nolock(uint8_t value, uint8_t bit_index)
{
    return value & (1 << bit_index);
}

uint8_t ADM3202_Registers::get_nolock(uint8_t register_index)
{
    return regs[register_index];
}

void ADM3202_Registers::set_nolock(uint8_t register_index, uint8_t byte)
{
    regs[register_index] = byte;
}

bool ADM3202_Registers::get_bit_nolock(uint8_t register_index, uint8_t bit_index)
{
    return regs[register_index] & (1 << bit_index);
}

void ADM3202_Registers::set_bit_nolock(uint8_t register_index, uint8_t bit_index, bool b)
{
    uint8_t byte = regs[register_index];
    byte = b ? (byte | (1 << bit_index)) : (byte & ~(1 << bit_index));
    regs[register_index] = byte;
}

uint8_t ADM3202_Registers::get(uint8_t register_index)
{
    std::lock_guard<std::mutex> lock(sci_mutex);
    return get_nolock(register_index);
}

void ADM3202_Registers::set(uint8_t register_index, uint8_t byte)
{
    std::lock_guard<std::mutex> lock(sci_mutex);
    set_nolock(register_index, byte);
}

bool ADM3202_Registers::get_bit(uint8_t register_index, uint8_t bit_index)
{
    std::lock_guard<std::mutex> lock(sci_mutex);
    return get_bit_nolock(register_index, bit_index);
}

void ADM3202_Registers::set_bit(uint8_t register_index, uint8_t bit_index, bool b)
{
    std::lock_guard<std::mutex> lock(sci_mutex);
    set_bit_nolock(register_index, bit_index, b);
}

ADM3202_Registers::ADM3202_Registers() {
    // H8 仕様書より
    regs[SCI::SMR] = 0x00;
    regs[SCI::BRR] = 0xff;
    regs[SCI::SCR] = 0x00;
    regs[SCI::TDR] = 0xff;
    regs[SCI::SSR] = 0x84;
    regs[SCI::RDR] = 0x00;
    regs[SCI::SCMR] = 0xf2;
}

uint8_t ADM3202_Registers::read(uint32_t register_index)
{
    std::lock_guard<std::mutex> lock(sci_mutex);
    return regs[register_index];
}

// TODO: CPU 側の関数を使って設定するほうがいい
void ADM3202_Registers::write(uint32_t register_index, uint8_t byte)
{
    std::lock_guard<std::mutex> lock(sci_mutex);

    switch (register_index) {
    case SCI::SMR: {
        set_nolock(register_index, byte);
        break;
    }
    case SCI::BRR: {
        set_nolock(register_index, byte);
        break;
    }
    case SCI::SCR: {
        set_nolock(register_index, byte);
        // TODO: enable になったとき、既に割り込みが発生していた場合は？
        break;
    }
    case SCI::TDR: {
        set_nolock(register_index, byte);
        break;
    }
    case SCI::SSR: {
        // SCR::TE ビットが 0 のときは SSR::TDRE は 1 に固定される
        // SSR::TDRE が 1 のときは、有効なデータがないことを示す
        bool prev_tdre = get_bit_nolock(register_index, SCI_SSR::TDRE);
        bool next_tdre = get_bit_from_nolock(byte, SCI_SSR::TDRE);
        set_bit_nolock(register_index, SCI_SSR::TDRE, next_tdre);

        if (prev_tdre != next_tdre) {
            // wait のループで RX/TX のどちらが解除されたかを見ているので、CV を分ける必要はない
            sci_cv.notify_all();
        }

        bool prev_rdrf = get_bit_nolock(register_index, SCI_SSR::RDRF);
        bool next_rdrf = get_bit_from_nolock(byte, SCI_SSR::RDRF);
        set_bit_nolock(register_index, SCI_SSR::RDRF, next_rdrf);

        if (prev_rdrf != next_rdrf) {
            sci_cv.notify_all();
        }

        // TODO: 他のビットも反映する
        break;
    }
    case SCI::RDR: {
        set_nolock(register_index, byte);
        break;
    }
    case SCI::SCMR: {
        set_nolock(register_index, byte);
        break;
    }
    default:
        fprintf(stderr, "Unknown SCI register (%d)\n", register_index);
        break;
    }
}

void ADM3202_Registers::wait_rdrf_to_be(bool b)
{   
    std::unique_lock<std::mutex> rdrf_lock(sci_mutex);
    sci_cv.wait(rdrf_lock, [&]{
        return b == (regs[SCI::SSR] & (1 << SCI_SSR::RDRF));
    });
}

void ADM3202_Registers::wait_tdre_to_be(bool b)
{
    std::unique_lock<std::mutex> tdre_lock(sci_mutex);
    sci_cv.wait(tdre_lock, [&]{
        return b == (regs[SCI::SSR] & (1 << SCI_SSR::TDRE));
    });
}

void ADM3202_Registers::dump(FILE* fp)
{
    for (int i = 0; i < ADM3202_Registers::SCI::SIZE; i++) {
        fputc(regs[i], fp);
    }
    fputc(0, fp);
}
