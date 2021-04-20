#ifndef _SCI_REGISTER_INCLUDED_
#define _SCI_REGISTER_INCLUDED_

#include <mutex>
#include <condition_variable>

class SCIRegister {
public:
    // todo: enum は別ヘッダに移す
    typedef enum {
        SMR = 0,    // シリアルモードレジスタ
        BRR,        // ビットレートレジスタ
        SCR,        // シリアルコントロールレジスタ
        TDR,        // トランスミットレジスタ
        SSR,        // シリアルステータスレジスタ
        RDR,        // レシーブデータレジスタ
        SCMR,       // スマートカードモードレジスタ
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

private:
    uint8_t regs[SCIRegister::SCI::SIZE];
    // エミュレータと OS の両方から SCI のレジスタにアクセスするので、排他が必要
    std::mutex sci_mutex;
    std::condition_variable sci_cv;

    static bool get_bit_from_nolock(uint8_t value, uint8_t bit_index);

    uint8_t get_nolock(uint8_t register_index);
    void set_nolock(uint8_t register_index, uint8_t byte);
    bool get_bit_nolock(uint8_t register_index, uint8_t bit_index);
    void set_bit_nolock(uint8_t register_index, uint8_t bit_index, bool b);

public:
    SCIRegister();

    uint8_t get(uint8_t register_index);
    void set(uint8_t register_index, uint8_t byte);
    bool get_bit(uint8_t register_index, uint8_t bit_index);
    void set_bit(uint8_t register_index, uint8_t bit_index, bool b);

    // OS 側へのインタフェース
    uint8_t read(uint32_t register_index);
    void write(uint32_t register_index, uint8_t byte);

    // rdrf/tdre が b になるのを待つ
    void wait_rdrf_to_be(bool b);
    void wait_tdre_to_be(bool b);

    void dump(FILE* fp);

};

#endif
