#ifndef _SCI_REGISTER_INCLUDED_
#define _SCI_REGISTER_INCLUDED_

#include <mutex>
#include <condition_variable>

class SCIRegister {
public:
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

    static bool get(uint8_t byte, uint8_t bit_index);
    static uint8_t set(uint8_t byte, uint8_t bit_index, bool b);

private:
    uint8_t regs[SCIRegister::SCI::SIZE];

    std::mutex ssr_mutex;
    std::mutex tdr_mutex;

    std::mutex rdrf_mut;
    std::condition_variable rdrf_cv;
    std::mutex tdre_mut;
    std::condition_variable tdre_cv;

public:
    SCIRegister();

    uint8_t read(uint32_t index);
    void write(uint32_t index, uint8_t value);

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

    void wait_rdrf();

    bool get_ssr_tdre();
    void set_ssr_tdre(bool b);

    void wait_tdre();
    void wait_tdre_if_up();

    void dump(FILE* fp);

};

#endif
