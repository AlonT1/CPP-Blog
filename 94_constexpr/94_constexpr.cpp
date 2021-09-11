#include <iostream>
#include "Header.h"

int main()
{
    /**********const*/
    const int test = 4; //compile-time constant - offers optimizations!

    int age = 0;
    std::cin >> age;
    const int y = age; // run-time constant!

    /**********constexpr*/
    //so const is for compile-time and run-time constants. constexpr disambiguates this
    //by being fit only for compile-time constants
    //constexpr int z = age; error! only for compile-time constant


    /**********extern*/
    //defining a variable x in a header and including it in multiple files, still obeys ODR (one definition rule)
    //because x is now
    //correct use: declare x in header, define x in other.cpp (without include of the header), main.cpp includes
    //the header and is able to use it.
}

