/*
exceptions decouple error returning from the true purpose of a function:
functions should have one job!!! - assume a function getfile() that requests the user to type a file name,
and the function returns the file. if the file doesn't exist - is it getfile()'s job to handle the execption / request
to retype a filename? probably not because getfile() just gets a file. We can return an int
error code, but this "breaks" the purpose of the function and the return value that needs to be a file type,
not an int to support error codes.

therefore the exception mechanism separates the job of the function, done at the body (try block),
from the error reporting that can be handled in catch block, (e.g: requesting a name retype in catch handler).
The return type can still be a file, which is independant from the type of error we can throw

BUT if we can return a null pointer, or a status code to indicate failure, that’s probably the better choice -
because its simpler and more performant. otherwise, an exception would be reasonable.
    


Exeptions tl;dr
1. try-catch blocks always appear together - there is no try without catch and vice-versa.
2. exception is raised using "throw X;" - X is the error msg/object/literal:
    a. if the throw is inside the try block of try-catch block - the catch block (excecption handler)
    that matches the argument of the expection catches the error. catch handlers are tested subsequently in order for
    matching parameter.
    b. if no appropriate catch handlers exist in the current try block that can accpet the argument of the exception,
    or if the throw is outside of a try-catch block,
    or it's inside a catch block itself,
    then the exception propagtes up the stack to the nearest catch in the nearest try-catch block up the stack of callers.
    c. if no appropriate catch handlers exist at all then the program will fail with an expcetion error
    (propgated up the stack until the end of the program - up until main)

3. compilers won't perform implicit type conversions when matching an exception to a catch block:
e.g - const char* exception won't be implicitly converted to a catch that accepts std::string,  makes
sense since we may want very specific exceptions to be caught be very specific catch

4. throwing and catching and exception does not cause the program to quit, it simply quits the current
try block when the error is caught, and the compiler continues to the next statement in the current
scope after the try-catch blocks.

NOTE: exceptions SHOULD BE CAUGHT in catch block BY REF/PTR for 2 reasons:
for example throwing a derived object and catching a base by value will cause the following:
    a. redundant copies -  of base section of derived into the base parameter of catch handler
    b. not using ref or ptr will cause slicing -
    throwing a Cat object as an error and catching it via Animal instead of Animal& will destroy polymorphism -
    in practice we will lose access to the virtual functions of Animal that Cat overrides!
    so if Cat overrides the virtual function  jump() thqt Animal has, executing
    jump() will execute the jump of the animal, and not the jump() function of the cat,
    because the Cat part of the object passed to the animal type has been "sliced"
    (the vtable to possessing the pointers to the correct functions has been lost - no virutal
    functions will be supported).
    In contrast throwing a Cat object caught by Animal& and executing cat.jump()
    will execute the jump() of the cat due to polymorphism supported only by references
    and pointers in cpp.




*/

#include <iostream>
#include <cmath>
#include <string>
#include <exception>
#include <sstream>
#include <string_view> //only in c++17
#include <utility>

int my_sqrt(double x)
{
    if (x < 0.0)
        throw "cannot take sqrt of a negative\n"; //NOTE: func return value is not related to type of throw!
    //this is a big adventage! decoupling of return value from the actual error type being thrown!
    //e.g: function get_index returns needs to return an int, but errors can be thrown as text!
    return sqrt(x);
}

void run_game()
{
    throw "game crashed";
}


void start_game()
{
    try
    {
        run_game(); // throws an error "game crahsed"
    }
    catch (const char* error) // catching "game crashed"
    {
        throw error; //rethrowing the error back up the stack so it will be caught in main
        //in this way we tell the caller in main that something went wrong and he can do something
        //about the situation. Without rethrowing th error back to main,
        //knowing what happeneed here.the program just returns to main and continues without
    }
}


/*********classes - construction fail*******/
class Engine
{
public:
    int m_cylinders;
    Engine(int cylinders) : m_cylinders{ cylinders } {};
    ~Engine() { std::cout << "Engine dtor\n"; }
};

class Car
{
public:
    Engine m_engine;
    int m_wheels;
    Car() : m_engine{ 4 }, m_wheels{ 0 } {}
	Car(int wheels) : m_engine{ 2 }, m_wheels{ wheels } { throw 1; }
    ~Car() { std::cout << "Car dtor\n"; }
    void erroring() { throw "class error\n"; }
};


/*********classes - Derived is A Base*******/
class Animal
{
public:
    int m_legs;
    Animal() : m_legs{ 4 } { std::cout << "animal ctor\n"; }
    virtual void print() { std::cout << "i'm an animal\n"; }
    ~Animal() { std::cout << "Animal dtor\n"; }
};

class Cat : public Animal
{
public:
    bool m_fur;
    Cat() try : Animal{}, m_fur{ true } //note:try-catch function block!
        //without it, all errors throwed during construction of Cat or Animal
        //will be caught in main
    {
        throw 1;
    }
    catch (int& exception)
    {
        //note: if an exception isn't explicitly thrown here, the current exception
        //will be implictly rethrown here back to main
        //and will act the same as throw;
    }

    void print() override { std::cout << "i'm a cat\n"; }
    ~Cat() { std::cout << "Cat dtor\n"; }
};


/*********deriving from class std::exception*******/
class IndexException : public std::exception
{
public:
    std::string m_error{}; // construct it here, or in the member initializer list
    IndexException(std::string_view error) : m_error{ error } {} // string_view is a view to a std::string
    // overriding virtual function what() of std::exception
    //to return our m_error{} as const char* 
    const char* what() const noexcept override {return m_error.c_str(); } 
};

class Array
{
public:
    //const static Animal a();
    const static int x = 5;
    int m_data[5]{};
    Array() : 
        m_data()
    {}
    int& operator[] (const int index)
    {
        if (index < 0 || index > 5)
        {
            std::stringstream ss{};
            ss << "index " << index << "out of range\n"; //best way to concatenate a string 
            //alterantive: 3 different concatenated std::strings for each subexpression
            throw IndexException(ss.str()); //construct the error class defined above and throw it
        }
            
        return m_data[index]; // if index is fine, return the value at that index
    }
};




int main()
{


    /*********basic try catch*******/
    try
    {
        throw "basic try-catch\n"; //exception raised using throw - execution 
        std::cout << "hello"; // won't get printed

    }
    catch (const char* exception) // always pass by ptr/ref to avoid unecessary copies
    //const char* (and not std::string) because implicit type conversions don't happen in try-catch
    //to enable catch of accurate types
    {
        std::cerr << exception ;
    }
    //the program will continue after the "catch" above

    

    /*********stack unwinding try-catch (throw at a function, caught here)*******/
    try
    {
        my_sqrt(-2);
        //note: a "throw" can be anywhere at the program, as long as it was invoked from a try-catch block
        //for example, there is a "throw" inside my_sqrt, a function which doesn't contain a try-catch block,
        //but my_sqrt itself was called from a try-catch block, therefore, via stack unwinding,
        //the throw is caught here.

        //a throw cannot exist if orignally the function/class we are throwing from wasn't created/called
        //from a try catch block - otherwise a catch will not occur which will lead to an error!
    }
    catch (const char* exception)
    {
        std::cerr << exception;
    }

    /*********uncaught exception will end the program******
    //raises run-time error
    try
    {
        throw "this won't get caught - no appropriate exception handler for const char*";
    }
    catch(int exception) // catches only ints, not c style strings
    {
        std::cerr << "test";
    }
    */

    /*********catch-all handler*******/
    try
    {
        run_game();
    }
    catch (double x) //won't get caught
    {
        std::cerr << "Abnormal termination\n";
    }
    catch (...) //mechanism to catch all types of exception.
    {
        std::cout << "catch all\n"; //"game crashed" thrown error is caught here, 
        //but since catch-all handler doesn't catch an argument, we print a general error
    }
    //saveState(); // if game crahsed, catch is done, the program continues, and we save the game
    //before program termination from main



    /*********classes - error thrown in methods*******/
    //throws in class methods works exactly as in normal (non-class) functions
    try
    {
        Car e;
        e.erroring();
    }
    catch (const char* exception)
    {
        std::cerr << exception;
    }


    /*********classes - construction fail*******/
    std::cout << "classes - construction fail\n";
    //failure occurs in Car ctor (throws an error) therefore Car's construction won't complete
    //thus its dtor won't run since there is nothing to destroy (the object wasn't built)!
    //since the throw is inside the ctor's body, this means we already constructed all class members of Car
    //including Engine member object: therefore, IMMEDIATELY AFTER THE THROW statment(BEFORE EVEN GOING TO THE CATCH HANDLER)
    //the compiler destroys all members, thus the dtor of Engine executes. Only after the destruction of all-already
    //constructed members, the compilers continues to the catch handler
    //This also apply in inheritance! if we instantiate a derived object and derived or base
    //ctor throw an exception then derived won't be constructed (therefore its dtor won't execute)
    //and its members, including those that were inherited from base will execute their own dtor to ensure
    //that no memory is leaking.
    //The same also applies to copy
    try
    {
        Cat cat; //the body of cat's ctor throws an exception. cat construction fails
        //therefore it isn't built (thus no need for dtor of cat) but Animal was constructed
        //in the member initializer list, therefore the compiler ensures that dtor of Animal object
        //does run to prevent memory leaks.
    }
    catch (int& error)
    {
        std::cout << "cat failed\n";
    }

    /*********classes - Derived is A Base*******/
    try
    {
        throw Cat{};
    }
    catch (Animal& animal) // activated! since Cat is an Animal! to catch a Cat we can flip the catch handlers
        //so that Cat handler will be before Animal handler
    {
        std::cout << "caught animal\n";
    }
    catch (Cat& Cat)
    {
        std::cout << "caught cat\n";
    }


    /*********class std::exception*******/
    //as of c++17 25 exception classes exist - all inherit from std::exception.
    //failed dynamic cast throws std::bad_cast which is-a std::exception that we can catch
    //(by ref - to avoid copy and infer the c-style string  by the virtual function expcetion.what()
    //that derived error classes override)
    //WE CAN ASLO THROW THESE ERRORS IF THEY ARE ADEQUATE TO OUR SPECIFIC CASE with a custom message
    try
    {
        throw std::overflow_error("overflow occured\n");
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what();
    }


    /*********deriving from class std::exception*******/
    try
    {
        Array arr;
        arr[6]; //invokes IndexException error (inherting from std::exception) 
        //from operator[] "index out of bound", trying to get 6th index out 5 array
    }
    catch (std::exception& exception) //use & to ensure polymorphism and avoid slicing (we access
        //what() of IndexException class and not what of std::exception class)
    {
        std::cout << exception.what(); // prints our custom msg: "index 6 out of range"
    }



    /***********throw; keyword - without parameters***************/
    try
    {
        try
        {
            throw Cat{};
        }
        catch (Animal& animal) //ok since cat is-a animal and catching by ref preserves
            //polymorphism(avoid slicing), and prevents redundant copies.
        {
            //BUT!!! rethrowing the caught object via "throw animal;" creates a copy-initialized copy of the thrown object -
            //meaning we create an entire new object only with a Animal portion (although the compiler may elide this copy,
            //meaning the caller will directly recieve the base reference object) - this causes slicing 
            //which destroyes  polymorphism (animal won't have a pointer to the vtable of cat)*********
            throw animal;

            //throw; better alternative!!!!!! 
            //this will throw the exact same polymorphic animal reference that caught by this catch-handler.
            //note: the animal reference in the parameter is an Animal type object that 
            //referes to the Animal portion of the thrown Cat object (which contains animal data members + pointer to vtable
            //of Cat class that points to functions that Cat has overriden, such as print() (executing animal.print()
            //will execute print of Cat)
        }
    }
    catch (Animal& animal)
    {

    }



    /**********exception safety guarantees************/
    /*
    a conctractual guidline about how functions or classes behave during exception:

    throwing guarantees:

        1. No guarantee - exceptions can be thrown and a class may be left in unstable state
        2. Basic guarantee - after exception throw, object will be stable, but the program may be in a modifiable state
        (side effects - effecting external objects outside the environemnt of the object itself)
        3. Strong guarantee - after thrown expcetion - no memory will be leaked, the program will not be changed,
        without side effects. everything will rolled back to the point of pre-failure.


    non-throwing guarantees:

        1. the no-fail guarantee: function will always sucessed in what it tries to do,
        thus it will never throw an exception (apply for move ctors and move assignemnts, and more) -
        this why vs studio requires explicit noexcept on move ctors

        2. the no-throw guarantee: weaker version of no-fail gurantee, function CAN fail, but like
        no-fail guarantee, it won't throw an exception, but rather an error code/ignore the problem
        (dtors or memory deallcoates/clean up functions)
    */


    /***********noexcept specifier***************/

    /*
    note: noexcept ctors/functions implicitly meet the no-fail guarantee: the programmer promises that
    the functions will always succeed and no exceptions will be thrown.

    Every function in C++ is either non-throwing or potentially throwing:
    assume: void do smething() noexcept{} ;
    1. noexcept(true) same as noexcept - "non-throwing" - throwing an exception through something()
    will invoke std::terminate, and calling std::terminate through noexcept function might 
    not cause stack unwinding, thus object clean up may not occur before termination - the same applies
    for constructors marked as noexcept - a "throw" inside them will invoke std::terminate (error msg on screen)

    The following are implicitly non-throwing "noexcept(true)" by default, can be transformed
    to potentially throwing by "noexcept(false)":
    IMPLICITLY DECLARED constructors, copy constructors, move constructors, destructors.
    implicitly declared means that user hasn't defined them, the compiler implictly declared them,
    and the compiler will define them at moment of construction

     *if these functions contain another function calls that are potentailly throwing (noexcept(false)
    - then the functions above will also become potentially throwing - this can be avoided by
    marking the functions above as EXPLICITLY non-throwing (noexcept(true))


    2."noexcept(false)" - "potentially-throwing", "noexcept(false)" means "potentially throwing".
    The following are potentially throwing (noexcept(false) by default, can be
    transformed to non-throwing by "noexcept(true)":
    Normal functions
    User-defined constructors
    Some operators, such as new copy assignment operators, move assignment operators.

   
    noexcept(true) & noexcept(false) can be used to override functions or in function templating
    where a template boolean non-type parameter can be used to dynamically create a
    potentially throwing/non-throwing function.

    noexcept specifier can also be used as an operator inside functions to evaluate at compile-time (static)
    if an expression evaluates to an exception
    void foo() {throw -1;}
    void boo() {noexcept (foo());} // noexcept(foo()) returns true
    void goo() {noexcept(3+5);} //false ints are non-throwing

    
    *******NOEXCEPT(TRUE) FUNCTIONS allow the compiler to perform optimizations -
    the compiler doesn't have to worry about keeping the runtime stack in an unwindable state.
    */





    /***********move_if_noexcept specifier***************/
    /*
    assume we are trying to initialize an std::pair by stealing resources from an existing std::pair (move):
    std::pair my_pair {std::move(another_pair)}; // std::pair contains built-in move ctor
    *template parameters "<>" are not necessary - cpp17 supports template deduction

    Assume "my_pair" and "another_pair" both contain two classes, A & B that both support move ctors,
    std::move(another_pair) invokes the move ctor of std::pair, which in turn invokes the move ctors
    of each class.

    Suppose the follwing:
    1. my_pair is being move constructed via another_pair
    2. class A of my_pair is successfully built (its move ctor went through the job) - this leaves A of another_pair
    in a modified state, albeit a good move ctor ensures that the stolen object will be left in a good null state.
    3. my_pair's class B is "moved constructed" via another_pair's class B but THE MOVE CONSTRUCTOR
    FAILS (throws expcetion) right before it finishes (although another_pair B already is stolen - left modified)
    thus B is not fully constructed due to the throw.

    IMMEDIATELY after the throw statement, the compiler now begins to destroy the failed construction of my_pair.
    until now only class A of my_pair was successfully constructed (class B and my_pair itself were not fully constructed
    therefore we don't need to destroy them), thus the destructor of class A executes and deletes all pointers/resources
    that class A of my_pair holds.

    This is bad because another_pair's class A & B were stolen and left in a null state (damaged),
    and my_pair wasn't constructed, therefore we lost both pair objects. THIS WOULD HAVE BEEN FINE
    IF "another_pair" was a temporary object that would have been discarded anyway, but it may be used
    by other objects, and now its in a bad state.
    to comply with the STRONG EXCEPTION GUARANTEE (after exception - no memory leak, everything is reverted)
    we must return the resources stolen back to another_pair class A & B - this is probably possible,
    but there is another alternative:

    instead of:
    std::pair my_pair {std::move(another_pair)};
    we perform
    std::pair my_pair {std::move_if_noexcept(another_pair)};
    
    what move_if_noexcept does is seek if the move ctor of std::pair is noexcept: if it does - then it
    means that the move ctor meets the no-fail guarantee (no memory leaks, everything reverted after exception)
    and will always succeed. Note that classes A and/or B can be throwing (without noexcept)  or non-throwing (with noexcept), 
    in either case if they throw an exception, because  std::pair is noexcept then the program would ABORT.

    **********since std::pair move ctor is NOT marked as noexcept, then std::move_if_noexcept will invoke 
    THE COPY CTOR OF std::pair AND THUS INVOKE THE COPY CTORS OF CLASS A AND B*****************
    this happens because the compiler cannot be sure that the non-noexcept move ctor would succeed. A fail
    may leave the stolen objects damaged. if the move ctor of std::pair was marked noexcept, then 
    the compiler would have used it since then we promise the no-fail guarantee - the move will succefully
    occur, and we wouldn't be left in a state where the stolen objects are in a null-state and the newly created
    object isn't created due to a failure, thus losing the resources from the stolen object.

    therefore in this state where std::pair move ctor is not noexcept (potentially throwing, not guaranteed to succeed)
    the compiler invokes the copy ctor of std::pair because copy ctors are much safer
    (although they are slower the move ctors), they cannot harm the copied objects because copy ctors by nature don't modify the
    copied objects (they are const), we just copy them, unlike move ctors which steal (reroute) resources and leave
    the stolen objects in a null state (ptrs members are nullified and pod's are set to 0...).

    */



    /****************notes:************
    1. when declaring a resource (e.g: a ptr (preferring smart ptr) to a file), we should declare it outside the try-catch block
    so that if it will be available to the scope of both the try and catch where we could delete the ptr
    if something goes wrong. 
    2. throwing error in dtor may cause the program to crash -  because the dtor is now
    competing with its stack unwiding and also with the exception being met which also requires stack unwiding.
    therefore default destructors are noexcept.
    3. exceptions do come with a performance price (stack manipulation, bigger executable, passing parameters..)
    4. Some modern computer architectures support an exception model called zero-cost exceptions. 
    Zero-cost exceptions, if supported, have no additional runtime cost in the non-error cast
    */

}