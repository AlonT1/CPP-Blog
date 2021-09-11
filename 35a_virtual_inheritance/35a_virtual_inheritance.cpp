
#include <iostream>

/*
diamond inheritance
            Base
            /   \
Intermeidate1   Intermediate2
            \   /
            Derived

Virtual inheritance is a C++ technique that ensures only one copy of a base class's
member variables are inherited by grandchild derived classes.
Note: virutal inheritance has nothing to do with virtual functions.

**************************************************************
WHEN TO USE: when a diamond inheritance pattern occurs
**************************************************************
*/


struct Base
{
    int base_value = 42;
    int ten = 14;
    int ww = 14;

};

struct Intermediate1 : virtual Base //virtual inheritance
{
    int i = 1;
};

struct Intermediate2 : virtual Base //virtual inheritance
{
    int i = 2;
};

struct Derived : Intermediate1, Intermediate2
{

};


int main()
{
    /*************WITHOUT VIRTUAL INHERITANCE*/
    int x = 5894623;
    Derived d;
    //d.i; //ambiguous - did we mean i from Intermediate1 or Intermediate2?

    //solution - explicit call
    d.Intermediate1::i; //choosing "i" through the specific class  from d's memory portion-
    //d.base_value; // ambiguous! did we mean base_value from d's Intermdiate1's Base portion
    //or base_value from d's Intermediate2's Base portion  - these are completely unieque 2 different base_value's!
    //solution: explicit call
    d.Intermediate1::base_value;
    d.Intermediate2::base_value;

    //for example, incrementing d's base_value of Intermediate1 will not affect d's base_value of Intermediate2
    d.Intermediate1::base_value++; //prints 43
    d.Intermediate2::base_value; // prints 42  - they are completely different base_value, housed inside d


    /*d's memory contains three main parts (the actual data is stored in the bytes in the following order):
    1. memory portion of Intermediate1 (i=1) that also contains portion of Base (base_value = 42)
    2. memory portion of Intermediate2 (i=2) that also contains portion of Base (base_value = 42)
    3. memory portion of Derived (empty in this case)
    these portions only apply to the realm of the compiler/ide, memory wise the data of d
    is just aligned in the bytes one after another - there is nothing that says "portion of x" - this 
    is only visible to the compiler. the memory looks like this:
    0x000000053E0FFC48  0000002a (start address of d) first byte(2a) contains 42 in decimal (base_value from Intermediate1 portion)
    0x000000053E0FFC4C  00000001 i=1 of Intermediate1 portion
    0x000000053E0FFC50  0000002a 42 in decimal (base_value from Intermediate2 portion)
    0x000000053E0FFC54  00000002 i=2 of Intermediate2 portion
    0x000000053E0FFC58  cccccccc empty portion of Derived (actually there isn't any memory allocated for d's content because it
    his empty, just showing that the next address doesn't contain data)

    */



    /*************VIRTUAL INHERITANCE 
    https://shaharmike.com/cpp/vtable-part3/
    ensures single copy of base's data is inherited by grandchild. 
    tl;dr both intermediate 1 & 2 hold pointers to the same base class, instead of each of them
    holding a unique copy of base. A vtable for Derived is created that contains offsets which help us
    to reach the same Base portions of Intermediate1 and Intermediate2 (since they both point
    to the same data, the offsets will lead us to the same Base values).
    Note: vs debugger doesn't show these vtables

    This is done in the following way:
    Note: vsdeugger hides the implementation (no vtables visible)
    A vtable for the Derived class is created. Memory wise d's memory will look like this:
    1. A pointer to a vtable (vfptr) from the Intermediate1 Portion of Derived
    that points to a 32 bit offset integer housed in the vtable of Derived.
    By adding 32 to the address of this pointer, we will get the address of the data of A 
    2.memory portion of Intermediate1 (i=1)
    3. A pointer to a vtable (vfptr) from the Intermediate2 Portion of Derived
    that points to a 16 bit offset integer housed in the vtable of Derived.
    By adding 16 to the address of this pointer, we will get the address of the data of A 
    4. memory portion of Intermediate2 (i=2)
    5. the data of Derived class
    6. the data of Base class (to which Intermediate 1 and 2 point to by adding the offsets that reside
    in the vtable of Derived
    */
    d.Intermediate1::base_value++;
    d.Intermediate2::base_value;


    /*
    Additionally, a direct cast from Derived to Base is also unambiguous, 
    now that there exists only one Base instance which Derived could be converted to.
    */

    //d.base_value; // Not ambiguous!;
    std::cout << sizeof(d) << std::endl;
    /*
    0x000000C3676FF648  00007ff6770d9c20
    0x000000C3676FF650  cccccccc00000001
    0x000000C3676FF658  00007ff6770d9ce8
    0x000000C3676FF660  cccccccc00000002
    0x000000C3676FF668  cccccccc0000002c 
    
    */
}