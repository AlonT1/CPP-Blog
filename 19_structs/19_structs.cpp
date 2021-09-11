/*same as class only struct are public by default, classes are private.
Same as classes, structs are USER-DEFINED TYPES.
structs are user-defined type, an aggregate data type (aggregates data).
they are fit for plain old data - POD - stands for Plain Old Data - that is, a class / struct 
without constructors, destructors and virtual members functions
something which represents variables, like a math vector class.
int, char, wchar_t, bool, float, double are PODs,
struct exists in cpp mainly for backwards compatability with c
in c -> #define struct class - a hack 
never use inheritence with a struct - added level of complexity
class inherits from a struct - possible gateway for errors in compilers

there is no extra memory or padding for the struct.
empty structs weigh 1 byte since we need to reference them
but when it holds data, there is nothing that denotes that a struct exsists
other than the data itself.


c structs don't support functions
 */
#include <iostream>

//padding:
struct Foo
{
    char x; //1 byte
    short y; //2 byte
    int z; //4 byte
};
/*
in this example
in computer architecture memory addresses are stored in word sized chunks.
the word (16 bits - 2 bytes) is the natural unit of data used by a processor.
the smallest addressable memory in computers  is 1 byte (8 bits) (because a character is 1 byte).
each byte has an address, e.g:
0x0000002C6933F744  may store 41(in hex)  (8 bits, represeting 65 in decimal , "A" in ASCII)
The next address 0x0000002C6933F745, represents the next byte which contains 00.
0x0000002C6933F746 stores 00 and 0x0000002C6933F747 store 00:
in total 00000041 or 65 in decimal  (in little endian where lsb is at the start)
For example, an integer is usually spread around 4 bytes (4 different memory addresses)

for 32bit cpu's, a memory address is is comprised of 32 bits -  (double word)
for example, these are 4 different contiguous memory addresses, each are 32 bits (4*B = 32 bits), B- a byte
BBBB   BBBB   BBBB   BBBB

struct members are stored in their order of DECLERATION!
char x will be stored 1st, short y 2nd, int z 3rd. The compiler(?) adds padding to avoid
fragmentation which can degrade performance due the necessity of multiple memory accesses
to different contiguous words and bitshifting to extract the int.

Rules of padding:
1. Every member of a struct should be at an address which is multiple of its size:
e.g int can be at the beginning/address 4th byte/8 byte/12 byte
2. every variable can be stored at the start
3. Padding at the end is decided based on the size of the largest member of the struct,
so that the size of the struct can be divided by the size of the largest type
(A struct is always aligned to the largest type’s alignment requirements)

naive packing would yield (for the struct above):

xyyz  zzzB  BBBB BBBB

int z is stored across the first and second word - fragmented!
this can be avoided by packing according to rules above:

P - padding
xyyP zzzz  BBBB BBBB


if the class members would have declared like this:
struct Foo
{
    int z; //4 byte
    char x; //1 byte
    short y; //2 byte
};
no padding needed! since int z would not be fragmented!


the performance difference between aligned and misaligned structs is negligible:
https://hps.vi4io.org/_media/teaching/wintersemester_2013_2014/epc-14-haase-svenhendrik-alignmentinc-paper.pdf




*/

//packing example 2:
struct Foo3
{
    char c;
    double d;
    char r;
};

/*
Packing:

cPPP iiii ssPP PPPP dddd dddd rPPP PPPP

c - no padding needed at the beginning 1 is multiple of everything
i - padded to satisfy rule 2 (stored in the second word) - the second address 
starts at address 0x00000000 (word 1 is address 0x00000004)
s - stored at adress 0x00000008 (multiple of 2 bytes)
d- cannot be stored at address 0x00000012 because 12 is not a multiple of d (8 bytes)
therefore pad until the 0x00000016 address
r- stored in 0x00000020 (1 is multiple of everything)
PADDING AT THE END satisfies rule 3, to ensure that the struct is aligned to
the largest type's alignment requirements. this ensures that an array of structs
can be aligned


*/



struct Vec2 //is just a representation of 2 floats, not supposed to contain
//functionality like a player class (3d model, render, movement, input_
{
    float x, y;
    void add(const Vec2& other)
    {
        x += other.x;
        y += other.y;
    }
};


//typedef struct
typedef struct Test // struct is the type, Test is the new name associated with the type
{
    int x;
}foo;

//in c, typedefing a struct allowed getting rid of the struct keyword when declaring a struct:
//Test test, instead of struct Test test;

//foo is an inline declaration+definition of a struct
//the name Test isn't necessary! a struct can be anonymous, we can declare a struct instance inline:

typedef struct
{
    int x;
}foo4;

//the name of the struct is also useful if we want to have a memeber which points to the struct, 
//like a node in a tree:

struct Node
{
    Node* left;
    Node* right;
    int data;
};
//in this case the name is mandatory since the pointers must point to something (to the name!)


int main()
{
    struct Vec2 vec; //equals to Vec2 vec; struct key word isn't necessary in cpp, it is in c
    
    std::cout << sizeof(Foo3);
}