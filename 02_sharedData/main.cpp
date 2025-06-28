#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>

// Increment using atomic variable
void sharedIncrementI(std::atomic<int> &value)
{
    for (int i = 0; i < 1000; i++)
        value++; // Atomic increment
}

// Demonstrates atomic usage
void usingAtomic()
{
    std::atomic<int> count;
    count = 0;

    std::thread t1(sharedIncrementI, std::ref(count));
    std::thread t2(sharedIncrementI, std::ref(count));

    t1.join();
    t2.join();

    std::cout << count << std::endl; // Should be 2000
}

// Increment using manual mutex lock/unlock
void sharedIncrementII(int &value, std::mutex& mtx)
{
    for (int i = 0; i < 1000; i++)
    {
        mtx.lock(); // Lock mutex
        value++;
        mtx.unlock(); // Unlock mutex
    }
}

// Increment using lock_guard for exception safety
void sharedIncrementIII(int &value, std::mutex& mtx)
{
    for (int i = 0; i < 1000; i++)
    {
        std::lock_guard<std::mutex> guard(mtx); // Lock/unlock automatically
        value++;
    }
}

// Demonstrates mutex usage
void usingMutex()
{
    int count = 0;
    std::mutex mtx;

    std::thread t1(sharedIncrementII, std::ref(count), std::ref(mtx));
    std::thread t2(sharedIncrementIII, std::ref(count), std::ref(mtx));

    t1.join();
    t2.join();

    std::cout << count << std::endl; // Should be 2000
}

int main()
{
    usingAtomic();
    usingMutex();
}