#ifndef _INTERRUPT_TYPE_INCLUDED_
#define _INTERRUPT_TYPE_INCLUDED_

typedef enum {
    NONE = -1,
    RESET = 0,
    NMI = 7,
    // トラップ命令
    TRAP0 = 8,
    TRAP1,
    TRAP2,
    TRAP3,
    // 外部割込み
    IRQ0 = 12,
    IRQ1,
    IRQ2,
    IRQ3,
    IRQ4,
    IRQ5,
    // タイマ
    CMIA0 = 36,   // コンペアマッチA0
    CMIB0,        // コンペアマッチB0
    CMIA1_CMIB1,  // コンペアマッチA1/B1
    TOVI0_TOVI1,  // オーバーフロー0/1
    CMIA2,        // コンペアマッチA2
    CMIB2,        // コンペアマッチB2
    CMIA3_CMIB3,  // コンペアマッチA3/B3
    TOVI2_TOVI3,  // オーバーフロー2/3
    // SCI0
    ERI0 = 52,
    RXI0,
    TXI0,
    TEI0,
    // SCI1
    ERI1 = 56,
    RXI1,
    TXI1,
    TEI1,
    // SCI2
    ERI2 = 60,
    RXI2,
    TXI2,
    TEI2,
} interrupt_t;

#endif
