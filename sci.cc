#include <thread>
#include <chrono>

#include "sci.h"
#include "sci_register.h"

void sci::start(uint8_t index, InnerMemory& memory, bool& terminate)
{
    printf("SCI(%d) started\n", index);
    SciRegister sci(index, memory);

    while (!terminate) {
        // 少し動作を遅くする
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // 送信要求がきていたら処理
        sci.process_send_request();
    }
}
