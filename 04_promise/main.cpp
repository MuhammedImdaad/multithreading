#include <thread>
#include <iostream>
#include <future>

// Demonstrates setting a value in a promise from a thread
void promiseSetValue()
{
    // create a std::promise, pass its std::future to another thread,
    // and set the value later.
    std::promise<double> prms;

    std::thread t([&prms = prms]()
                  {
        double result = 0;
        for (int i = 0; i < 1'000'000; i++)
            result++;
        prms.set_value(result); // Set value in promise
    });

    // Used to retrieve results from std::async or std::promise.
    std::future<double> fut = prms.get_future();
    std::cout << "get value from promise - " << fut.get() << std::endl; // Wait for result
    t.join();
}

// Demonstrates setting an exception in a promise
void func(double value, std::promise<double> prms)
{
    try
    {
        if (value < 1)
        {
            std::cout << "exception thrown" << std::endl;
            throw std::runtime_error("can not be less than one");
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "exception raised" << std::endl;
        prms.set_exception(std::current_exception()); // Set exception in promise
    }
}

// Demonstrates exception handling with promise/future
void promiseSetException()
{
    std::promise<double> prms;
    std::future<double> fut = prms.get_future();

    std::thread t(func, 0, std::move(prms)); // Will throw exception
    try
    {
        std::cout << fut.get(); // Will rethrow exception
    }
    catch (const std::exception &e)
    {
        std::cout << "exception caught" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    t.join();
}

int main()
{
    promiseSetValue();
    promiseSetException();
}