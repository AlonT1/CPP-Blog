/*
type punning - (treating a type stored in a memory as if it was another different type).
all we have to do is to get the adress of the type (address of an int),
cast it to a different pointer type (double*) and dereference it to get the value.
getting around the type system by raw memory operations. cpp is strongly type - types
must be explicitly declared with the exception of auto, although types are enforced
by the compiler, we can manipulate int memory block and treat it as float, for example.

*/

#include <iostream>

struct Entity
{
    int x, y;

    int* getpos()
    {
        return &y;
    }
};

int main()
{
    int a = 50;
    double value = a; //implicit conversion of int into a double
    double value2 = (double)a; //Explicit conversion (type casting) of int into a double
    //equivalent to:
    double value3 = *(double*)&a; //explanation
    /*
    type punning our integer into a double. we want treat the memory block of a as if it were
    a mem block designated for a double.

    &a - (address-of operator) returns a void pointer of type a that points to a. 
    (double*)&a - csasting the void pointer to a pointer to a double
    *(double*)&a - we dereference the "casted pointer to a double" and get the data that is stored there
    (the int) which is treated as a double.

    *(double*)&a this expression returns gibrish
    */
    value3 = 4;
    std::cout << value3 << std::endl; // the value3 is gibbrish


    Entity e = { 5,8 };//the struct is made up of two integers and thats it
    //there is no extra memory or padding for the struct.
    //empty structs weigh 1 byte since we need to reference them
    //but when it holds data, there is nothing that denotes that a struct exsists
    //other than the data itself

    int* pos = (int*)&e; // because the struct holds only ints, we can easily type pun it into int
    /*explanation: &e returns a pointer of type entity which points to the starting adress of e, this pointer 
    is then cast to an int pointer - this works because indeed this 4 bytes store an int
    */
    std::cout << pos[0] << std::endl;
    
    int y = *(int*)((char*)&e + 4); //raw memory arithmetic using char that allows byte by byte traversal, so we get y of "Entity e"
    /*
    &e - returns an entity pointer to the starting mem location of e
    (char*)&e - cast the entity pointer into a char pointer, char allows to maniuplate memory byte by byte
    (char*)&e + 4  - since we are pointing to start location of char x(orignially, int x),
    adding 4 moves us to the memory address of y (pointer artihmatic). Note because we add 4 to a char pointer
    we add 4 bytes, if the ptr was an int, adding 4 would have moved us 4*4 = 16 bytes
    (int*)((char*)&e+4) we cast y back to an int*, meaning we treat the memory block of y as an int (that it truly is)
    *(int*)((char*)&e+4) - we dereference y
    */

    /* the above example is useful if we do not want to copy e.y directly. the memory operations
    of int y allow us to directly retrieve that memory location!*/

    //alternative:
    int* y2 = e.getpos();

    std::cout << y << std::endl;

}
