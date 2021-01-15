#ifndef _SCI_REGISTER_INCLUDED_
#define _SCI_REGISTER_INCLUDED_

#include <mutex>
#include <condition_variable>
#include "inner_memory.h"

class SciRegister {
    const uint32_t sci_base_address[3] = { 0xffffb0, 0xffffb8, 0xffffc0 };
    uint32_t smr_address;
    uint32_t brr_address;
    uint32_t scr_address;
    uint32_t tdr_address;
    uint32_t ssr_address;
    uint32_t rdr_address;
    uint32_t scmr_address;

    InnerMemory& memory;

public:
    SciRegister(uint8_t index, InnerMemory& memory);
    uint8_t read();
    void write(uint8_t value);

    bool get_scr_re();
    bool get_scr_te();
    bool get_scr_rie();
    void set_scr_rie(bool b);
    bool get_scr_tie();
    void set_scr_tie(bool b);

    void set_rdr(uint8_t data);

    uint8_t get_tdr();

    bool get_ssr_rdrf();
    void set_ssr_rdrf(bool b);
    bool get_ssr_tdre();
    void set_ssr_tdre(bool b);

};




class SCIRegister {
    typedef enum {
        SMR = 0,
        BRR,
        SCR,
        TDR,
        SSR,
        RDR,
        SCMR,
        SIZE
    } SCI;

    typedef enum {
        CKE0 = 0,
        CKE1,
        TEIE,
        MPIE,
        RE,
        TE,
        RIE,
        TIE
    } SCI_SCR;

    typedef enum {
        MPBT = 0,
        MPB,
        TEND,
        PER,
        FERERS,
        ORER,
        RDRF,
        TDRE
    } SCI_SSR;

    uint8_t regs[SCIRegister::SCI::SIZE];
    std::mutex rdrf_mut;
    std::condition_variable rdrf_cv;
    std::mutex tdre_mut;
    std::condition_variable tdre_cv;

public:
    uint8_t read(uint32_t index) { return regs[index]; }
    void write(uint32_t index, uint8_t value) { regs[index] = value; }

    bool get_scr_re() { return regs[SCR] & (1 << SCI_SCR::RE); }
    bool get_scr_te() { return regs[SCR] & (1 << SCI_SCR::TE); }
    bool get_scr_rie() { return regs[SCR] & (1 << SCI_SCR::RIE); }
    void set_scr_rie(bool b) {
        if (b) {
            regs[SCR] |= (1 << SCI_SCR::RIE);
        } else {
            regs[SCR] &= ~(1 << SCI_SCR::RIE);
        }
    }
    bool get_scr_tie() { return regs[SCR] & (1 << SCI_SCR::TIE); }
    void set_scr_tie(bool b) {
        if (b) {
            regs[SCR] |= (1 << SCI_SCR::TIE);
        } else {
            regs[SCR] &= ~(1 << SCI_SCR::TIE);
        }
    }

    void set_rdr(uint8_t data) {regs[RDR] = data;}

    uint8_t get_tdr() { return regs[TDR]; }

    bool get_ssr_rdrf() { return regs[SSR] & (1 << SCI_SSR::RDRF); }
    void set_ssr_rdrf(bool b) {
        if (b) {
            regs[SSR] |= (1 << SCI_SSR::RDRF);
        } else {
            regs[SSR] &= ~(1 << SCI_SSR::RDRF);
            rdrf_cv.notify_all();
        }
    }
    void wait_rdrf() {
        std::unique_lock<std::mutex> lock(rdrf_mut);
        rdrf_cv.wait(lock);
    }
    bool get_ssr_tdre() { return regs[SSR] & (1 << SCI_SSR::TDRE); }
    void set_ssr_tdre(bool b) {
        if (b) {
            regs[SSR] |= (1 << SCI_SSR::TDRE);
        } else {
            regs[SSR] &= ~(1 << SCI_SSR::TDRE);
            tdre_cv.notify_all();
        }
    }
    void wait_tdre() {
        std::unique_lock<std::mutex> lock(tdre_mut);
        tdre_cv.wait(lock);
    }

};

#endif
