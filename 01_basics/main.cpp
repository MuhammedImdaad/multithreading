#include <thread>
#include <iostream>
using namespace std::literals;

// Function to simulate work in a thread
void work(int id, int delay)
{
    for (int i = 0; i <= 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Simulate work
        std::cout << "Loop " << id << ": " << i << std::endl;
    }
}

// Function to demonstrate thread sleep
void foo() {
    std::this_thread::sleep_for(200ms);
}

// Demonstrates joinable, join, and detach
void joinable()
{
    std::cout << std::boolalpha;
    std::thread t;
    std::cout << "before starting, joinable: " << t.joinable() << '\n'; // Not joinable yet
    t = std::thread{foo};
    std::cout << "after starting, joinable: " << t.joinable() << '\n'; // Now joinable
    t.join();
    std::cout << "after joining, joinable: " << t.joinable() << '\n'; // Not joinable after join
    t = std::thread{foo};
    t.detach();
    std::cout << "after detaching, joinable: " << t.joinable() << '\n'; // Not joinable after detach
    std::this_thread::sleep_for(1000ms);
}

int main()
{
    joinable(); // Demonstrate joinable, join, detach

    std::cout << "join and detach\n";
    std::thread t1(work, 1, 500); // Thread 1 with 500ms delay
    std::thread t2(work, 2, 1000); // Thread 2 with 1000ms delay

    t1.join(); // Wait until t1 finishes
    t2.detach(); // t2 runs independently
}