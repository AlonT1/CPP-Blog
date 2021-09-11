/*
primary purpose of threads is optimization and executing multiple
things at once.
"usual" code uses 1 thread - cpu executes instructions 1 by 1.
we can move certain work to other words.

*/
#include <iostream>
#include <thread>


static bool s_finished = false; //s for static- used for s in 1s in sleep_for()

void do_work()
{
    // great for knowing which thread is running
    std::cout << "thread num " << std::this_thread::get_id() << std::endl; 
    using namespace std::literals::chrono_literals;
    while (!s_finished)
    {
        std::cout << "working...\n";
        std::this_thread::sleep_for(1s); //the while loop may cause 100% thread usage
        //sleep_for mitigates that
        //std::this_thread references the current "worker" thread
    }
    
}

/*
in thread 2, do_work will print "working...", meanwhile thread 1 is blocked thanks to cin.get().
when something is entered, thread 1 will execute s_finished = true, causing thread 2 to quit the loop
and in the end, worker.join() which blocks thread 1 until worker is finished, will unblock
thread 1 because worker is indeed finished.
*/

int main()
{
    std::thread worker(do_work); //making thread object - thread will continue until its done.
    //takes in a function pointer - kicks of the thread 
    std::cin.get();//std::cin.get() - must wait to recieve input(while loop behind the scenes) - blocks this thread.
    //we can use other threads such as "worker" to do some work in the mean time (printing
    s_finished = true; // causes worker thread to quit the loop and finish his job
    worker.join(); // essentialy waits for thread to finish (join) - "wait" in c#
    //asks the current thread to wait/be blocked until the other thread (worker) is completed
    //we run in parallel - the main thread starts a worker thread and eventually we say to the main thread
    //wait for the "worker" thread to finish all of its execution before we continue

    // great for knowing which thread is running
    std::cout << "thread num " << std::this_thread::get_id() << std::endl;
    std::cout << "finished\n";

    /*
    alternative to threads in this case:
    although std::cin.get is blocking, we can alternate between worker
    and a custom cin.get quickly to simulate a multithread and avoid the blocking 
    */
}
