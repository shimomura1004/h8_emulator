#include "rte.h"

int h8instructions::rte::rte(H8300H *h8)
{
    h8->restore_pc_and_ccr_from_stack();

    // todo: 外部割り込みの場合は勝手にクリアしてはいけない
    // 割り込みから戻ったら割り込み要因をクリアする
    interrupt_t type = h8->interrupt_controller.getInterruptType();
    h8->interrupt_controller.clear(type);

    return 0;
}
