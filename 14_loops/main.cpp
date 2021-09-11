#include <iostream>
//additional info can be found in 31_array

//a pointer just holds a memory address (just an integer) - it can be void*. but to manipulate it it must be of some type such as int*
//A void pointer can be useful if the programmer is not sure about the data type of data inputted by the end user.
//pointer arithmetic can not be performed in a void pointer. 
void func(void* a, int z)
{
    if (z == 1) //user says that the void pointer points to an int
        std::cout << *(int*)(a); //*((int*)(a)) cast pointer to an an int and then dereference it
    else if (z == 2) // user says the the void pointer points to a char
        std::cout << *(char*)(a); //*((char*)(a)) cast pointer to a char and then dereference it
}

int main()
{

    int var = 10;
    void* ptr = &var; //& address of operator returns a void pointer to its operand with its type, e.g: (int*)temp where temp points
    //to the starting mem loc of var.  temp is assigned by copy initialization to ptr_int (the content of the pointer 
    //(mem address of var) , no r/w is possible - the compiler doesn't know the type
    //*ptr = 10; // void pointer!! we have to inform the compiler it's an int - thats when types come in

    std::cout << "size of: " << sizeof(ptr) << std::endl; // prints 8
    //the size of ptr is 8 bytes because it stores the starting memory location of var (the address to the
    //1st byte at which var starts at, contains 10 in hex. the size is 8 bytes because memory addresses in 64 bit
    //systems are 64 bit long (8 bytes).
    


    int* ptr_int = &var;
    //ptr_int points to the starting address of var (this starting address is 8 bytes long
    //on a 64 bit system, thus all pointers are 8 bytes long on a 64 bits system), no matter their type.
    //the type of the pointer (int*, char*...) has only one effect - it enables r/w
    //operations on the value we are pointing to when we dereference it.
    //because ptr_int is of type int* - when we derefrence the pointer, 
    //the compiler will only read 4 bytes forward, because the size of an int
    //is 4 bytes.

    //for example:
    //ptr points to (contains the value): 0x0000002C6933F745 (the start of var) (16 hex digits, each digit 4 bits = 64 bits)
    //(add. of 1st byte)    0x0000002C6933F745  0A   (0A in hex = 10 in decimal).
    //(add. of 2nd byte)    0x0000002C6933F746  00
    //(add. of 3rd byte)    0x0000002C6933F747  00
    //(add. of 4th byte)    0x0000002C6933F748  00
    //because int is 4 bytes, it spread across 4 different bytes, each byte with its own unique 64 bit address


    //computer stores all data as a sequence of bits.
    //A variable’s type, and specifically the type of the pointer
    //tells the computer how to translate that sequence of bits into a meaningful value
    //void pointers just store the address of where to point to (the address is just a number
    //represented as hex). the int type enables the compiler to treat the address the pointer
    //points at  as one that stores an integer.

    *ptr_int = 12; //read or write to data (manipulating 4 bytes starting from the
    //gining address of var to which ptr_int points to


    int* w; // declaring w as a pointer to an int (read right to left) - it isn't initialized yet!
    //doesn't points to nowhere! but the variable itself IS defined in a location in memmory
    //pointers are like any other variable, they have stored in memory (have an address) and the content in that address
    //is an address of another object (variable, instance...)
    int* t = (int*)0; // equivalent to nullptr, means that t is pointing to 00000000 
    //as if there is an int there (but there isn't) 
    //(int*)1  starts from 00000001 memory address
    std::cout << t << std::endl;


    /*************pointer arithmetic*/
    char* buffer = new char[8];
    //*buffer = 'w' // equals to buffer[0] ='w' *(buffer+1) means buffer[1]
    // because it is a pointer to a char, the compiler knows that *(buffer+1)
    //means jumping 1 byte forward from the start of the array to reach the 2nd element -- ptr arithmetic
    //if int*, the compiler would have jumped 4 bytes
    memset(buffer, 0, 8);  //starting from the address of buffer, fill 8 bytes with 0
    char** ptrt = &buffer;
    delete[] buffer;


    int var2 = 2;
    int* ptr1 = &var2;
    int** ptr2 = &ptr1;
    std::cout << *(*ptr2) << std::endl; 

    //          mem address  data(content)
    //  var      0xb       2
    //  ptr1     0xc       0xb(address of var)
    //  ptr2     0xd       0xc (address of 0xc)

    //ptr1 == 0xb the value of ptr1 which is also the memory address of var (points to var)
    //*ptr1 == get data of ptr1 (0xb) which is a memory address, retrieive the data that lies in this memory address
    // *ptr2 means - reveal data at the memory address which is stored in the data of ptr2 (0xc) -> the revealed data is 0xb
    // **ptr2 == *(*ptr2) == *(0xb) means reveal data of memory address 0x2 -> 2

    //**ptr2's-data --> *(*ptr2's-data) --> *(*(0xc)) -> *(0xd) -> 2
    //values of pointers (the memory address of what thier pointing at) is stored in little endian (67 45 23 01) -> can be converted to big (01 23 45 67)
    //leftmost column in vs memory window is memory adress th columns to the right are the stored values (Data).
}