// https://en.cppreference.com/w/cpp/thread/condition_variable
// https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
// https://stackoverflow.com/questions/33011362/synchronizing-very-fast-threads
#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <stack>
#include <random>
#include <atomic>

std::condition_variable cv;
std::mutex m;
std::stack<int> stack;
std::atomic<bool> workdone{ false };
std::atomic<bool> read_ready{ false };

/*
Alternating between two threads.

can be used as a basis for  a messaging system between threads - 
one thread pushes, the other thread reads. Steps:
1. A single mutex exist and shared between "producer" and "consumer" threads
because they both use global resource cout.
2. consumer is invoked and waits immediately for "read_ready" to be set "true".
3. producer is invoked and sets read_ready to true, notifies consumer, and wait
until read_ready will be set to false
4. consumer wakes up "reads" the message, and sets "read_ready" back to false
5. this wakes up producer thread and the steps repeat.

The program runs for 2 seconds until workdone is set to true, which 
stops both "while" threads. workdone is an atomic variable, i.e it is ok for
two threads to manipulate it without a mutex (without unexpected behaviour).
*/

void produce()
{
    while (!workdone.load())
    {
        std::unique_lock<std::mutex> lock(m);
        std::cout << "push\n";
        read_ready = true;
        cv.notify_one();
		cv.wait(lock, []() { return !read_ready.load(); });
    }
}

void consume()
{
    while (!workdone.load())
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, []() { return read_ready.load(); });
        std::cout << "pop\n";
        read_ready = false;
        cv.notify_one();
    }
}

int main()
{
    std::thread producer(produce);
    std::thread consumer(consume);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    workdone = true;

    producer.join(); 
    consumer.join();

    return 0;
}