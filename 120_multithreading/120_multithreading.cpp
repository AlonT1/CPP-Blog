/*
Terms and Definitions:

Instruction - a single operation of a processor.
Program - a set of instructions to cpu. stored on non-volatile memory (disc).
created by programming languages (binary code). When executed, their loaded into ram and read by cpu,
Software - made up of two or more programs (many files)
Application - application software (=app) - a set of programs that make up a software.
Operating system - software which manages hardware, software resources and services.
Task - a set of instructions loaded in memory ran by a thread. in practice this means a program/app.


Process - program loaded into memory by OS, has at least 1 thread running a task.
every process is in one of the following state:
ready (when other process is running), running (by the cpu), waiting (for some event, e.g: i/o),
every process has a PCB given to it in its creation (process control block, a data structure) that contains:
1. a memory layout : text (binary code), Data segment (static/global vars), 
BSS (Uninitialized global and static vars), stack segment (for the single thread), heap segment.
OS kernel space (read 26_destructor for more info). the memory layout is "spread" across
the unique space address (a chunk of memory) that each process has 
2. registers (quickly acessible memory on the cpu) - although a cpu has a single set
of registers, the cpu can load the registers of the process from the PCB, when different processes
starts and resumes back during context switching (detailed below). e.g: program counter register
(a.k.a instruction pointer - stores the address of the next instruction to be executed, when switching
back to the process we left, this registers tells us what is the next instruction)
3. Process id
4. memory limits
5. scheduiling priorities
6...... (95+ fields)


Concurrency of tasks whether it is asynchronous or synchronous(when dealing with a single core)
is implemented via Scheduling of tasks, done by the operating system by the means of different algorithms.

Long-Term Scheduler -  its duty is to bring the process from the JOB pool to the Ready state for its execution.
Short-Term Scheduler - responsible for selecting one process from the ready state for scheduling it on the running state.
some are preemptive (stop a job and schedule another, all done within small time slices which represent
the runnning time alloted per task).  (round robin, shortest remaining time first) and some are non-preemptive 
(process keeps the cpu until it is terminated/done) (Shortest job first, first come first served).
Medium-Term Scheduler -  suspends processes (transfer to waiting queue - e.g: when process waits for i/o)

The queues (ready, waiting) store the pcb of processes (chrome pcb, powerpoint pcb).

          Long-Term Scheduler                 Short-Term Scheduler
Job Pool----------------------->Ready Queue-------------------------->CPU---->exit
                                     ↑                                  |
                                     |                                  |
                                     ---------I/O Waiting Queue←--------
                                                                    ↑
                                                         Medium-term scheduler

The scheduling algorithms of the short-term scheduler decides which process will be selected to run.
The actual switching between "ready" and "running" processes is done via context switching.

context switching - cpu performs Context Swithching which enables multitasking - switching from
one thread to another: this could be switching between threads within the same process
or switching between two processes because we need to reach one of their threads.
EVENTUALLY THE THREADS ARE SCHEDULED - they are considered as the unit of work.
we may need to switch between threads of different processes or between threads within a process.

scheduling example (preemptive), by storing and resuming execution in later points:
*p1 (process 1), PCB (process control block).
thread of p1: ----running->-----------------waiting--------------------------------------->
CPU: ----------interrupt p1->---save state into PCB1, reload PCB2---------------------->
thread of p2: ----waiting--------------------------------------------------running----->

This is done by the scheduling algorithms describe above.
switching between processes through context switching requires loading it into RAM - performance heavy
(via MMU which maps virtual memory address of a process to a physical ram address - described below),
in contrast to switching between different threads within the same process where we
save and restore registers such as stack and instruction pointers (threads live in the stack
of the process, detailed below), thus context switching between
threads in the same process typically occurs faster than context switching between processes



How to create a process?
fork() command - creates a "child" process of the "parent process":
    a. creates and initializes the PCB (process control block)
    b. initializes the stack inside the PCB with a full copy of the "parent" process
    c. puts the "child" process in the "ready" queue.

Thread - (a.k.a thread of exectuion) - conceptually considered as the unit of execution that lies within a process.
a process can have multiple threads. its an ordered sequence of instructions that can be processed by a single CPU core.
Note that processes are performance heavy (have their own PCB intercomuunication, own files), and they help
solving multittasking (word - gets input, checks spelling, formats text...).
A multithreaded process has its own memory layout (text, static data, data segment....),
and each thread has its own stack representing it, within the stack of the process.
(in addition to its own set of registers - this why the thread is considered a unit of work,
its registers manipulates data and its part of the stack saves variables and stack frames of functions).
so if there are 2 threads, the memory layout of the process has 2 stacks inside the stack
that represents the process (processes have only one heap - the heap of process). 
Therefore if a process dies its resources and threads die as well. 


 +--------------------------+
 |    kernel space          |
 +--------------------------+
 | Stack of thread 1        | also called "thread stack"
 |--------------------------|
 | Stack of thread 2        |
 |--------------------------|
 |                          |
 | (grows-downwards)        |
 |                          |
 |         F R E E          |
 |        S P A C E         |
 |--------------------------|
 |     (grows upwards) Heap |
 +--------------------------+
 |    Initialized data      |
 |         segment          |
 +--------------------------+
 |     Initialized to       |
 |        Zero (BSS)        |
 +--------------------------+
 |      Program Code        |
 +--------------------------+

Threads can be viewed as a lightweight process since:
1. threads share the same address space of the process (occupy the same memory).
2. no context switching between processes (no MMU, no mapping between virtual and
physical addresses via a lookup table) - although threads do go through scheduling.
Instead of context switching, and because threads are essentially in the stack of the process,
we can simply move the stack and instruction pointer to the specific thread of the process. 
3. threads share the same global variables, resources (e.g: opened files), permissions as the process
(although they do have unique local variables and function calls on their portion of the stack).

Threads have their own   stack, registers,
attributes (policies, for scheduling), thread specific data...
But unlike processes they don't share files, heap, scheduling attributes,
global variables, initialized to zero (BSS) global and static variables, 
and don't have interprocess communication
(pipes, semaphores, etc..), their address space is the same as the process,
so they don't need a whole new memory layout (heap, text, BSS, data segement) of their own -
they share the data that exists in the process (Unlike forking (duplicating) processes
where a COMPLETE COPY of everything (heap, files, resources, stack, data) is created.
The code itself, any resources and files belong to the process and are shared across the threads

***********************************************
in fact, when a process is created (which has 1 thread by default), the initial reigsters and
stack of the process actually belong to the thread! the process doesn't have a stack
and registers of its own, they are all unique per thread within the process. if we create additional threads
we'll have 2 stack frames in the stack of the process and 2 sets of registers.

Advantages  threads vs processes include:
1. Lower resource consumption of threads: because threads share the same resources
of the process, an application can operate using fewer
resources than it would need when using multiple processes.
2. Simplified sharing and communication of threads: unlike processes, which require
a message passing or shared memory mechanism to perform inter-process communication (IPC),
threads can communicate through data, code and files they already share.

Disadvantages - threads vs processes
1.  if a thread crashes - the process may crash due to threads sharing the same address space,
an illegal operation performed by a thread can crash the entire process;
therefore, one misbehaving thread can disrupt the processing of all the other threads in the application.
On the other hand, if a program supports multi-processes like chrome where each tab is a process - 
if one process (tab) crashes, the rest of tabs (processes) remain functioning.
2. debugging is hard with multithreads, its easier to debug processes because they are more
"atomic", no data races, they are all unique with their own memory layout.
3. processes are scalable we delegate process a to machine 1 and process b to machine 2,
while threads exist withing the memory space of a single process.


Within the context of a process, threads can be thought of the actual unit of work -


The os manages memory
fastest memory speed                        slowest memory speed
expensive (price)                           cheapest (price)

registers----------cache-------------ram----------disk

1GB of the address space is reserved for the kernel. the remaining
space is the actual memory layout model (stack, heap, text, data...)

How memory is allocated for processes?
    1. A process is given its own address space - this initial chunk of memory is called
    a virtual address space allocated by the OS specifically for the process. It's called
    virtual because the process thinks it has all the physical memory for itself (e.g: 4GB)
    2. The MMU - Memory Management Unit (CPU Component) -
    maps the virtual addresses of the process to real physical adresses on the RAM,
    done using a lookup table stored in the ram (the app (process) still thinks its the only one running,
    writing to any virtual address it wants), the MMU manages everything, the app doesn't
    care about other processes. for example
    Process1 occupies 0-100 virtual memory addresses,
    Process2 occupies 0-100 virtual memory addresses,
    The OS (via the MMU) maps process 1 to 100-200 and process2 to 300-400 address of the physical ram.
    Note that the MMU can decide to fragment (split) the processes into multiple chunks across the ram.
    When an app requests a resource, the MMU looks it up in the pysical memory and retrieves it

    The problem is that a naive lookup table that performs a mapping of 1-to-1 between each virtual memory
    address to a physical address will be huge! for example:
    assume a 300MB process => 2,400,000,000 bits / 32 bits sytsem => 75,000,000 memory addresses!
    (because each memory address on a 32 bit system is 32 bit long). therefore the lookup table
    must have 75,000,000 pointers! because each address is 32 bit long, the size of each pointer
    is 32 bit => 32 bit * 75,000,000 pointers =>  300MB for the mapping! while the process itself is 300MB!

    Solution: using pages - the physical memory is divided into different blocks - pages - 4k (4000 bits) in size
    and the MMU maps virtual address into these pages.
    so now the page table weighs: 300MB process => 2,400,000,000 bits / 32 bits system / 4k (4000 bits) =
    18570 bits => 0.002 MB needed instead of 300mb previously

    When an address is repeatedly needed, instead of repeated RAM access, the CPU
    has a cache of recently looked up address (translation lookaside buffer TLB), 
    storing recently accessesd addresses of pages.

    When MMU can't find an entry for mapping between virtual and physical memory - 
    causes a segmentation fault (crash) (e.g: when an app tries to access a forbidden location).
    A page fault occurs when a program attempts to access data from the virtual sapce,
    but it is not currently located in the system RAM.

    Virtual memory has another meaning: when an operating system that compensates
    shortages  of physical memory by transferring pages of data from random access memory
    to disk storage.


concurrency - Execution of multiple tasks at the same time, not necessarily simultaneously (parallel).
Parallelism - when multiple tasks run at the same time and in parallel(on a multiple core cpu)

Synchronous - tasks are executed one after the completion of the other (eat-> done then sing->done).
Asynchronous - "jumping" between execution of multiple tasks while the tasks aren't necessarily
finished (executing task "a", stopping task "a", switching to task "b", stopping task "b"
returning to task "a").

Concurrency in a single core cpu: 
only non-parallel concurrency can exist in a single core cpu, because there aren't
more cores that can work in parallel.
1. Non-Parallel synchronous:
Thread (cpu) executes task 1 until its finished -> executing task 2
2. Non-Parallel Asynchronous:
core (cpu) executes task 1 -> stop task 1 -> runs task 2 -> stops task 2 -> runs task 1
**a task means a process who must have a single thread
https://www.studytonight.com/operating-system/cpu-scheduling


Concurrency in a multicore cpu:
The concurrency here is parallel because each core handles 1 thread 
(or 2 threads if hyperthreading enabled).
1. Parallel Synchronous (switching a task only after completing one):
core1: task1 finished -> task6 finished -> task 4 wip...
core2: task2 wip....
core3: task5 finished -> task7 finished -> task 8 wip...
core4: task3 wip....

2. Parallel Asynchrnous (each thread is time slicing):
core1: task1 wip -> task6 finished -> task 1 wip...
core2: task2 wip....
core3: task5 wip... -> task7 wip... -> task 5 finished -> task 7 wip...
core4: task3 wip....


CPU - electronic circuit inside the computer that carries out program instructions
(e.g: mainpulating registers and performing arithmetic calculations).
reads 0's and 1's the make up the machine code.
when firing an application the code for that program is taken from the hard drive 
and stored in the RAM before being fed to the cpu which reads instructions from the ram.


Core - Physical Core - Mult-core processor - 1 cpu (processor) which integrates additional 
individual physical processors (=cores) into one integrated circuit. performance gain is usually 
not doubled due to imperfect alogrithms that don't implement multi threading. 
multi-core processors were developed to help CPUs run faster as  it became more difficult 
to increase clock speed (how quickly each physical core retrieves and interprets instructions - the speed
1 core can handle successive set of instructions from a single program (e.g: Microsoft word).
this means that 4 cores can parallely and independenly run and manage 4 different programs

Multithreading: can have 2 meanings:
1. refers to a program that can take advantage of a multicore cpu.
2. a process having more than one thread.

Hyper Threading - hyper threading enables a single core to interact concurrently with 2 threads
instead of 1. So instead of processing a single set of instructions from a single thread,
the physical core can switch very fast between two different thread (not parallel! but concurrent!),
each have their own set of instructions. This maximizes efficiency when the 1st
thread is "resting" and the 2nd thread needs "attention".

Logical cores - number of physical cores * number of threads that can run on each core.
e.g: an inte core i5 processor has 4 phyiscal cores and suports hyperthreading like most cpu's
thus 4 physical cores * 2 threads possible per core
(thanks to hyperthreading, exists per single physical core) = 8 threads
Logical cores are the actual cores that the OS "sees".

How many threads to should we create in our program:
at max one-to-one ration with the PHYSICAL core count. more then that performances gets worse.
some say that the max is the the number of logical cores
https://stackoverflow.com/questions/4828296/how-many-threads-can-i-run-concurrently

*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <numeric>

#if 0
int counter = 0;
std::mutex mtx;

void increase_counter()
{
    for (int i{ 0 }; i < 100'000; ++i)
    {
        if (mtx.try_lock())
        {
            ++counter;
            mtx.unlock();
        }
    }
}

int main()
{
    std::thread t1(increase_counter);
    std::thread t2(increase_counter);
    t1.join();
    t2.join();
    std::cout << counter;

}
#endif

/*
global variables:
1. for processes - not shared since each process has its unique memory layout
(code, bss, stack, heap, globals...)
2. for threads - because threads exist in the address space of the process (sharing
files, stack, heap..) they share the same global variables
*/
int a = 4;


void func(int &a)
{ 
    a = 5;
    std::cout << "hello" << a << '\n';
}

std::mutex mutex;
void func_lock()
{
    mutex.lock();
    for (int i{ 0 }; i < 100; ++i) //since each thread has its own stack, i is local per thread
        std::cout << std::this_thread::get_id() << " prints: " << i << '\n';
    mutex.unlock();
}

std::timed_mutex tm;
void func_trylock_trylockfor()
{
    if (!mutex.try_lock()) //or if(tm.try_lock_for(std::chrono::seconds(3))
        return;
    for (int i{ 0 }; i < 100; ++i) //since each thread has its own stack, i is local per thread
        std::cout << std::this_thread::get_id() << " prints: " << i << '\n';
    mutex.unlock();
}


void func_lockguard()
{
    std::lock_guard<std::mutex> lock(mutex);// equivalent to: "std::unique_lock<std::mutex> lock(mutex);"
    for (int i{ 0 }; i < 100; ++i) //since each thread has its own stack, i is local per thread
        std::cout << std::this_thread::get_id() << " prints: " << i << '\n';
    //"lock" is unlocked when the locking thread goes out of scope
}

void func_uniquelock()
{
    std::unique_lock<std::mutex> lock(mutex); 
    for (int i{ 0 }; i < 100; ++i) //since each thread has its own stack, i is local per thread
        std::cout << std::this_thread::get_id() << " prints: " << i << '\n';
    //the dtor of unique_lock always unlocks the unique_lock
}


int money{ 0 };
std::condition_variable cv;
void add_money()
{
    std::lock_guard<std::mutex> lock(mutex);
    money += 500;
    std::cout << "money added: " << money << '\n';
    cv.notify_one(); // notify a waiting condtional variable - 
    //once notified the conditional variable will check its predicate (below).
}

void withdraw_money()
{
    std::unique_lock<std::mutex> lock(mutex); //condition variable only works with unique_lock!

    /*the wait functions sends this_thread to sleep. Therefore before it goes to sleep
    the wait function of the conditional variable needs to know the mutex to unlock it, allowing
    other blocked threads to continue their job, otherwise the sleeping thread will never unlock
    the mutex.
    once the conditional variable is notified, the thread will wakeup and check the prdicate
    (2nd parameter - a function that retuns a bool) and only if it returns true,
    the thread will continue, otherwise if it returns false - it will be remained block!
    Note: when the thread wakes up and finds that the condition is false - this is called
    a spurious wakeup.

    inefficient alternative to "wait":
    1. A while 
    which constantly  checks the same predicate - this causes redundant
    looping and perforamnce hit
    2. std::this_thread::sleep_for(std::chorno::seconds(?)) but it is hard
    to estimate how much time the thread needs to sleep to wait for the other
    thread to get the job done. conditional variables solve this
    because they get notified.
    
     */
    cv.wait(lock, []() {return money > 0;});
    money -= 500;
    std::cout << "money withdrawn: " << money << '\n';
}


void perform_task(std::promise<int>&& promise)
{
    promise.set_value(10);
}

bool is_even(int num)
{
    std::cout << std::this_thread::get_id() << '\n';
    return (num % 2) == 0;
}

int main()
{
    /*basic thread construction:*/
#if 0

    /************step 1: construct std::thread with a callable object**********
    constructing a thread. the arguments are:
        1. callable objects:
            a. function pointer (&func - operator & retrieves function to a pointer.
            note that functions in itself are not objects.
            b. functors
            c. lambdas (which are essentially functors)
            d.....
        2. arguments supplied to the callable objects. arguments passed by ref
        MUST use std::ref! return values from the callable objects are ignored.
        Note that due to data racing (explained in the data racing part) and because
        the referenced argument is shared between threads, reading this parameter
        is ok (should be treated as immutable) but changing it may produce 
        unexpected beahviour.

    Behind the scenes (tl;dr of the text above on "process"):
    The  memory layout of a process (text code, stack, heap, bss, globals...) is created
    within a virtual address space, e.g: 0-100, this frees the process from thinking about
    if these addresses are actually occupied in the RAM. Later the MMU maps the process
    from the the virtual address to actual physical address on the RAM. every process
    has inherently 1 thread which is conceptually considered the actual unit of work
    (has own set registers that manipulate data and stack that stores frames of function calls
    and saves variables within them, manipulated by the registers). Thus when dealing with 
    a single thread, the stack of the thread IS the stack of the process.

    The constructed thread "t" is the child of t parent thread "main" - this is expressed
    by the fact that we need to choose a relationship between them - in the next step.
    In practice though the -*/
    int a = 1;
    std::thread t(func, std::ref(a));

    /****step 2: choose relationship between parent & child threads (join/detach)*****

    after constructing the thread we MUST determine the relationship between
    thread child "t" and parent thread "main" before "t" goes out of scope. We have 2 relationship choices -
    we must choose exactly 1 of them & execute it once - otherwise  - error!:

    1. t.join(): the main parent thread is blocked / waits for thread t (child)
    to finish ("join up with main"). main thread "owns" thread t, if parent dies,
    the child dies.*/
    t.join();

    std::cout << a;

    /*2. t.detach(): separates main (parent) and child (t) for independent execution.
    thread main will no longer "own" child thread t - when main will finish,
    the cpp runtime library will be responsible to reclaim t which is also
    called a daemon process (background process). some daemon processes will run
    until the system shuts down.

    when using detach, "main" thread could finish before "t", resulting in the
    job of "t" not completing. "t" can also finish before main, the order
    of thread execution is not deterministic unless we use join, locks etc.

    *joinable - checks if we already chose a relationship (join/detach)
    because performing join() or detach() a second time causes a crash.*/
    if (t.joinable())
        t.detach();

    //every thread has its own unique id
    std::cout << std::this_thread::get_id() << '\n';

    //threads cannot be copied via "=" only moved. t is empty now.
    std::thread t2 = std::move(t);

    /*the number of threads  we create at most must equal to the number of cores
    on the cpu. Creating more threads then available cores is called oversubscription
    and it is not efficient
    returns number of logical cores (physical cores * how many threads each core can run)
    this is the number of cores the cpu actually sees.*/
    std::cout << "number of cores: " << std::thread::hardware_concurrency() << '\n';

    //There is no guarantee about the finishing order of  threads - the parent may finish
    //before the child and vice versa.

    //rule of thumb - if in doubt, declare join or detach relationship at the end
    //of the scope
#endif

    /*Data Race and Mutex (locking and locking):*/
#if 0

    /*Race Condition - a condition where the outcome of a program depends on the relative
    execution order of one or two threads that try to read/write to the same resource cocurrently.
    Same resource means a global object (file, variable, instance...), a reference passed between 2 threads...
    Critical Section - A piece of code that consists of shared resources by threads, such as
    global variables or global objects (std::cout), where concurrent access to these resources
    can lead to unexpected behaviour.
    Mutex - synchronizes the access of threads to the critical section.
    in practice, ensuring that one thread locks (i.e take ownership of the mutex)
    and therefore can be certain that he is the only thread that can access the critical section.
    THIS CAN ONLY HAPPEN WHEN BOTH THREADS GOVERN THE SAME GLOBAL MUTEX OBJECT:
    before entering the critical section the  thread must lock access to the resource to 
    use it exclusively and at the end the thread should unlock the mutex to allow other threads to do the same thing.
    Like a bathroom, where each person(thread) locks the door (mutex) when entering and
    unlocks the door (mutex) when leaving allowing other people (threads) to come in and do the same thing,
    all manipulate the same mutex (key-door), synchronizing their order for entering the bathroom
    (order of exeution of the ciritical section).
    */
    std::thread t(func_lock);
    /*the mutex (locking and unlocking) in func2_lock causes mutual exclusion (mutex),
    upon the critical section (std::cout is global and accessible by both threads).
     one of the threads will necessarily reach func 1st (in microsecond difference)
    (which one is indetermined), and will lock the loop (critical section). 
    When the other thread reaches the same "lock()" command it will recognize
    that the same mutex already has been locked. The thread will become blocked
    and is waiting for the other thread to unlock.
    in this time the waiting thread does not do anything, it just waits, and cannot
    access the critical section.
    The mutex prevents two threads from manipulating the same global object std::cout, or any
    global variables or shared resources which cause erronous behaviour, as seen
    with the mess printed caused by two threads using std::cout!
    
    Whether "t" thread will execute func_lock first or "main" thread will execute func_lock
    first is indetermined. Also when one of the threads locks the critical section,
    no other thread can access it, BUT! when the thread unlocks it, the unlocking
    thread gives a chance for the other thread to gain access to the section 
    and lock it for its own use.
    *************************************************************************
    The mutex guarantees only one thing - when one thread is acesssing the critical section,
    it will block (prevent) other threads from doing the same thing
    *************************************************************************

    Note that when two threads want access to the same critical section, and the mutex
    is already locked by a thread it must unlock the mutex when it is done with the job, otherwise the
    blocked thread who just met the lock() command and waits for the mutex to be unlocked,
    will wait forever - it is blocked from continuing to the critical section until the
    2nd thread owning the mutex will unlock it

    in practice - "main" thread seems to gain access first (maybe because it takes time
    to setup thread "t", and thread "main" locks and unlocks the critical section so 
    fast that it doesn't give thread "t" any chance to gain access to the section
    and lock it for its own use.*/
    func_lock();

    /*another options instead of the standard lock():
    1. try_lock function of the mutex:
        The thread that performs a "lock()" on the critical access gains exclusive
        access to it while the other threads that try to execute the function are waiting for
        the mutex to be unlocked (blocked).
        Maybe we can utilize the waiting threads for another job?
        Thus try_lock() exists - if a thread tries to lock the critical section, but fails
        because another thread already locked it, instead of just waiting for it to be unlocked
        try_lock retuns false when the section is already locked, therefore we can use this
        to "return" from the section, and utilize the otherwise "just-waiting" thread to do
        other job.
    2. "m.try_lock_for(std::chrono::seconds(3));":
       this mutex blocks a thread from entering a lock critical section, until the
       mutex is unlocked, or until the time elapses. on successful lock aquisition 
       returns true else false (then we can retun from the function and utilize the
       thread for other purpose instead of letting him be blocked and wait.*/
    func_trylock_trylockfor(); //examplifies 1 and 2

    /*3. "std::lock_guard<std::mutex> lock(m);" where m is "std::mutex m;":
        std::lock_guard is wrapper around a mutex implementing RAII (scoped based - auto unlock):
        No need for lock() and unlock() - the ctor of "std::lock_guard<std::mutex> lock(m);"
        locks the critical section for an owning thread, and when the function goes out of
        scope, the destructor of "lock" object will go out of scope and its dtor will unlock
        the mutex. We cannot explicitly unlock the lock_guard!, no lock.unlock()!
        denotes to other programmers that we want to lock until the end of the scope.*/
    func_lockguard();

    /*4. std::unique_lock - see func_uniquelock() for more details
    //equivalent to std::lock_guard<std::mutex> lock(mutex);"
    unique_lock is similar to lock_guard in the sense with both the ctor locks and dtor unlocks.
    Unique_lock allows us to manually unlock the lock as opposed to lock_guard
    the advatange of unique_lock is that it provides further flexibility with 3 locking strategies:
    1. deferred_lock:
        the ctor won't lock the mutex, the locking is deferred for another time - done manually by us.
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
        lock.lock(); //now it is locked
        the constructor essentially call the lock function of the mutex it wraps
    2. try_to_lock:
        std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);
        if(lock) {....} //overrides bool conversion function to return the lock.owns_lock()
        and tells us if the locking was successful
        the constructor essentially call the try_lock function of the mutex it wraps
    3. try_lock_for
        std::unique_lock<std::mutex> lock(mutex, std::try_to_lock_for);
        //no examples available
        just use the member funcion:
        lock.try_to_lock_for(std::chrono::seconds(4));
        unique_lock can be instantiated as above:
        std::unique_lock<std::mutex> lock(mutex);
        and we can use the member functions that are identical to try_lock, try_lock_for,
        try_lock_until. */
    func_uniquelock();
    t.join(); //rule of thumb: 
#endif

    /*mutex based on some variable (condition variable):*/
#if 1
    /*tl;dr
    a global variable x is shared between threads t1 & t2, where t2's continuation is dependant 
    on a modification of x. t2 can use a global "conditional variable" cv's wait command(),
    that puts the thread to sleep not before unlocking the mutex, 
    allowing t1 to manipulate x and then notify t2 via the same global
    conditional variable, causing him to wake up and check a predicate to test if indeed
    the x was changed.

    t1 and t2 threads run - their order of execution and order of completion is indetermined.
    t1 adds 500$ to money global variable - "money+=500;"
    t2 withdraws those 500$ - "money-=500;"
    we must ensure that thread t2 withdraws 500$ from an account that has 500$,
    therefore we must synchronize that t2 (withdraw money) acts after t1 (add money).
    
    this can be implemented via condition variable - an object that is able to
    block the thread handling the condition variable, causing it wait until it is 
    notified to resume by another thread. This assumes that both threads
    manipulate the same mutex and mirrors a while loop that checks a condition
    to move forward, albeit the wait() function is more efficient, saving the redundant
    looping by simply putting the thread to sleep until it is notified by other thread
    to check the predicate and see if it is ok to continue.

    If t2 starts, it will lock the mutex. 
    t1 will try to lock the same mutex, because both handle global variable std::cout and money,
    thus avoiding a data race and erronous behaviour stemming for manipulating global vars.
    t1 will be blocked because t2 has locked the mutex.
    t2 hits the wait command of the condition variable, causing t2 to sleep, not before 
    the wait() command unlocks the mutex, allowing t1 to continue, while t2  waits for
    notification to wake up from other threads handling the conditional variable.
    Once this notification arrives from t1 (notify_one())
    t2 will wake up and check the predicate of the conditional variable - if the predicate
    is true - t2 will continue, if the predicate is false - t2 will remain blocked
    until the next notfication to check the predicate.


    Note - Sleep vs Wait:
        a. Sleeping thread: A thread that owns the mutex (has locked it) and went to sleep (inactive, will not be scheduled
        for a certain amount of milliseocnds). A sleeping thread keeps the mutex locked + the thread is inactive
        in itself, so no one can access the critical section, until thread wakes up and unlocks the mutex.
        we can use  std::this_thread::sleep_for(std::chrono::seconds(5)) to make a thread goes to sleep.
        in itself, the sleep function will not unlock the mutex, we can do this manually if we want
        b. Waiting Thread: A thread that owns the mutex, but before it starts to "wait" meaning it
        becomes inactive, it unlocks the mutex to allow other threads to manipulate
        the critical section. Waiting threads will wake up only when other threads notify them to this via
        the notify function. the wait() function of a conditional variable both unlocks the mutex
        and causes the thread to wait (be inactive) until notified by other threads using the notify_one()
        or notify_all() function of the conditional variable object.


    */
    std::thread t1(add_money);
    std::thread t2(withdraw_money);
    t1.join();
    t2.join();
#endif 

    /*Future and Promise*/
#if 0

    /*
    Future and promise give a solution for retrieving a value from thread t2 
    to thread t1. The naive way to do this is to pass an std::ref as an argument
    from t1 to a function ran by t2, essentially both threads will manipulate the same
    value - this of course will cause a data race with potential erronous behaviour because
    2 threads will manipulate a shared resource.
    The other alternative is to use conditional variables and mutex, but it can be cumbersome.

    Future and promise solve this issue.

    assume 2 threads, t1 and t2:
    promise - promising that thread t2 will provide a value at some point
    by setting a value to the promise (fulfill the promise - promise.set_value()).
    future - t1 will get (future.get()) the value from the promise set in t2 at some time in the future,
    in the meantime, when encoutering future.get(), t1 will sleep, and will be awakened, when
    t2 will fulfill the promise via promise.set_value(), only then t1 will wake up
    and retrieve the value via future.get().
    There is a clear distinction between these two mechanisms - 
    promise sets (a value) and future gets (the value).
    The future object must be "linked" with the specific promise, because when we use
    future.get() the future must know from which promise to retrieve the value.
    */


    //promise to be fulfilled (value to be set) by another thread.
    //future and promise must be paramterized with the same type (they deal with).
    std::promise<int> promise;  
    //linking the future object with the promise object, in this way the future object in t1 will get the value
    //from this specific promise (as promised) in the future, when t2 wil fulfill it (set a value to it).
    std::future<int> future = promise.get_future();
    //launching t2 - moving the promise to it, hoping t2 will fulfill the promise.

    std::thread t(perform_task, std::move(promise)); //promises MUST BE MOVED! 

    //get() blocks this thread, until the promise is set at some time "in the future" by t2
    //via promise.set_value(). This will wake this thread backup causing future.get() to retrieve the value in the promise
    ////we can use future.wait_for(std::chrono::seconds(4)) instead to ensure we ain't
    //stuck for execssive amount of time of the promise isn't fulfilled by the other
    //thread for some reason
    int promised_value = future.get(); 
    std::cout << promised_value << '\n';
    t.join();

    /*
    The problem with future-promise setup aside from the extra syntax,
    is that we want to retrieve a value from t2 but we do this thorugh the
    promise, and not in the classic functional way via the return value of the
    function. This also disable the abillity to use the function
    in a non-multithreaded way, because there is no actual classic return - 
    only a promise object fulfilled, fit for threading.
    async solves this and automates much of the syntax here.

    in conclusion the setup of promise and future is:
    1. create a promise obj
    2. link a future obj with the promise obj created in 1
    3. create a thread t2 and passing the promise to the function ran by t2
    4. t1 waits for the promise to be fulfilled in the future via future.get(),
    which will get the value in the promise (future was linked with the promise in 2)
    5. t2 will fulfill the promise via promise.set_value(), waking t1 up causing him
    to retrieve the value from the promise, fulfilled in 4,  via future.get().
    */
#endif


    /*async*/
#if 1
    /*
    async automates the setup of future and promises, saving us the following steps:
    1. creating a promise obj.
    2. linking a future obj with a promise obj.
    3. creating a thread.
    4. moving the promise to the function ran by a thread t2.
    5. fulfilling the promise in t2

    async creates a thread, creates and and links a future object with an implicitly created
    promise object, sends the promise to the function ran by t2, causes t1 to wait
    for the promise to be fulfilled by activating future.get() on the implicitly
    created future object, it even implicitly joins the thread it created,
    and allows us to specify a return type in the function ran by t2 through which
    the type of the promise is deduced.
    This causes the code to more readable, we know what the function ran by t2 returns
    and the same function can be used in a non-multithreaded fashion, In contrast 
    to the future/promise manual setup in which the value was manually set in the promise,
    and the function didn't "return;"'d anything.
 
    async has a few policies, passed as the 1st argument 
    1.std::launch::deferred - everything will be excuted in the same thread, when future.get()
    executes, the same thread will execute the function is_even, will fulfill the promise and
    return back
    2. std::launch::async - will create the thread 
    3. std::launch::async | std::launch:deferred (ORing both of them) - whether async function
    will create another thread or not will depend on the implementation.
    THIS IS THE DEFAULT for this parameter

    therefore if we want to make sure that a new thread is created we must provide the
    std::launch::async argument.
    */
    std::future<bool> future = std::async( is_even, 4);
    std::cout << std::this_thread::get_id() << '\n';
    bool returned_value = future.get(); //future.get() can appear only once - crash otherwise!
    std::cout << returned_value;
#endif

}

/*
Notes:
1. the loading of each file can occur on each seperate thread (saving performance), but the insertion
to a shared global vector should occur via mutex
*/


