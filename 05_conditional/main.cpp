#include <thread>
#include <iostream>
#include <condition_variable>

volatile bool ready = false;     // Flag to signal worker to start
volatile bool processed = false; // Flag to signal main that work is done
std::mutex mtx;
std::condition_variable cv;

// Worker thread waits for 'ready', processes, then signals 'processed'
void workerThread()
{
    std::cout << "Worker thread is waiting\n";

    // Wait for main to signal 'ready'
    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]
                { return ready; }); // Wait until ready is true
    }

    std::cout << "Worker thread is processing... \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate work

    // Signal main that processing is done
    std::cout << "Worker thread signals data processing completed\n";
    {
        std::lock_guard<std::mutex> lk(mtx);
        processed = true;
        cv.notify_one(); // Notify main thread
    }
}

// Main thread signals 'ready', then waits for 'processed'
void mainThread()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate preparation
    std::cout << "main signals data ready for processing\n";
    {
        std::lock_guard<std::mutex> lk(mtx);
        ready = true;
        cv.notify_one(); // Notify worker thread
    }

    // Wait for worker to signal 'processed'
    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]()
                { return processed; }); // Wait until processed is true
    }

    std::cout << "Back in main" << '\n';
}

int main()
{
    try
    {
        std::thread t1(workerThread); // Start worker thread
        mainThread();                 // Run main thread logic
        t1.join();                    // Wait for worker to finish
    }
    catch (const std::system_error &e)
    {
        std::cerr << "Thread creation failed: " << e.what() << '\n';
        return -1;
    }
}