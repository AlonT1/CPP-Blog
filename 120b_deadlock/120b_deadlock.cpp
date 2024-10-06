#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m1;
std::mutex m2;

/*
Deadlock describes a condition in which two or more threads are
blocked (hung) forever because they are waiting for each other.
*/

void func1()
{
    std::lock_guard<std::mutex> lock1(m1);
    std::cout << "hello from thread1\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::lock_guard<std::mutex> lock2(m2);
}

void func2()
{
    std::lock_guard<std::mutex> lock1(m2);
    std::cout << "hello from thread2\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::lock_guard<std::mutex> lock2(m1);
}

void func3()
{
    m1.lock();
}

void func4()
{
    m1.lock();
}

int main()
{
#if 0  /*deadlock with two mutex*/
    /* Most deadlock examples have 2 mutex(one for security reasons, the second
    is for file sharing ? ? according to https ://www.youtube.com/watch?v=_N0B5ua7oN8 01:11) */
    std::thread t1(func1);
    std::thread t2(func2);
    t1.join();
    t2.join();
#endif

#if 1  /*deadlock with one mutex*/
    /* t1 locks the thread but doesn't unlock it. t2 tries to lock the thread,
    * but since it is already locked, t2 goes to sleep and waits until the mutex
    * will be unlocked. when it will unlock, t2 will lock it, but since the mutex
    * never unlocks, t2 will wait indefinitely.
    */
    std::thread t1(func3);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::thread t2(func4);
    t1.join();
    t2.join();
#endif

   
}