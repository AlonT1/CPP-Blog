
/*
3 variable types in cpp: values, pointers, references. 
primitive types (POD): float, double, bool: 
symbols: classes, functions, constants, variables...

 computer stores all data as a sequence of bits. 
 A variable’s type tells the computer how to translate that sequence of bits into a meaningful value

integer types (fundamental data types, built in the language):
char (1 byte), short(2 bytes), int (4 bytes), long (4 bytes***), long long (8 bytes)
***8 bytes in 64 bits.
floating-point types:
float (4 byte, 6 decimal places), double (8 byte, 15 decimal plcaes), long double (10 byte, 19 decimal plcaes)

these sizes are determined by the os - the data model for windows x64 is the LLP64 data model.
the c/cpp standard doesn't specify these sizes!

int is 4 bytes because x86 processors can directly fetch a 32 bit wide field
from memory. memory is divided into "words" (x86 word - 32 bit (4 bytes),  x64 word - 64 bit (8 bytes)
and an int easily fits into a word of x86 systems, therefore it can easily be fetched with single access
to that specific word (address which stores the int). x64 can store 2 ints in a single word!

typedef types from stdint.h (not builtin, not guarantee to exist in all compilers)
int8_t, int16_t....  (the names are consistent - after the 1999 update of ISO C standard C(99)
these are fixed-width types - consistent among compilers (each implement a version of stdint.h)
to ensure that int16_t is always 16 bits!
to check their "true" type behind the scenes - go to their definition in stdint.h

for example: x86 compiler can typedef int16_t as short
while a small embedded device can typedef int16_t as int, because int in that system is 16 bit!

In order for a piece of C/C++ code to be portable among many different compiler/hardware combinations,
stdint.h provides typedefs that guarantee you certain width (or minimum width).
So, when for example you want to use a 16-bit signed integer (e.g. for saving memory, or mod-counters),
you don't have to worry whether you should use an int or short, by simply using int16_t.
The developers of the compiler will provide you a properly
constructed stdint.h that will typedef the requested fixed-size integer into the actual 
primitive that implements it. That means, on x86 an int16_t will probably be defined as short, while 
on a small embedded device you may get an int, with all these mappings maintained by the compiler's developers.

can be declared signed or unsigned. e.g: signed int x;  unsigned int y;

*/

#include <iostream>


size_t w=4; //  size_t is a size of a WORD - depends on the os/cpu
/*
size_t defined in a header as :
#ifdef _WIN64  
typedef unsigned __int64 size_t;  //for 64bit systems, equals to unsigned int of 8 bytes
#else
typedef unsigned int     size_t; //for 32bit systems it equals to unsigned int (4
 bytes)
*/

int main()
{
    float x = 5.4f; // without f it is considered a double
    double d = 4; //d is initialized via "copy-initialization" of 4 into d
    bool t = 5==4; //returns 0 
    unsigned int variable = 0;
    std::cout << sizeof(size_t) << std::endl;

    decltype(x) y = 4; //y is a float because x is a float
    //decltype(entity/expression) specifier inspects the declared (compile-time) type of the entity/expression
    //typeid inspects the RTTI (real-time type information of the object):
    //for example, Animal* anim = &Cat - then decltype(anim) --> Animal*, decltype(*anim)--> Animal& (a type of reference to Animal
    // typeid(anim) --> Animal*
}
