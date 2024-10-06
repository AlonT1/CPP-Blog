#include <iostream>
/*

https://stackoverflow.com/questions/61823137/why-does-a-const-reference-to-a-reference-lose-its-constness

“X const& x” and “X const* p”

1. east-const notation (const on the right of what it consitifies - the int):
"int const& x" - x is a reference to a const int

2. west-const notation  (const on the left of what it consitifies - the int):
"const int& x" - x is a reference to an int that is constant

although both (1) and (2) are identical, (1) has a clear advantage:
1. it is less confusing to read.
2. it solves the following confusion: */

// west-const notation:
template <typename T>
void foo(const T& x) {}

// east-const notation
template <typename T>
void goo(T const& x) {}

int main()
{
    int x = 4;
    foo<int&>(x);
    // with west-const the following evalution occurs:
    // const T &x ==> const int& &x ==> const int& x (due to reference collapsing)

    goo<int&>(x);
    // with east-const the following evalution occurs:
    //  T const &x ==> int& const &x ==> "int& const": references are always const by nature - 
    // they must be initialized and cannot be reassigned, therefore "int& const" is reduced to "int&" ==>
    // "int& &x" ==> int& x (due to reference collapsing)

    /************************************************
    with west-const we get: const int& x
    with east-const we get: int&

    which version is correct: east-const! therefore write with east-const! (it is the standard)
    ***************************************************
     
    Note: remembmer that const instances can only call const methods and not non-const methods! 
    
    */

}