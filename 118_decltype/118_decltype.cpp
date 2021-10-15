#include <iostream>

/*
https://en.cppreference.com/w/cpp/language/decltype
https://stackoverflow.com/questions/36517596/decltype-parenthesis-syntax-for-a-lvalue
https://stackoverflow.com/questions/27557369/why-does-decltypeauto-return-a-reference-here

how decltype works - decltype(e):

1. if expression e is unparenthesized (e.g: decltype(e)) decltype returns the underlying type of the expression
(in other words the decl-type-specifier see 30b_declarations_declarators):
int x; decltype(x) => int  |   int& x; decltype(x) => int&  |  int& x; decltype(x) => int&
decltype(4) => int  |   decltype('c') => char  |    decltype(std::move(4)) => int&&

2. if expression e is parenthesized (e.g: decltype((e))) decltype returns the underyling type AND the value
category as interpreted by the compiler:

    a. if decltype yields T&&, then the value category of expression e is xvalue:
    decltype( (std::move(x)) ) => int&& , decltype( (std::move(4)) ) => int&&

    b. if decltype yields T&, then the value category expression of e is lvalue:
    int x; decltype ((x)) => int&
    explanation: x is an identifier of an object (region in memory) that interpreted by
    the compiler as storing an int, in other words, x can bind to literal integers.
    the value category of expression "x" is an lvalue because it has an identity (memory address)
    and it is not stealable (because x maybe in use by other pointers or references).
    Note: the base type of the object identified by "x" is "int", but the expression "x"
    (evaluates to a single ouput value) is an lvalue.

    c. if decltype yields T, then the value category of expresion e is prvalue.
    for exmple: decltype((4)) => int
    int here signifies the base type, but also denotes the value category of the
    literal "4" which is a prvalue. this is in contrast to unparanthesized e
    which only yields the type "int" (from which we can't deduce the value category)

*Note: decltype vs typeid:
decltype is a KEYWORD the type of an expression at compile-time.
typeid is a Function that can obtain run-time information about the type of an expression.

for example, the statement:
A* a = &B; (assuming B inherits from A)
typeid can obtain the run-time type of pointer "a" with is B (a is a polymorphic 
pointer that points to B, when B inherits from A), while decltype will yield
the compile-time type of a which is A*


*/

class A {public:};
class B : public A {public:};

void func(A* a)
{
    std::cout << typeid(*a).name();
}

int main()
{
    std::cout << std::boolalpha; //bool prints "true"/"false" instead of 1 or 0

    int n = 4;
    //unparenthesized decltype (not decltype((a)), but decltype(a) - 
    //returns the underlying type of a - an int)
    decltype(n) j = 45; //using type of a to grant "j" int type.

    std::cout << std::is_same<decltype(j), int>::value << '\n';

    B b;
    A* a = &b;
    func(a);
    //std::cout << typeid(*a).name();
    if( typeid(*a) == typeid(B)) std::cout << " true";
    //std::cout << std::is_same<decltype(a), class B>::value << '\n';

}