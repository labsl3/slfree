#ifndef SLFREE_BOUNDED_QUEUE_SPSC_H
#define SLFREE_BOUNDED_QUEUE_SPSC_H

#include <atomic>
#include <cstddef>

static constexpr size_t CachelineSize = 64;

namespace SLFree {

    template<typename T, size_t Size = 128>
        class BoundedQueueSPSC {
            public:
                enum { Size_ = Size + 1 };

                BoundedQueueSPSC() :
                    headIndex { ATOMIC_VAR_INIT(0) },
                    tailIndex { ATOMIC_VAR_INIT(0) }
                {
                }

                bool enqueue(const T &value) 
                {
                    auto currentTail = tailIndex.load(std::memory_order_acquire);
                    auto nextTail = (currentTail + 1) % Size_;
                    if (nextTail == headIndex.load(std::memory_order_acquire)) 
                        return false;

                    buffer_[currentTail] = value;
                    tailIndex.store(nextTail, std::memory_order_release);
                    return true;    
                }

                bool dequeue(T &result) 
                {
                    auto currentHead = headIndex.load(std::memory_order_acquire);
                    if (currentHead == tailIndex.load(std::memory_order_acquire))
                        return false;

                    result = buffer_[currentHead];
                    auto nextHead = (currentHead + 1) % Size_;
                    headIndex.store(nextHead);
                    return true;
                }

                bool isFull() 
                {
                    auto currentTail = tailIndex.load(std::memory_order_acquire);
                    auto currentHead = headIndex.load(std::memory_order_acquire);
                    return ((currentTail + 1) % Size_) == currentHead;
                }

                bool isEmpty()
                {
                    auto currentTail = tailIndex.load(std::memory_order_acquire);
                    auto currentHead = headIndex.load(std::memory_order_acquire);

                    return currentTail == currentHead;
                }
            private:
                std::atomic<size_t> headIndex;

                char pad1_[CachelineSize - sizeof (headIndex)];

                std::atomic<size_t> tailIndex;

                char pad2_[CachelineSize - sizeof (tailIndex)];

                T buffer_[Size_]; 


        };
}


#endif
