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

2. if expression e is parenthesized (e.g: decltype((e)) decltype returns the underyling type AND the value
category as interpreted by the compiler (probably works by overloading the () operator):

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
decltype is a KEYWORD and returns the compile-time type of an expression (types are not allowed,
i.e decltype(int) - see template example below)!.
************************************************************************************************8
in addition decltype can be used to specify the type of an object, using another type (not possible with typeid):
************************************************************************************************
int x = 4;
decltype(x) w = 2; // w is now also an int

typeid is a FUNCTION that returns the run-time type of an expression AND a type (typeid(int)
even template parameter types (typeid(T)
for example, the statement:
A* a = &B; (assuming B inherits from A, and overrides a virtual function from A!)
typeid can obtain the run-time type of pointer "a" which is B, while decltype will yield
the compile-time type of a which is A*
*/

class A 
{
public:
    virtual void foo() = 0;
};

class B : public A 
{
public:
    void foo() override {}
};


// typeid can return the name of both a template parameter type and a variable.
// since templates and decltype both run at compile time, they can safely be used together
template <typename T>
void func(T x)
{
    std::cout << typeid(T).name(); // type id can accept both types and expressions
    std::cout << typeid(x).name();
    decltype(x) w = 5; // w is the same type of T (if T is int, w will also be int)
    // decltype(T) w = 5; // Error! decltype accepts only Expressions and not Types!!!!!

}


int main()
{
    std::cout << std::boolalpha; //bool prints "true"/"false" instead of 1 or 0

    int n = 4;
    //unparenthesized decltype (not decltype((a)), but decltype(a) - 
    //returns the underlying type of a - an int)
    decltype(n) j = 45; //using type of a to grant "j" int type.

    // this is especially helpful when we the complex return type of a function,
    // for example a vector of the value returned by std::bind
    // std::vector<decltype(std::bind(add, std::placeholders::_1, 1))> vec;
    // https://stackoverflow.com/a/30906150

    std::cout << "is j an int? " <<  std::is_same<decltype(j), int>::value << '\n'; // prints true

    B b;
    A* a = &b;
    std::cout << "typeid of a: " << typeid(a).name() << '\n'; // returns "class A* __ptr64"
    // returns "class B" because the run time type of a is B (it is a polymorphic pointer
    // that points to B which inherits from A and overrides a virutal function)
    // without inheritance + overriding virtual function, the runtime
    // type of the polymorphic pointer would have been Class A!
    // polymorphic pointer + inheritance + overriding virtual function
    // are the enablers of runtime types! (see 28_virtual_functions).
    std::cout << "typeid of a: " << typeid(*a).name() << '\n';
    if( typeid(*a) == typeid(B)) std::cout << " true\n";

    //returns false because decltype returns the compile type of "a" (class A* __ptr64)
    // and the run time type of a like typeid does
    std::cout << std::is_same<decltype(a), class B>::value << '\n';


    const int& z = 4;
    std::cout << std::is_same<int&, decltype((z))>::value;
    // z is an lvalue expression (like all identifiers) and (int&) represents an lvalue - see 113b_universal_reference
    // therefore, the statement prints true

}