#include <thread>
#include <iostream>
#include <mutex>

// Class with a member function run in a thread
class Hello
{
    void helloWorld()
    {
        std::cout << "Hello world\n";
    }
    std::thread t;
public:
    void startThread()
    {
        t = std::thread(&Hello::helloWorld, this); // Start thread with member function
        t.join(); // Wait for thread to finish
    }
};

// Functor class for counting with mutex protection
class App
{
public:
    void operator()()
    {
        for (int i = 0; i < 1000; i++)
        {
            std::lock_guard<std::mutex> guard(mtx); // Protect count
            count++;
        }
    }

    int getCount() { return count; }

private:
    int count = 0;
    std::mutex mtx;
};

int main()
{
    Hello h;
    h.startThread(); // Run Hello::helloWorld in a thread

    App app;
    std::thread t1(std::ref(app)); // Thread 1 runs App
    std::thread t2(std::ref(app)); // Thread 2 runs App

    t1.join();
    t2.join();

    std::cout << app.getCount() << std::endl; // Should be 2000
}