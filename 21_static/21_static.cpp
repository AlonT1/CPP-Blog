#include <iostream>
//extern - external linkage - look for s_Var in external translation units
//extern int s_var;
extern int t_var;
//can compile since function() in static.cpp is static, preventing double symbol error


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

    std::cout << t_var << std::endl;

    //static in a function means infinite lifetime (as opposed to scoped lifetime)
    //and one time initialization
    static int s_variable = 5;
}

