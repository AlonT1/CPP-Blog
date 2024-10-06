// C++ code to demonstrate bind() and
// placeholders
#include <iostream>
#include <functional> // for bind()

// Driver function to demonstrate bind()
void func(int a, int b, int c)
{
    std::cout << (a - b - c) << std::endl;
}

struct Foo
{
    void display_greeting()
    {
        std::cout << "Hello from Foo.\n";
    }

    int add_xy(int x, int y)
    {
        return x + y;
    }
};

struct Doo
{
    void display_greeting()
    {
        std::cout << "Hello from Doo.\n";
    }
};


int main()
{
    //https://www.geeksforgeeks.org/bind-function-placeholders-c/
    
    //********************std::bind
    // std::bind returns a function object based on my_func, but with its arguments bound to args (8,2,3).
    //  equivalent to: 
    //  std::_Bind<void (*(int, int, int))(int, int, int)> my_func = std::bind(func, 8, 2, 3);
    // (*(int, int, int))(int, int, int): (unamed) is a function (int, int,int) returning a pointer
    // to a function (int, int, int) that returns a void/
    auto my_func = std::bind(func, 8, 2, 3);
    
    my_func(); // invoking the function

    //  bind also works with member functions!!!!!  the first parameters is as always a pointer (address of
    // returns a pointer to a member function), and the 2nd param is the implicitly the "this" pointer
    // that every member function takes as the first implicit argument.
    Foo my_foo;
    // equivalent to: std::_Bind<void (Foo::*(Foo *))()> class_bind = std::bind(&Foo::display_greeting, &my_foo);
    // unnamed is a pointer to Foo that point is a pointer to a class member of Foo that is a function 
    // accepting 0 parametrs and returns void
    auto class_bind = std::bind(&Foo::display_greeting, &my_foo);
    class_bind();


    //********************std::placeholders
    // using placeholders: The std::placeholders namespace contains the placeholder objects [_1, . . . _N],
    // they direct the position of a value in a function.
    // When used as an argument in a std::bind expression, the placeholder objects are stored in the generated function object
    // and when that function object is invoked with unbound arguments, 
    // each placeholder _N is replaced by the corresponding Nth unbound argument.
    
    // std::_Bind<void (*(std::_Placeholder<1>, int, int))(int, int, int)> my_func = std::bind(func, std::placeholders::_1, 2, 3)
    auto fn1 = bind(func, std::placeholders::_1, 2, 3); // 2,3 are "FIXED" for b,c. only a accpets input
    fn1(10); // accepts only 10 (a=10), result: 10-2-3 = 5

    using namespace std::placeholders; // we can use "using delcerative" which inserts the namespace here
    auto fn2 = bind(func, 2, _1, _2); // 2 is fixed for a. only b and c accpets input.

    fn2(10,20); // result: 2-10-20 = -28


    //***************std::mem_fn - alternative to "bind to member function" - although bind works well as shown above!
    // Function template std::mem_fn generates wrapper objects for pointers to members,
    // which can store, copy, and invoke a pointer to member. Both references and pointers 
    // (including smart pointers) to an object can be used when invoking a std::mem_fn.

    // bind works with normal functions (classic), mem_fn is like bind but for methods (member functions).
    auto greet = std::mem_fn(&Foo::display_greeting); // bind greet to the address of member function "display_greeting).
    // equivalent to:   std::_Mem_fn<void (Foo::*)()> greet = std::mem_fn(&Foo::display_greeting);
    // unnamed is a pointer to a class member Foo that is a function accepting no parameters and returns void
    Foo foo; // equivalent to:   Foo foo = Foo();
    greet(foo); // the "greet object must accept an instance of the class that hold the mem func the we binded to
    //equivalent to:   greet.operator()(foo);

    auto add_xy = std::mem_fn(&Foo::add_xy); // equivalent: std::_Mem_fn<int (Foo::*)(int, int)> add_xy = std::mem_fn(&Foo::add_xy);
    std::cout << "add_xy: " << add_xy(foo, 1, 2) << '\n'; //the first parameter is always the function we bind to

}