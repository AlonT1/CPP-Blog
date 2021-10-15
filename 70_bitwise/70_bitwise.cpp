﻿#include <iostream>
#include <bitset>
/*
76543210  Bit position (starting at 0)
00000101  Bit sequence

word - 16 bits == 2 bytes == 4 hexes. (1 hex == 4 bits ) (nibble = 1 byte, or 2 hexes such as 1b)
0b1000 - 8 in binary, 0b for binary

/************MSB VS LSB:
MSB (most significant bit) - leftmost bit
LSB (least significant bit) - rightmost bit
65 decimal -> 0b01000001 (1 byte of memory is the minimum addressable storage unit) msb: 0  lsb: 1
This always applies no matter the endianess (big or small).

/************Big Endian VS Small Endian:
5894623 decimal -> 0x0059F1DF in hex  represeting 32 bit integer-> (most significat BYYYTTEE is (leftmost) 00,
least significat BYYYYTTEE is DF  

endianess - order of bytes of a word of digital data in memory
endianess decides which byte should be stored in the lowest address, the msb or the lsb?
big endian -the most significant byte of the data (00) is placed at the byte with the lowest address: (the next bytes are following)
address
0x0000002C6933F744  stores 00 (hex)  //every 64 bit address can store 1 byte on byte-addressable computer
0x0000002C6933F745  stores 59
0x0000002C6933F746  stores F1
0x0000002C6933F747  stores DF
in vs memory window, 5894623 will look like 0059F1DF

little endian - the least signifcant byte is placed at the the byte with the lowest address: (the next bytes are following)
0x0000002C6933F744  stores DF
0x0000002C6933F745  stores F1
0x0000002C6933F746  stores 59
0x0000002C6933F747  stores 00
in vs memory window, 5894623 will look like DFF15900


in vs memory window, memory addresses in each columns rise from right to left (see mem_window diagram).
popular intel cpus use little endianess


Bitwise operators:

&  AND
| OR
^ XOR
<< x logical left shift by x bits
>> x logical right shift x bits  
~NOT

signed bit - msb bit that indicates the sign of the number(+ 0/- 1)
unsigned int - 0...65535 possible ints. the msb is 0 (only positive nums)
signed int - -32768 ... 32767 possible ints. the msb is only used to represent 
the sign of the number, and not another possible int!
unsigned (positive) 1 is 000000000000000000000000000001
signed -1 is 10000000000000000000000000000001
one of the problems with signed bit method is that 
There are two ways to represent zero, 00000000 (0) and 10000000 (−0).

INTEL cpu's use Two's Complement method instead of signed bit
where 1 is 00000001 and -1 is 111111111  (convert 1 to its complement -1 - rightmost 1 is detected in 00000001
and all zeros to the left of it are flipped)s
as opposoed to the signed bit method 00000000 means 0 and 10000000 means -128


*/

int main()
{
    long x = 45863255;
    int a = 0b101; //equivalent to int a = 5
    int b = 0b011; //equivalent to int b = 3

    int and_op = a & b; // 0b001 == 1
    int or_op = a | b; // 0b111 == 7
    int xor_op = a ^ b; // 0b110 == 6
    int left_shift = a << 2; // 0b101 << 2  -> 0b10100 (10) (if leftshift by x bits -> right is padded by x zeros)
    int right_shift = a >> 2; // 0b101 >> 2  -> 0b001 (4) (bits shifted off the right are lost! no padding!) 
    x >>= 1; //  equals to x = x >> 1

    /*
    in Java >> (arithmetic right shift) is special, perserves the sign of msb, this doesn't exist in c .
    for example -7 is 10000000000000000000000000000111 >> 1(in java) will yield 10000000000000000000000000000011 (-4).
     c's >> is actually >>> in Java (both called logical right shift - do not preserve msb)
    -7 is 10000000000000000000000000000111 >>> 1 (in java) will yield 01000000000000000000000000000011 (2147483644)  (same as >> in c)

    left shift for signed integers is arithmetic overlfow - undefined behaviour. for example :
    std::cout << (-7 << 1) << "\n"; //undefined behaviour
     -7 10000000000000000000000000000111  << 1    becomes 10000000000000000000000000001110 (-14) ->  the msb 1 is "stuck"
     but: 01000000000000000000000000000000 (1073741824) << 1 becomes 10000000000000000000000000000000 (-2147483648) -> the msb 0 turned to 1  - 0 isn't stuck!!

    */
    
    //************************************bitsets*/
    std::bitset<8> mybitset1{0b0000'0101}; // room for 8 flags , decimal 3
    std::bitset<8> mybitset2{ 3 }; // room for 8 flags (3 is represented by 1 byte, smallest addressable unit)
    std::cout << mybitset1 << std::endl; // prints 000000101
    std::cout << mybitset2 << std::endl; // prints 000000101
    mybitset2.set(0); // set bit position 3 to 1
    std::cout << (std::bitset<4>{ 0b0101 } | std::bitset<4>{ 0b0110 }) << std::endl; //or between two bitsets
    //bitwise operators can operate on bitsets only when the two operands are bitsets!!!!!!!

    //IMPORTANT NOTE: BITWISE OPERATORS on bitsets yield other bitsets. Bitwise operators on ints yield another int
    //when using two ints where the 1st is a bit mask (shown below) and the 2nd operand is another num (flags -set of bits
    //represeting objects boolean values (on /off true/false), people... and not an actual number), the bitwise operation result will be either 0 or 1 
    //(since the bit mask operates on a specific bit), this can be casted into a boolean

    constexpr std::uint_fast8_t num1{ 0b0000'0101 }; //just an int - not a binary bitset
    constexpr std::uint_fast8_t num2{ 0b0000'0111 }; //just an int - not a binary bitset
    std::cout <<"num1 & num2 " << (num1 & num2) << std::endl; // "oring" num1 and num2 yields 25

    //***********bit masks (the mask is just a binary number with a specific single set bit
    //which is used in bitwise operations together with another num to control a specific bit)
    //this can be done easily with bitsets which support "set" "flip".....

    //action1 - testing a if a bit is set (bitwise OR) with a bitmask
    constexpr std::uint_fast8_t mask0{ 0b0000'0001 }; // 0-is masked 1-the bit to reveal reveal, represents bit 1
    //mask0 means the 0th bit is set (the "reveal bit"
    //fast8_t for performance, the smallest and fastest size in bytes that can house 8 bytes
    //and quickly addressable by the cpu (not necesseraly 8 bits), as opposed to uint_least8_t
    //which explicitly asks for 8 bits, although it might be unoptimized for the cpu. uint - unsigned int
    //to address 8 bits, maybe 32 bit address is easier for the cpu
    //instead of 0b0000'0001 we can also use
    //1, 0x1 (hex), 1 << 0, (1<<1 means 0000 0010, shifting left 1 by 1 bit )
    std::uint_fast8_t flags{ 0b0000'0101 }; //flags (bit flags) each bit is used as a boolean value (the 0th bit is 1 (set, meaning TRUE)
    //we don't look at the sequence as a set of bits representing a number, but rather boolean values
    //(flags & mask0) prints 1 (mask0 1st bit is "reveal" and the 1st bit in flags" is set (to 1))
    std::cout << "bit 0 is " << ((flags & mask0) ? "on\n" : "off\n"); //"anding" mask0 with the flags will reveal if the 0th bit is set
    
    //action2 - setting a bit with a bitmask
    constexpr std::uint_fast8_t mask3{ 0b0000'1000 };
    std::uint_fast8_t flags1{ 0b0000'0100 };
    flags1 |= mask3; // flags1 = flags1 | mask3;  the 3rd bit of flags 1 will be set

    
    //action3 - turn off a bit whether its on or off
    flags &= ~mask3; // "not" turns off the set 3rd bit of mask 3, when "anded" with flags, 
    //its 3rd bit whill be also set off

    //action4 - toggle bit (flipping on or off the state_
    flags1 ^= mask0; //the 0th bit of flags1 is 0, 0th bit of mask0 is 1
    //xor - 1 when when two bits are different. if the 0th bit of flags1
    //was set to 1, xoring it with mask0 would cause the 0th bit flags1
    //to become 0 (flip)
    //tl;dr mask0's 0th bit is always 1, if xored with with a bit flags,
    //it will flip the bit - 0 becomes 1, 1 becomes 0 due to xor


    //bit masks also work with bitsets
    constexpr std::bitset<8> mask1{ 0b0000'0010 };
    constexpr std::bitset<8> flags2{ 0b0000'0110 };
    std::cout << (mask1 & flags2) << std::endl; //prints 00000010, as opposed to the examples above that simply print 0 or 1

    //query example
    constexpr std::uint_fast8_t is_laughing = { 1 << 0 }; //0000'0001
    constexpr std::uint_fast8_t is_happy = { 1 << 1 }; //0000'0010
    std::uint_fast8_t me{}; //0000'0000
    me |= is_laughing | is_happy; // me is set to  laughing and happy

    std::cout << "am I laughing? " <<(me & is_laughing) << std::endl; // prints 1
    std::cout << std::boolalpha; // will cause the printing of true/false instead of 0 or 1
    std::cout << "am I laughing? " << static_cast<bool>(me & is_laughing) << std::endl; // prints 1
    //same example can be applied with bitsets: (me & is_laughing).any(), meaning,
    //are there any bits set to 1 after the "anding" between "me" and "is_laughing" mask

    /***
    practical examples:
    1. instead of passing 32 bools parameters to a function, we can just pass a single int, where each bit
    serves as a true/false switch. this improves readability (less parameters) and performance (reduced
    size - 32 bools => 8 bytes * 32 bools =  256 bits  vs 32 bits in the latter technique.
    
    2. RGBA, R=255, G=255, B =255, A=255 all components can be represented by 4 different unsigned 8 bit ints
    although it will require 4 different 8 bit masks to access the 4 RGBA sections

     https://www.learncpp.com/cpp-tutorial/bit-manipulation-with-bitwise-operators-and-bit-masks/

    3. bitmasks ARE space efficient in the following example:
    100 people, 8 booleans per person (1 bool = 1 byte -> 64 bits per person) 
    (moshe_happy, moshe_sad, moshe_walking, moshe_sitting, moshe_drinking, moshe_playing, moshe_jumping, moshe_swimming,
    haim_happy, haim_sad...)
    a total of 800 bytes (6400 bits) 
    Alternative:
    instead of having 64 bits (8 bools) per person we can just use a bitset of 8 bits (in both cases we
    can represent 8 states per person). So 8bits *100 people = 800 bits  vs 6400 bits in the previous technique
    + we need an 8 bit mask to control the state of each 8 bits representing states per person.
    in all, almost 8 times less size!
    */
}
