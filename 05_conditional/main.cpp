#include <thread>
#include <iostream>
#include <condition_variable>

volatile bool ready = false;
volatile bool processed = false;
std::mutex mtx;
std::condition_variable cv;

void workerThread()
{
    std::cout << "Worker thread is waiting\n";

    // ready
    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]
                { return ready; });
    }

    std::cout << "Worker thread is processing... \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Send data back to main
    std::cout << "Worker thread signals data processing completed\n";
    {
        std::lock_guard<std::mutex> lk(mtx);
        // Using std::lock_guard here because we only need a simple lock
        // without the flexibility of deferred locking or unlocking.
        processed = true;
        cv.notify_one();
    }
}

void mainThread()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "main signals data ready for processing\n";
    {
        // Using std::lock_guard ensures that the mutex is released automatically when the scope ends.
        std::lock_guard<std::mutex> lk(mtx);
        ready = true;
        cv.notify_one();
        // It’s safe to call notify_one() while holding the lock.
        // The waiting thread will not miss the notification;
        // it will just wait for the mutex to be released before continuing.
    }

    // processed
    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]()
                { return processed; });
    }

    std::cout << "Back in main" << '\n';
}

int main()
{
    try
    {
        std::thread t1(workerThread);
        mainThread();
        t1.join();
    }
    catch (const std::system_error &e)
    {
        std::cerr << "Thread creation failed: " << e.what() << '\n';
        return -1;
    }
}