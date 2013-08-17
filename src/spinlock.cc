#include "spinlock.h"

namespace SLFree {

    using namespace std;

    Spinlock::Spinlock() :
        flag { ATOMIC_FLAG_INIT } 
    {
    }

    void Spinlock::lock() {
        while (flag.test_and_set(memory_order_acquire)) { }
    }

    void Spinlock::unlock() {
        flag.clear(memory_order_release);
    }

    bool Spinlock::try_lock() {
        return flag.test_and_set(memory_order_acquire);
    }
}
