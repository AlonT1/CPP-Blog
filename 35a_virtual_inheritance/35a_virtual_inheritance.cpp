
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
    (1) 0x000000053E0FFC48  0000002a (start address of d) first byte(2a) contains 42 in decimal (base_value from Intermediate1 portion)
    (2) 0x000000053E0FFC4C  00000001 i=1 of Intermediate1 portion
    (3) 0x000000053E0FFC50  0000002a 42 in decimal (base_value from Intermediate2 portion)
    (4) 0x000000053E0FFC54  00000002 i=2 of Intermediate2 portion
    (5) 0x000000053E0FFC58  cccccccc empty portion of Derived (actually there isn't any memory allocated for d's content because it
    his empty, just showing that the next address doesn't contain data)

    *******************************************************************
    Notice that (1) and (3) (the memory of Base) repeats twice in the memory of "d". This is solved by virtual inheritance:
    *******************************************************************
    */

            
    /**************************VIRTUAL INHERITANCE 
    
    https://shaharmike.com/cpp/vtable-part3/
    ensures single copy of base's data is inherited by grandchild.
    ***************************************************************************************
    tl;dr both intermediate 1 & 2 hold pointers to the same base class, instead of each of them
    holding a unique copy of Base, which causes "Derived" to have Base appearing twice in its memory,
    because its inherited once from intermediate 1 and 2.
    ***************************************************************************************
    A vtable for Derived is created that contains offsets which help us
    to reach the same Base portions of Intermediate1 and Intermediate2 (since they both point
    to the same data, the offsets will lead us to the same Base values).
    Note: vs debugger doesn't show these vtables

    This is done in the following way - given:
       A [int a]
       /     \
   B[int b]   C[int c] 
        \    /  
       D[int d]

    all inheritance are virtual   
    'D' memory section looks like this:

    4 bytes               12 bytes                     12 bytes             4 bytes     (total of 32 bytes)
    mem sec of A        mem sec of B                mem sec of C          mem sec of D
    [A::int a,      vfptr of D, B:int b,    vfptr of D, C::int C,     D::int d]

    As seen mem sec of B and C contain DIFFERENT pointers to different offsets in the vtable of D.
    for example (the numbers are random):
    vtable of D
    int offset = 20
    int baseOffset = 12
    int offsetToTop = 8
    Using these offsets, the compiler can say: "in the memory of D, we have mem portion of C, and there
    we have a pointer to a specific int offset - adding this offset to the mem portion of C will lead us to 'A'.
    In this way the mem sec of C doesn't need to store a copy of the information of 'A', but rather just a pointer to 'A'.
    
    now that there exists only one Base instance which Derived could be converted to.
    */

    d.base_value; // Not ambiguous!;
}
