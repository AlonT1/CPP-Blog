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
stack allocated structs/classes cannot be null like a pointer, no need to null check!
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


79

"Word size" refers to the number of bits processed(read/write) by a computer's CPU in one go, typically 32/64 bit
Data bus size, instruction size, address size are usually multiples of the word size:
1. data bus size: num of bits that can be transferred simultaneously between the CPU and memory.
For example, if a computer has a 32-bit data bus, it can transfer 32 bits of data at a time 1.
2. Instruction size: number of bits used to represent an instruction in the computer’s instruction set.
e.g: if a computer has a 32-bit instruction size, each instruction in the instruction set is represented by 32 bits.
3. Address size: num of bits used to represent a memory address.
e.g: if a computer has a 32-bit address size, it can address up to 2^32 bytes of memory, or 4 GB (read operating systems doc).

Just to confuse matters, for backwards compatibility, Microsoft Windows API defines a WORD as being 16 bits,
a DWORD as 32 bits and a QWORD as 64 bits, regardless of the processor - but this is irrelevant for this context.


Padding is added to a struct to ensure that each member of the struct is aligned on a memory boundary that 
is a multiple of its size. explanation:
We know that memory addresses start from 0x0000, and points to the 1st byte in memory
the address of next byte is 0x0001, the address of next byte is 0x0002, and so forth.
Simplifying we have the 0th byte, 1st byte, 2nd byte, 3rd byte...
given a "int" we know its size is 4 bytes, thus we say it has a 4-byte memory boundary, what this means
is that the compiler will add padding to align the integer on a byte that is address is a multiple of 4 (size of int),
for example the the 0th byte (multiple of every number), the 4th byte, the 8th byte, and so forth.
for example, given a system architecture of 32 bits - the word size, max bits that can be processed in a single mem access:

struct Example
{
    char a;   // 1 byte
    int b;    // 4 bytes
};

Naively, if we try to "pack" these together in a single word (to minimize memory access), we will get:
abbb b  (each letter represents a byte)
so in the 1st word we have 1 byte representing a char + 3 bytes of the integer, and in the 2nd word we store 1 byte of int.
the result is that "b" is fragmented across 2 words -  and crashes in some CPU's, and slower performance,
because if we want to read the integer, we need to perform 2 memory access (to 1st and 2nd word) and construct
the integer from the data we gathered

the alternative is:

struct Example
{
    char a;   // 1 byte
    char pad[3]; // 3 byte padding (done automatically by the compiler).
    int b;    // 4 bytes
};

0123 45678
aPPP bbbb (P is for padding)
now we can read the integer in one go. note that b was aligned to the 4-byte boundary (starts from the 4th byte,
a multiple of the integer size).


in computer architecture memory addresses are stored in word sized chunks?????.
the smallest addressable memory in computers  is 1 byte (8 bits) (because a character is 1 byte).
each byte has an address, e.g:
0x0000002C6933F744  may store 41(in hex)  (8 bits, represeting 65 in decimal , "A" in ASCII)
The next address 0x0000002C6933F745, represents the next byte which contains 00.
0x0000002C6933F746 stores 00 and 0x0000002C6933F747 store 00:
in total 00000041 or 65 in decimal  (in little endian where lsb is at the start)
For example, an integer is usually spread around 4 bytes (4 different memory addresses)

struct members are stored in their order of DECLERATION!
char x will be stored 1st, short y 2nd, int z 3rd. The compiler(?) adds padding to avoid
fragmentation which can degrade performance due the necessity of multiple memory accesses
to different contiguous words and bitshifting to extract the int.

Rules of padding:
1. Every member of a struct should be at an address which is multiple of its size:
e.g int can be at the beginning/address 4th byte/8 byte/12 byte
2. every variable can be stored at the start (0th byte, because 0 is a multiple of every size)
3. Padding at the end will be added to ensure that the size of the whole struct is a multiple
of the largest data member of the struct. this ensures that an array of structs can be aligned.
the performance difference between aligned and misaligned structs is negligible:
https://hps.vi4io.org/_media/teaching/wintersemester_2013_2014/epc-14-haase-svenhendrik-alignmentinc-paper.pdf




*/

//packing example 2:
struct Foo3
{
    char c; // 1 byte
    int i; // 4 bytes
    short s; // 2 bytes
    double d; // 8 bytes
    char r; // 1 byte
};

/*
Packing:
0 1 2 3   4 5 6 7   8 9 10 11  12 13 14 15  16 17 18 19   20 21 22 23   24 25 26 27   28 29 30 31   (0xbyte num)
c P P P   i i i i   s s P P    P  P  P  P   d  d  d  d    d  d  d  d    r  P  P  P    P  P  P  P

sizeof(Foo3) - 32 bytes

c - no padding needed at the beginning 0 is multiple of everything
i - padded to satisfy rule 1 (stored in the second word) - the second address 
starts at address 0x00000000 (word 1 is address 0x00000004)
s - stored at adress 0x00000008 (multiple of 2 bytes)
d- cannot be stored at address 0x00000012 because 12 is not a multiple of d (8 bytes)
therefore pad until the 0x00000016 address, the 16th byte (multiple of 8 bytes)
r- stored in 0x00000024 (1 is multiple of everything)
PADDING AT THE END satisfies rule 3, to ensure that the size of the struct is a multiple of the largest
data type member (32 bytes is a multiple of 8 byte double). this ensures that an array of structs can be aligned.
for example, lets say that the size of the struct is 28 bytes, without the extra 4 bytes at the end (28-31),
and we initialize the following array: "Foo3 arr[2];"
then arr[0] would be word-aligned, but arr[1] would not be, it will start on the next 29th byte.
this means arr[1] will be fragmented across two different words (0-27 byte is arr[0], 27-54 is arr[1],
so arr[1] occupies the last 4 bytes of the word of arr[0], 28 bytes of arr[1] (word size = 32 bytes).

Recall that array lookup is done by adding multiples of sizeof(array[0]) to the address of the first element,
so we don't want to end up with fragmented array elements across different bytes.

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


//the name of the struct is  useful if we want to have a memeber which points to the struct:
struct Node
{
    struct Node* left;
    struct Node* right;
    int data;
}node; // inline instance of Node

// typedefing:
typedef struct Node 
{
    struct Node* left; // still need to write "struct" here, typedef only begins later
    struct Node* right;
    int data;
} Node; // "struct Node" is now typdefed as <----Node, so we can delcare "Node node = {...};" not an instance!
// note that we can still use struct keyword "struct Node node = ". it's up to us if node is a ptr or stack allocated.
// in cpp struct keyword is not necessary! see 128_typedef.cpp why

typedef struct // structs can be annonymous!
{
    int x, y;
}TheStruct; // the annonymous struct above can be instantiated as "TheStruct myStruct;"

int main()
{
    struct Vec2 vec; //equals to Vec2 vec; struct key word isn't necessary in cpp, it is in c
    
    std::cout << sizeof(Foo3);
}
