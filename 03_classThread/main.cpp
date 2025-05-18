#include <thread>
#include <iostream>
#include <mutex>

class Hello
{
    void helloWorld()
    {std::cout << "Hello world\n";}
    std::thread t;
    public:
    void startThread()
    {
        t = std::thread(&Hello::helloWorld, this);
        t.join();
    }
};

class App
{
public:
    void operator()()
    {
        for (int i = 0; i < 1000; i++)
        {
            std::lock_guard<std::mutex> guard(mtx);
            count++;
        }
    }

    int getCount(){return count;}

    private:
    int count = 0;
    std::mutex mtx;
};
int main()
{
    Hello h;
    h.startThread();

    App app;
    std::thread t1(std::ref(app));
    std::thread t2(std::ref(app));

    t1.join();
    t2.join();

    std::cout << app.getCount() << std::endl;
}