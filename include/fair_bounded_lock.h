/* fairboundedlock.h
   Mathieu Stefani, 21 August 2014
   
   A Fair lock that uses a bounded queue to order threads
   The "Fairness" is a property of the lock meaning that each thread
   will wait for its turn in order when the lock is under contention. This means
   that no random thread will "steal" the lock while some other threads have
   been waiting for longer.

    TODO: avoid cacheline sharing by making sure that each boolean from the ring buffer
    ends up in its own cacheline
*/


#pragma once

#include <atomic>
#include <array>

#include "internal/details.h"

namespace SLFree {

template<size_t Size>
class FairBoundedLock {
public:
    struct Guard {
        Guard(FairBoundedLock<Size> &lock) : lock(lock) {
            slot = lock.lock();
        }

        ~Guard() {
            lock.unlock(slot);
        }
    private:
        FairBoundedLock<Size> &lock;
        size_t slot;
    };

    FairBoundedLock() {
        for (auto& entry: ring) {
            entry.store(false, std::memory_order_relaxed);
        }

        ring[0].store(true, std::memory_order_relaxed);
        current.store(0, std::memory_order_relaxed);
    }

    size_t lock() {
        size_t slot = current.fetch_add(1) % Size;
        while (!ring[slot]) { }
        return slot;
    }

    void unlock(size_t slot) {
        ring[slot] = false;
        ring[(slot + 1) % Size] = true;
    }

private:
    std::array<std::atomic<bool>, Size> ring;
    cacheline_pad_a padding_;
    std::atomic<size_t> current;
};

} // namespace SLFree
