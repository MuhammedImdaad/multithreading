#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>

void sharedIncrementI(std::atomic<int> &value)
{
    for (int i = 0; i < 1000; i++)
        value++;
}

void usingAtomic()
{
    std::atomic<int> count;
    count = 0;

    std::thread t1(sharedIncrementI, std::ref(count));

    std::thread t2(sharedIncrementI, std::ref(count));

    t1.join();
    t2.join();

    std::cout << count << std::endl;
}

void sharedIncrementII(int &value, std::mutex& mtx)
{
    for (int i = 0; i < 1000; i++)
    {
        mtx.lock();
        value++;
        mtx.unlock();
    }
}

void sharedIncrementIII(int &value, std::mutex& mtx)
{
    for (int i = 0; i < 1000; i++)
    {
        std::lock_guard<std::mutex> guard(mtx);
        value++;

    }
}

void usingMutex()
{
    int count = 0;
    std::mutex mtx;

    std::thread t1(sharedIncrementII, std::ref(count), std::ref(mtx));

    std::thread t2(sharedIncrementIII, std::ref(count), std::ref(mtx));

    t1.join();
    t2.join();

    std::cout << count << std::endl;
}

int main()
{
    usingAtomic();
    usingMutex();
}