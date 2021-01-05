#ifndef _INTERRUPT_QUEUE_INCLUDED_
#define _INTERRUPT_QUEUE_INCLUDED_

#include <queue>

typedef unsigned char interrupt_t;

class InterruptQueue {
    std::queue<interrupt_t> queue;

public:
    bool hasInterrupt() { return queue.empty(); }
    interrupt_t pop() {
        interrupt_t type = queue.front();
        queue.pop();
        return type;
    } 

};

#endif
