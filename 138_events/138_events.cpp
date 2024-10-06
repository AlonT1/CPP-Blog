#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <any>
#include <tuple>

// see function_pointers and multicast_delegates
// events in C# are like delegates (objects that functions can subscribe to, calling the
// delegate invokes the functions). the difference is that in general, events are more restrictive e.g:
// 1. with delegates the following scenario is possible:
// MyDelegate myDelegate = MethodA;
// myDelegate += MethodB;
// myDelegate(); // invokes both MethodA and Method B
// myDelegate = MethodA;
// myDelegate(); // ONLY MethodA IS INVOKED!!!
// Note: Delegates can also be subscribed (+= or -=) to another delegate e.g:  myDelegate = delegate1 + delegate2;
// where both delegate1 and delegate2 point to a function.
// *************************************************************************
// with events we can only subscribe and unsubscribe! no assignment is possible, which avoids
// overwrriting all other methods/delegates which are already subscribed to myDelegate.
// *************************************************************************

//2. only the class that owns the event can invoke it, as opposed to delegates, where
// all classes can reach out to them and invoke them.
// this makes sense since the whole concept behind events is that the instance of a class
// that originally hold the event should be the one who invokes, and this notifies(fires) the functions in
// other classes that has subscribed to this event.
// https://www.youtube.com/watch?v=TdiN18PR4zk



// https://stackoverflow.com/a/47914094
template <typename R, typename X>
class open_mem_fun 
{
public:
    R(X::* fn)(); // fn = pointer to a member of class X that is a func accepting 0 params returning R
    open_mem_fun( R(X::* fn)() ) : fn(fn) {} // assigning fn to incoming fn

    template <typename T>
    R operator()(T* p) const 
    {
        return (dynamic_cast<X&>(*p).*fn)();
    }
};

template <typename R, typename X>
open_mem_fun<R, X> open_mem_fn(R(X::* fn)()) 
{
    return open_mem_fun<R, X>(fn);
}


class Event
{
public:
    virtual void operator()() = 0;
};

struct Foo
{
    void method_a(const std::string& message)
    {
        std::cout << "methodA " + message + '\n';
    }
};

struct Doo
{
    void method_b(const std::string& message)
    {
        std::cout << "methodB " + message + '\n';
    }
};


struct Koo
{
    void method_a()
    {
        std::cout << "Koo methodA " + '\n';
    }
};

struct Woo
{
    void method_b()
    {
        std::cout << "Woo methodB " + '\n';
    }
};

template <typename R>
struct Event2
{
    using pair = std::pair<std::any, std::any>;
    std::vector<pair> subscribers;

    template <typename C>
    void operator += (R(C::* fn)())
    {
        subscribers.push_back(std::make_pair<std::any, std::any>(fn, C{}));
        
    }

    void operator()()
    {
        for (pair subscriber : subscribers)
        {
            std::any fp = subscriber.first;
            std::any c = subscriber.second;

            std::cout << fp.type().name();
            // HOW CAN I KNOW THE TYPE OF FP????????????? for conversion purposes
        }
    }
};



int main()
{
    /*
    void(Foo:: * func_ptr1)(const std::string&) = &Foo::method_a;
    Foo foo;
    // (foo.*func_ptr1)("shalom");

    void(Doo:: * func_ptr2)(const std::string&) = &Doo::method_b;
    Doo doo;
    // (doo.*func_ptr2)("shalom");

    std::vector<std::any> my_vector;
    my_vector.push_back(func_ptr1);
    my_vector.push_back(func_ptr2);

    void(Foo::* ptr)(const std::string&) = std::any_cast<void(Foo::*)(const std::string&)>(my_vector[0]);
    (foo.*ptr)("shalom");

    func<int>(5);
    */

    Event2<void> ev;
    //ev += &Woo::method_b;
    //ev();

    //std::vector<void
    // open_mem_fun<void, Koo> //

    // std::function<void(Base*)>(open_mem_fn(&X::XFunc));

    //return type, parameters, class name, instance
}