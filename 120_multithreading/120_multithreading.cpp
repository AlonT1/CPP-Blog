/*


READ "Operating System OS.docx" first

tl;dr
1. starting child thread - std::thread(func, args...) t- 
starts a thread which runs func(), supplying args to func.
2. determine relationship between parent and child (MANDATORY - error otherwise) -
    a. t.join() - parent is blocked and waits
    for the child thread to finish (going through all func() statements)
    b. t.detach() - parent is detached from child, parent continues execution
    and won't wait for child

3. std::mutex mutex - ensures that a single thread deals with a critical section at a time.
how it works:
    a. parent thread instantiates "std::thread(func())" and parent itself invokes func(),
    remember that only one thread should access func() at time.
    b. the thread that reaches func() 1st, performs "mutex.lock()" (locks the door) 
    c. the 2nd thread the encouters "mutex.lock()" sees that the mutex (door) is locked,
    thus it waits (blocked) until the mutex (door) is unlocked.
    d. at the end of func(), 1st thread executes "mutex.unlock()" (unlocks the door).
    c. the 2nd (blocked) thread notices the mutex (door) is unlocked and continues to execute
    func's statements
    notes:
    i. The mutex should be a global object - to allow both threads to lock and unlock the same door
    (threads share the same global variable of the process)
    ii. critical section - a section of code that multiple threads "compete" to execute - 
    concurrent accesses to shared resources such as std::out can lead to unexpected or erroneous behavior.
    iii. instead of "mutex.lock()" threads can execute "if(mutex.try_lock()) else..." - if a thread sees
    that the mutex (door) is already locked by another thread, it won't wait for the mutex (door)
    to unlock and continue "else". Or if(mutex.try_lock()_for(std::chrono::seconds(3)) else... - 
    ultimatum - the thread will wait no more then 3 seconds for the mutex (door) to unlock, after
    that the thread will continue to do something else.

4. the followings are wrappers around a mutex. they offer the same abillities as mutex
+ their dtors unlocks the mutex automatically (when leaving a scope).
"std::lock_guard<std::mutex> lock(m);" where m is "std::mutex m:
automates unlocking when leaving the function.
RAII wrapper to mutex. "m" is global. when "lock" is instantiated in func(), the mutex is locked,
and when leaving func(), the dtor of "lock" unlocks the mutex.
std::unique_lock offers additional options - in cost of performance:
a. when no arguments : "std::unique_lock<std::mutex> lock(mutex);" //equivalent to std::lock_guard<std::mutex> lock(mutex);
b. std::unique_lock<std::mutex> lock(mutex, std::defer_lock); //
locking / locking is deferred to us, we can manually execute "lock.lock()/unlock() whenever we want "
c. std::unique_lock<std::mutex> lock(mutex, std::try_to_lock); (analgous mutex.try_lock): 
if (lock.owns_lock()) ...else...
d. std::unique_lock<std::mutex> lock(mutex, std::try_to_lock_for); (same as try_lock_for)
if (lock.try_to_lock_for(std::chrono::seconds(4)))... else... 

5. conditional variables - where t2's continuation is dependant some condition (i.e if...)
e.g: t1 notifies t2 that it should check the predicate of the cv. this predicate
(func returning bool) queryies if some global variable was indeed modified, and if it did,
t2 can continue. check the example below , where t1 wants to withdraw_money() via "money-=500;",
but performs "cv.wait()" to wait  for a "cv.notify()" (notification) from t2 add_money() i.e deploys money+=500;
This notification causes t2 to check  the condition (predicate) of cv (return true if money>0) and thus it continues
to withdraw 500;

6. Future and promise: enables t1 to pass information, to waitiing t2 thread without global variables.
as opposed to conditional variables, where the thread evaluates a condition, to check if it should continue.
a promise is an object "moved" to a thread, hoping the thread,
will "fulfill" the promise, i.e will set a value to the promise. since values (promises)
are set in the future, the values are stored in a future object, and a thread is blocked
until the promise is fulfilled.
code:
void perform_task(std::promise<int>&& promise) { promise.set_value(10);}
in main....
std::promise<int> promise; //promise to be fulfilled (value to be set) by another thread
std::future<int> future = promise.get_future(); // promises are fulfilled in the future thus we store the answer in a future obj
std::thread t(perform_task, std::move(promise)); //promises MUST BE MOVED to thread that will fulfill the promise.
int promised_value = future.get(); // blocks the current thread until promise is fulfilled, will equal 10;

7. std::async - automates setup of promise, and thread creation. (no promise need to be moved, more natural syntax)
bool is_even(int num) { return (num % 2) == 0;} //promise fulfileld by simple return, no need for promise arg
in main...
std::future<bool> future = std::async( is_even, 4); //creats a thread, a promise, and links it to a future obj.
bool returned_value = future.get(); // true - future.get() can appear only once - crash otherwise!




Notes:
1. the loading of each file can occur on each seperate thread (saving performance), but the insertion
to a shared global vector should occur via mutex

2. Locking Granularity:
- Fine-grained lock: protects small amount of data, but more risk of dead lock, trickier programming
- Coarse-grained lock: protects big amount of data, but we lose opportunity for more parallelism.
lock should be not too fine and not too course.
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
    /*
    the mutex goal is to synchronize the access of threads to the critical section that consists
    of std::cout and money which are global variables - if both threads will manipulate
    them at the same time unexpected behaviour will occur.

    cv.wait will cause the thread to "wait" for a notification, essentially, the thread
    will sleep until it will be notified to wakeup and check the predicate.
    The problem is that the thread sleeps while the mutex is locked, therefore
    the access to the function is blocked for no reason and the mutex seemingly will be locked
    for ever. thus, the cv mechanism unlocks the mutex while it waits for notification
    (granting access for other threads), and will lock the mutex only when it is waken up.
    once awake, the thread will wakeup and check the prdicate (2nd parameter - a function that retuns a bool)
    and only if it returns true, the thread will continue continue to execute the following statements,
    otherwise if the predicate returns false - it will unlock the mutex and keep on waiting.
    Note: when the thread wakes up and finds that the condition is false - this is called
    a spurious wakeup.
    We can also use cv.wait_for for a duration, after it will it unlock the mutex.

    ********************************************************
    conditional variables can help us determine the order of execution between threads.
    ********************************************************
    */
    std::unique_lock<std::mutex> lock(mutex);
    // predicate seems as a must! without it we may miss a wake up or a spurious wake up can occur
    cv.wait(lock, []() {return money > 0;}); 
    money -= 500;
    std::cout << "money withdrawn: " << money << '\n';

    /*
     inefficient alternative to "wait":
     1. A while
     which constantly  checks the same predicate - this causes redundant
     looping and perforamnce hit
     2. std::this_thread::sleep_for(std::chorno::seconds(?)) but it is hard
     to estimate how much time the thread needs to sleep to wait for the other
     thread to get the job done. conditional variables solve this
     because they get notified.
    */
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
    by the fact that we need to choose a relationship between them - in the next step.-*/
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

    /*the number of threads  we create at most must equal to the number of physical cores
    on the cpu. Creating more threads then available cores is called oversubscription
    and it is not efficient
    returns number of logical cores (physical cores * how many threads each core can run)
    this is the number of cores the cpu actually sees.*/
    std::cout << "number of cores: " << std::thread::hardware_concurrency() / 2 << '\n';

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
    THIS CAN ONLY HAPPEN WHEN BOTH THREADS GOVERN THE SAME GLOBAL MUTEX OBJECT, i.e
    since global vars of process are shared among threads that live in that process,
    thus the global mutex is known by both threads - they both aware of its status
    and can lock/unlock it: 
    before entering the critical section the thread must lock access to the resource to 
    use it exclusively and at the end, the thread should unlock the mutex to allow other threads to do the same thing.
    *****************************************************************************************
    Like a bathroom, where each person(thread) locks the door (mutex) when entering and
    unlocks the door (mutex) when leaving allowing other people (threads) to come in and do the same thing,
    all manipulate the same mutex (key-door), synchronizing their order for entering the bathroom
    (order of exeution of the ciritical section).
    *****************************************************************************************
    
    What happens of the same thread locks an already locked mutex?
    the thread sleeps (following statements will not be executed),
    and waits until the mutex is unlocked. When the mutex becomes unlocked, the thread
    wakes up and locks the thread again.
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
        The thread that performs a "lock()" on the critical section gains exclusive
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
        in addition we can unlock the mutex manually, and lock it again just like a regular std::mutex.
        just like lock_guard, the death of the  unique_lock will unlock the mutex.
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
        try_lock_until. 
        
        unique_lock has more options than lock_guard, but it comes at a price of performance:
        unique_lock is more demanding than lock_guard.
        */
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
        itself, so no one can access the critical section, until thread wakes up and unlocks the mutex.
        we can use  std::this_thread::sleep_for(std::chrono::seconds(5)) to make a thread goes to sleep.
        in itself, the sleep function will not unlock the mutex, we can do this manually if we want
        b. Waiting Thread: Waiting threads will wake up only when other threads notify them to this via
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



