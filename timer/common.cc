#include "./common.h"

// ミリ秒単位
const double periods[] = {
    1.0 / 20000000 * 1000,         // 0.00000005秒(20MHz)
    1.0 / 20000000 * 1000 * 8,     // 0.0000004秒
    1.0 / 20000000 * 1000 * 64,    // 0.0000032秒
    1.0 / 20000000 * 1000 * 8192   // 0.0004096秒
};
