#include <iostream>


/*
tl;dr
use pointers to classes as fields instead of instance variables.
in the latter the compiler requires the class to be defined before it can be used as a field within
another class (may cause dependency, header includes and  ordering issues , as explained below),
while the former only requires forward declaration to the class, the ptr can be repointed and may even
reduce the size of the class (size of pointer instead of a full sizeof an instance variable).

Note: in general, in cpp we use forward declaration when we use an identifier
before it is defined. the forward declaration satisfy the compiler, promising it
that the definition exists somewhere. The definition will be found by the linker
(after compilation stage), which will link the forward declartion with the implementation.
*/


/*************************Concept explained*******************/

class Dog; // this is a forward declaration that introduces an incomplete type - 
//until definition is reached - only then the compiler will know the size of Dog


class Animal
{
public:
    Dog* dog; //a pointer to an incomplete type - Works! the compiler doesn't have to
    //know the exact definition of Dog, an incomplete type is enough because the size of 
    //the pointer is depends on the architecture (e.g: 64 bit, or sizeof(void*)
    void func(Dog dog); // can be used as an argument in a forward declaration 
    //(as long as the function isn't defined the compiler doesn't need to know the size of the parameters
    //therefore it is okay to use incomplete, undefined types
};


class Dog // the definition of Dog, making Dog a complete type.
{

};

/*************Practical Usage - class circular dependency*********

if class A uses class B as a field(1), and class B uses class A as a field(2) -
(1) couldn't be instantiated because B isn't defined yet. Switching the order between (1)
and (2) will not help because then B will not be able to use A as a field because A isn't defined.
Therefore to amend this issue we can do the following"

//A.h
class A
{ 
public:
    B b;
};

//B.h
class B
{
public:
    A a;
};


a possible (wrong) solution is to include B before A, and include A before B - 
this would cause double includes when trying to use class and A and B, since 
both of the include each other - we will have a A&B pair and a B&A pair.

*/

/*solution:
1. write a forward declaration of class B before class A.This will satisfy the compiler
the definition for class B exists somewhere, the linker will link the forward declaration
with the definition at linking stage, same for class B.

2. This doesn't solve the problem completely - when compiling A, the compiler can't
wait for the linker - to have a variable instance field, the compiler MUST SEE THE 
DEFINITION AT COMPILE TIME to calculate the size of the instance , therefore we can use a
pointer to the class as a field - which doesn't require a full definition of a class -
it simply stores an adress and doesn't need to be aware about the size of the class.
The forward declaration notifies the compiler that the class exists MIGHT somewhere, t
herefore a pointer pointing to an instance of
that class can also exists

one of the reasons why it is better to use pointers to user defined objects,
instead of instance variables, is because of class dependency issues:
.cpp files are compiled line by line. To compile class A that has class B as a field
B must be defined!

*/

//A.h
class B;
class A
{
    B* b;
};

//B.h
class A;
class B
{
    A* a;
};



int main()
{

}