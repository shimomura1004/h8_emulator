#include "rtl8019as_register.h"

// TODO: レジスタへのアクセスは排他制御する

uint8_t RTL8019ASRegister::getPage()
{
    return (this->CR & 0b11000000) >> 6;
}

uint8_t* RTL8019ASRegister::getRegister(uint8_t index, uint8_t page, bool read_mode)
{
    switch (page) {
    case 0x00:
        switch (index) {
        case 0x00: return &CR;
        case 0x01: return read_mode ? &CLDA0        : &PSTART;
        case 0x02: return read_mode ? &CLDA1        : &PSTOP;
        case 0x03: return &BNRY;
        case 0x04: return read_mode ? &TSR          : &TPSR;
        case 0x05: return read_mode ? &NCR          : &TBCR0;
        case 0x06: return read_mode ? &FIFO         : &TBCR1;
        case 0x07: return &ISR;
        case 0x08: return read_mode ? &CRDA0        : &RSAR0;
        case 0x09: return read_mode ? &CRDA1        : &RSAR1;
        case 0x0a: return read_mode ? &CHIP8019ID0  : &RBCR0;
        case 0x0b: return read_mode ? &CHIP8019ID1  : &RBCR1;
        case 0x0c: return read_mode ? &RSR          : &RCR;
        case 0x0d: return read_mode ? &CNTR0        : &TCR;
        case 0x0e: return read_mode ? &CNTR1        : &DCR;
        case 0x0f: return read_mode ? &CNTR2        : &IMR;
        }
    case 0x01:
        switch (index) {
        case 0x00: return &CR;
        case 0x01: return &PAR0;
        case 0x02: return &PAR1;
        case 0x03: return &PAR2;
        case 0x04: return &PAR3;
        case 0x05: return &PAR4;
        case 0x06: return &PAR5;
        case 0x07: return &CURR;
        case 0x08: return &MAR0;
        case 0x09: return &MAR1;
        case 0x0a: return &MAR2;
        case 0x0b: return &MAR3;
        case 0x0c: return &MAR4;
        case 0x0d: return &MAR5;
        case 0x0e: return &MAR6;
        case 0x0f: return &MAR7;
        }
    case 0x02:
        switch (index) {
        case 0x00: return &CR;
        case 0x01: return &PSTART;
        case 0x02: return &PSTOP;
        case 0x03: return nullptr;
        case 0x04: return &TPSR;
        case 0x05: return nullptr;
        case 0x06: return nullptr;
        case 0x07: return nullptr;
        case 0x08: return nullptr;
        case 0x09: return nullptr;
        case 0x0a: return nullptr;
        case 0x0b: return nullptr;
        case 0x0c: return &RCR;
        case 0x0d: return &TCR;
        case 0x0e: return &DCR;
        case 0x0f: return &IMR;
        }
    case 0x03:
        switch (index) {
        case 0x00: return &CR;
        case 0x01: return &CR9346CR;
        case 0x02: return &BPAGE;
        case 0x03: return read_mode ? &CONFIG0 : nullptr;
        case 0x04: return &CONFIG1;
        case 0x05: return &CONFIG2;
        case 0x06: return &CONFIG3;
        case 0x07: return read_mode ? nullptr : &TEST;
        case 0x08: return read_mode ? &CSNSAV : nullptr;
        case 0x09: return read_mode ? nullptr : &HLTCLK;
        case 0x0a: return nullptr;
        case 0x0b: return read_mode ? &INTR : nullptr;
        case 0x0c: return read_mode ? nullptr : &FMWP;
        case 0x0d: return read_mode ? &CONFIG4 : nullptr;
        case 0x0e: return read_mode ? nullptr : nullptr;
        case 0x0f: return nullptr;
        }
    }

    return nullptr;
}

// TODO: レジスタの初期値の設定
// テスト用に、仮で以下の値を設定している
// ISR(0xff): Remote DMA が常に完了した状態
RTL8019ASRegister::RTL8019ASRegister()
    : CR(0x04)
    , CLDA0(0x00)
    , CLDA1(0x00)
    , BNRY(0x46)
    , TSR(0x00)
    , NCR(0x00)
    , FIFO(0x00)
    , ISR(0xff)
    , CRDA0(0x00)
    , CRDA1(0x00)
    , CHIP8019ID0(0x00)
    , CHIP8019ID1(0x00)
    , RSR(0x00)
    , CNTR0(0x00)
    , CNTR1(0x00)
    , CNTR2(0x00)
    , PSTART(0x46)
    , PSTOP(0x80)
    , TPSR(0x40)
    , TBCR0(0x00)
    , TBCR1(0x00)
    , RSAR0(0x00)
    , RSAR1(0x00)
    , RBCR0(0x00)
    , RBCR1(0x00)
    , RCR(0x00)
    , TCR(0x00)
    , DCR(0x00)
    , IMR(0x00)
    , PAR0(0x00)
    , PAR1(0x00)
    , PAR2(0x00)
    , PAR3(0x00)
    , PAR4(0x00)
    , PAR5(0x00)
    , CURR(0x46)
    , MAR0(0x00)
    , MAR1(0x00)
    , MAR2(0x00)
    , MAR3(0x00)
    , MAR4(0x00)
    , MAR5(0x00)
    , MAR6(0x00)
    , MAR7(0x00)
    , CR9346CR(0x00)
    , BPAGE(0x00)
    , CONFIG0(0x00)
    , CONFIG1(0x00)
    , CONFIG2(0x00)
    , CONFIG3(0x00)
    , CSNSAV(0x00)
    , INTR(0x00)
    , CONFIG4(0x00)
    , TEST(0x00)
    , HLTCLK(0x00)
    , FMWP(0x00)
{
}

bool RTL8019ASRegister::get_CR_TXP()
{
    return CR & (1 << 2);
}

void RTL8019ASRegister::set_CR_TXP(bool b)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    if (b) {
        // TODO: マジックナンバーを消す
        this->CR |= 1 << 2;
    } else {
        this->CR &= ~(1 << 2);
    }
    this->rtl_cv.notify_all();
}

uint8_t RTL8019ASRegister::get_BNRY()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return this->BNRY;
}

void RTL8019ASRegister::set_BNRY(uint8_t BNRY)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->BNRY = BNRY;
}

uint16_t RTL8019ASRegister::get_RSAR()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return ((uint16_t)this->RSAR1 << 8) + this->RSAR0;
}

void RTL8019ASRegister::set_RSAR(uint16_t RSAR)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->RSAR0 = RSAR & 0xff;
    this->RSAR1 = RSAR >> 8;
}

uint8_t RTL8019ASRegister::get_PSTART()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return PSTART;
}

void RTL8019ASRegister::set_PSTART(uint8_t PSTART)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->PSTART = PSTART;
}

uint8_t RTL8019ASRegister::get_PSTOP()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return PSTOP;
}

void RTL8019ASRegister::set_PSTOP(uint8_t PSTOP)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->PSTOP = PSTOP;
}

uint8_t RTL8019ASRegister::get_TPSR()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return TPSR;
}

void RTL8019ASRegister::set_TPSR(uint8_t TPSR)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->TPSR = TPSR;
}


uint16_t RTL8019ASRegister::get_TBCR()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return ((uint16_t)this->TBCR1 << 8) + this->TBCR0;
}

void RTL8019ASRegister::set_TBCR(uint16_t TBCR)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->TBCR0 = TBCR & 0xff;
    this->TBCR1 = TBCR >> 8;
}

uint8_t RTL8019ASRegister::get_CURR()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return this->CURR;
}

void RTL8019ASRegister::set_CURR(uint8_t CURR)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->CURR = CURR;
}

uint8_t RTL8019ASRegister::get_IMR()
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    return this->IMR;
}

void RTL8019ASRegister::set_IMR(uint8_t IMR)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);
    this->IMR = IMR;
}

void RTL8019ASRegister::wait_txp_to_be(bool b)
{
    std::unique_lock<std::mutex> txp_lock(this->rtl_mutex);
    this->rtl_cv.wait(txp_lock, [&]{
        // TODO: マジックナンバーを消す
        // TXP が true/false になるのを待つ
        return b == (bool)(CR & (1 << 2));
    });
}

// H8 側からレジスタを読む
uint8_t RTL8019ASRegister::read8(uint32_t address)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);

    if (address > 0x0f) {
        fprintf(stderr, "Error: Invalid access to RTL8019AS register (0x%x)\n", address);
        return 0;
    }

    uint8_t page = getPage();
    // fprintf(stderr, "read from 0x%x, get 0x%x\n", address, *this->getRegister(address, page, true));

    if (page == 0 && address == 0x07) {
        // テスト用に、ISR は常に 0xff
        ISR = 0xff;
    }

    return *this->getRegister(address, page, true);
}

// H8 側からレジスタに書き込む
void RTL8019ASRegister::write8(uint32_t address, uint8_t value)
{
    std::lock_guard<std::mutex> lock(this->rtl_mutex);

    if (address > 0x0f) {
        fprintf(stderr, "Error: Invalid access to RTL8019AS register (0x%x)\n", address);
        return;
    }

    // fprintf(stderr, "write 0x%x to 0x%x\n", value, address);

    uint8_t page = getPage();
    *this->getRegister(address, page, false) = value;

    if (address == 0x00) {
        // CR への書き込みがあったら notify する
        this->rtl_cv.notify_all();
    }
}

void RTL8019ASRegister::dump(FILE* fp)
{
}
