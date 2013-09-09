#include "bounded_queue_spsc.h"
#include <random>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

namespace {
    mt19937 rng;
}

template<typename T> struct RandomValue {
};

template<> struct RandomValue<uint32_t> {
    static uint32_t generate() {
        uniform_int_distribution<uint32_t> dist;
        return dist(rng);
    }
};

template<typename T, size_t Size>
struct Perf {
    Perf() 
    {
    }

    void run() {
        enum { 
            TotalOps = 1000 * 1000 * 100 
        };

        SLFree::BoundedQueueSPSC<T, Size> queue;

        volatile uint32_t totalEnqueueFail { 0 };
        volatile uint32_t totalDequeueFail { 0 };

        auto producer = [&]() {
            for (size_t i = 0; i < TotalOps; ++i) {
                bool result = queue.enqueue(RandomValue<T>::generate());
                if (!result) {
                    ++totalEnqueueFail;
                }
            }
        };

        auto consumer = [&]() {
            for (size_t i = 0; i < TotalOps; ++i) {
                T result { };
                bool deq = queue.dequeue(result);
                if (!deq) {
                    ++totalDequeueFail;
                }
            }
        };

        chrono::time_point<chrono::system_clock> start, end;
        start = chrono::system_clock::now();

        std::thread thProducer(producer);
        std::thread thConsumer(consumer);

        thConsumer.join();
        thProducer.join();

        end = chrono::system_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        cout << "Total of " << TotalOps << " operations elapsed in " 
             << elapsed.count() << " seconds" << endl;

        cout << "totalEnqueFail = " << totalEnqueueFail << endl;
        cout << "totalDequeueFail = " << totalDequeueFail << endl;

    }



private:
};

int main() {
    Perf<uint32_t, 10000> perf;

    perf.run();
}
