#include <iostream>

//extern int globalVariable; tells the compiler that globalVariable exists and is defined elsewhere.
extern int globalVariable; // Declaration only, no storage allocated here


int& function1()
{
    static int var = 4; // var will not die until the end of the program
    //calling function1 multiple times WILL NOT redifine var.
    //it is defined ONLY ONCE at the first call. it will reside in the static
    //section of memory
}

int main()
{
    int x = 2; //storage class specifier controls: storage duration + linkage in cpp storage duration is implicitly auto by default (block scope, local)
    // in cpp 11 auto means a deduced type
}

