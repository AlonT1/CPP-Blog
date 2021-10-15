/*
RTTI (Run-time type information) is a mechanism that exposes the run-time data
type of an object. The run-time type of an object can differ from its compile-time
only(?) when the following three conditions happen simultaneously:
1. Inheritance - class B inherits from class A.
2. Virtual functions - class A (the class we inherit from) MUST(!) HAVE AT LEAST ONE
VIRTUAL FUNCTION.
3. polymorphic ptr/ref - of type A which point/refer to an object of type B
(polymorphism - one symbol that can represent multiple other symbols:
pointer/ref of type A can represent both A and B)

Because RTTI has a pretty significant space performance cost,
some compilers allow to turn RTTI off as an optimization.
this will cause typeid and  dynamic_cast to function incorrectly.
RTTI Can be disabled -> Project properties -> C/CPP -> Language  ->
enable run-time-type information -> off
this removes the overhead!!

examples below... 
*/
#include <iostream>
#include <typeinfo>

struct A {};
struct B : public A {};

struct C { virtual void func() {} };
struct D : public C { };

int main()
{
    A* a = new B;
    //since A doesn't have at least one virtual function, typeid prints type "class A"
    std::cout << typeid(*a).name() << '\n'; 

    C* c = new D;
    // prints "class D", all three conditions above occur .
    std::cout << typeid(*c).name() << '\n';

    //other ways of checking the run-time type:
    if (typeid(*c) == typeid(D)) std::cout << "indeed1\n"; //decltype vs typeid at 118_decltype
    if (dynamic_cast<D*>(c)) std::cout << "indeed2\n"; //more at 69_casting about dynamic_cast
    //*If the dynamic cast fails, it returns a null pointer of the type inside <> (AnotherDerived)
}