#include <thread>
#include <iostream>
using namespace std::literals;

void work(int id, int delay)
{
    for (int i = 0; i <= 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        std::cout << "Loop " << id << ": " << i << std::endl;
    }
}

void foo() {
    std::this_thread::sleep_for(200ms);
}

void joinable()
{
    std::cout << std::boolalpha;
    std::thread t;
    std::cout << "before starting, joinable: " << t.joinable() << '\n';
    t = std::thread{foo};
    std::cout << "after starting, joinable: " << t.joinable() << '\n';
    t.join();
    std::cout << "after joining, joinable: " << t.joinable() << '\n';
    t = std::thread{foo};
    t.detach();
    std::cout << "after detaching, joinable: " << t.joinable() << '\n';
    std::this_thread::sleep_for(1000ms);
}
int main()
{
    joinable();

    std::cout << "join and detach\n";
    std::thread t1(work, 1, 500);
    std::thread t2(work, 2, 1000);

    t1.join(); // wait until t1 finishes
    t2.detach(); // it take more time, exit without waiting for t2
}