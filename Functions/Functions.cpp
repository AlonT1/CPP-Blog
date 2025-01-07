#include <iostream>


/*************************forward declaration*************************/

int multiply(int a, int b); //forward decleration - this function exists somewhere / signature
//equals to extern int multiply(int a, int b); non static vars and functions are extern by default!!!

//the name of the parameter (identifier) isn't required in forward decleration or in definiton
//we provide an identifier only we want to refer to the variable. sometimes we don't need to
//provide an identifier, for example when want to differentiate between two overloads:
int perform_this(int) {}
int perform_this(int, int) {}

//int- return value, multiply - function name, (int a, int b) - parameters
int multiply(int a, int b) //definition
{
    return a * b; //statement
}


/*************************function overloading************************

function overloading  - when two or more functions have the same name, but
different in either the number of parameters, and/or the type of the parameters.
note: 2 different functions that have the same name & same number and type of parameters
but differ only in  return type are not considered overload, but an ambiguous functions.
function overloading is also called compiled-time polymoprhism - because it occurs
at compile-time and "polymorphic" because we have a symbol with the same name (a
function with the same name, representing different types of functions).
Thus, these functions are said to be statically dispatched (at compile time).

C does not support function overloading! you can have: null_check_ptr, null_check_int functions instead.
in CPP function-overloading mechanism utilizes "name mangling" that occurs with every function
in cpp. for example:
we have two multiply functions: multiply(a,b,c) and multiply(a,b), to ensure
there is no collision between them, the compiler simply gives the two different names
based on their parameters. multiply(a,b,c) becomes internally ___multiply_a_b_c and multiply(a,b)
becomes ___multiply_a_b.

Note that function overloading mechanism can only be utilized at compile time, therefore
polymorphic references won't work correctly when supplied as an argument, since the type
they referto can only be deduced at run-time (dynamic dispatch), thus the type of the ref
will be sliced to its underlying type. see 121_double_dispatch as an example.

function overloading is not available in C (therefore C doesn't mangle
the name of functions.)

function overload - multiply(1,2,3) will activate this function:

function overriding - occurs only between classes and inherting subclasses, 
when a function in a subclass has the same name and parameters as the function
in base*/
int multiply(int a, int b, int c)
{

}

void multiply_and_log(int a, int b)
{
    int result = multiply(a, b); //a,b are arguments
    std::cout << result << std::endl;
}

//advanced example with pointers
/*
when passing arguments to function or when returning them, c/cpp exhibits
an "assignment" (=) relationship:
the incoming argument is the address of s which is being assigned to parameter int* w
just like an assignment operation int* w = &s

the returned value which is the address of w is returned as int*
just as we assign int* x = w  (w is already and address)
*/
int* example(int* w)
{
    ++*w;
    return w;
}



int main()
{
    multiply_and_log(3, 2); //function call, (3,2) are arguments
    multiply_and_log(8, 5);

    int s = 2;
    int* u = example(&s);
}
