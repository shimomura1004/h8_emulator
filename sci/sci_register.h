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
    std::recursive_mutex sci_mutex;

    std::mutex rdrf_mut;
    std::condition_variable rdrf_cv;
    std::mutex tdre_mut;
    std::condition_variable tdre_cv;

public:
    SCIRegister();

    uint8_t get(uint8_t register_index);
    void set(uint8_t register_index, uint8_t byte);
    bool get_bit(uint8_t register_index, uint8_t bit_index);
    void set_bit(uint8_t register_index, uint8_t bit_index, bool b);
    static bool get_bit_from(uint8_t value, uint8_t bit_index);

    // OS 側へのインタフェース
    uint8_t read(uint32_t register_index);
    void write(uint32_t register_index, uint8_t byte);

    void wait_rdrf();
    void wait_tdre();

    void dump(FILE* fp);

};

#endif
