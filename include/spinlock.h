#include <atomic>

/**
 * spinlock.h
 *
 * A simple spinning lock using std::atomic_flag
 */

namespace SLFree {

    class Spinlock {
    public:
        Spinlock();

        void lock();
        void unlock();

        bool try_lock();

    private:
        std::atomic_flag flag;
    };
}

