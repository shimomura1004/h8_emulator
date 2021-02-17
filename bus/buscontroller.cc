#include "buscontroller.h"

const uint32_t BusController::abwcr_address  = 0xfee020;
const uint32_t BusController::astcr_address  = 0xfee021;
const uint32_t BusController::wcrh_address   = 0xfee022;
const uint32_t BusController::wcrl_address   = 0xfee023;
const uint32_t BusController::brcr_address   = 0xfee013;
const uint32_t BusController::cscr_address   = 0xfee01f;
const uint32_t BusController::adrcr_address  = 0xfee01e;
const uint32_t BusController::bcr_address    = 0xfee024;
const uint32_t BusController::drcra_address  = 0xfee026;
const uint32_t BusController::drcrb_address  = 0xfee027;
const uint32_t BusController::rtmcsr_address = 0xfee028;
const uint32_t BusController::rtcnt_address  = 0xfee029;
const uint32_t BusController::rtcor_address  = 0xfee02a;

BusController::BusController()
    : abwcr(0xff)
    , astcr(0xff)
    , wcrh(0xff)
    , wcrl(0xff)
    , brcr(0xfe)
    , cscr(0x0f)
    , adrcr(0xff)
    , bcr(0xc6)
    , drcra(0x10)
    , drcrb(0x08)
    , rtmcsr(0x07)
    , rtcnt(0x00)
    , rtcor(0xff)
{}

uint8_t BusController::read(uint32_t address)
{
    switch (address) {
    case abwcr_address:  return this->abwcr;
    case astcr_address:  return this->astcr;
    case wcrh_address:   return this->wcrh;
    case wcrl_address:   return this->wcrl;
    case brcr_address:   return this->brcr;
    case cscr_address:   return this->cscr;
    case adrcr_address:  return this->adrcr;
    case bcr_address:    return this->bcr;
    case drcra_address:  return this->drcra;
    case drcrb_address:  return this->drcrb;
    case rtmcsr_address: return this->rtmcsr;
    case rtcnt_address:  return this->rtcnt;
    case rtcor_address:  return this->rtcor;
    default:
        fprintf(stderr, "Error: Invalid read to bus controller (0x%06x)\n", address);
        return 0;
    }
}

void BusController::write(uint32_t address, uint8_t value)
{
    switch (address) {
    case abwcr_address:  this->abwcr = value;  break;
    case astcr_address:  this->astcr = value;  break;
    case wcrh_address:   this->wcrh = value;   break;
    case wcrl_address:   this->wcrl = value;   break;
    case brcr_address:   this->brcr = value;   break;
    case cscr_address:   this->cscr = value;   break;
    case adrcr_address:  this->adrcr = value;  break;
    case bcr_address:    this->bcr = value;    break;
    case drcra_address:  this->drcra = value;  break;
    case drcrb_address:  this->drcrb = value;  break;
    case rtmcsr_address: this->rtmcsr = value; break;
    case rtcnt_address:  this->rtcnt = value;  break;
    case rtcor_address:  this->rtcor = value;  break;
    default:
        fprintf(stderr, "Error: Invalid write to bus controller (0x%06x)\n", address);
        break;
    }
}
