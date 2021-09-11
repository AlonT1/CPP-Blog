#include <iostream>
#include <vector>
#include <algorithm>

/*
functor (or function object) are classes that operate like functions - 
this is done by a class which overloads operator(). Therefore when 
instantiating a class "entity" and invoking "entity();" this looks like a "regular"
function call, but we actually invoked the overloaded operator() function of entity.

The advantage of a functor over a normal function?

1. functors store the state of a data and are persistent:
calling accumulator() below increases m_counter each time, and we preserve the state
of m_counter between calls.

2. we can have multiple Accumulator (functor) objects, while we can have only 1 function
*/



int main()
{
    //************simple example - Accumulator************
    class Accumulator
    {
    public:
        int m_counter{ 0 }; //default member initialization
        //overloading operator()  (emulating a function call)
        int operator() (int i = 1) { return m_counter += i; }
    };


    Accumulator accumulator;
    accumulator(); //increase m_counter by 1
    accumulator(20); //increase m_counter by 20


    //************functors with stl************
    std::vector<int> v{ 1,2,3,4,5 };
    class Printer
    {
    public:
        void operator() (int value) { std::cout << value << ' '; }
    };
    Printer p;
    for_each(v.begin(), v.end(), p);
}
