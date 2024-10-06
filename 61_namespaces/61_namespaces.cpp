/*
* A namespace is a declarative region that provides a scope to the identifiers 
(the names of types, functions, variables, etc) inside it.
Namespaces are used to organize code into logical groups and to
prevent name collisions that can occur especially when your code base includes multiple libraries.
namespaces - exist to avoid naming conflicts, aids in encapsulation. helps distinguishing 
identical symbols (function/variables..) by using different namespaces
in glfw which is a c library, all functions have glfw prepended to function names (c doesn't support namespaces!!!!) 
to emphasize that a certain function belongs specifically to the glfw library.
in cpp we can use namespaces to discern between namespaces, especially if two libraries have the same function
name such as init is very common in many libraries - namespaces solve this issue!
:: scope resolution operator
in SERIOUS projects namespaces should be used for organization, and avoiding conflicts.
"using namespace" should be confined to the smallest scope possible - never in header files
a class is a type of namespace - visible in class enums
*/

#include <iostream>

namespace a
{
    void print()
    {
        std::cout << "a" << std::endl;
    }
}

namespace b
{
    void print()
    {
        std::cout << "b" << std::endl;
    }
}

//namespaces can be nested
namespace c
{
    namespace functions
    {
        void hello()
        {

        }
    }
}

//alternative to above, starting from c++17
//namespace c::functions
//{
//
//}


enum class Colors
{
    RED,BLUE,GREEN
};

int main()
{
    //two identical symbols, distinguished by different namespaces
    a::print();
    b::print();
    std::cout << "Hello World!\n";
    c::functions::hello();

    Colors::BLUE; // class enums and regular enuams are also namespaces 

    //instead of c::functions::hello(), we can use two "using namespace"
    using namespace c;
    using namespace functions;
    hello();
}

