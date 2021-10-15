#include <iostream>
#include <array>
/* represent a collection of variables. [] - index
store data in contiguously in memory - in a row
*/

class Array // improves the dynamic array by offering a way to retrieve its size (int* a = new int[5], sizeof(a) retrieves sizeof int pointer...)
{
public:
    int* example;
    int size;
    static const int examSize = 4;
    int exam[examSize];
    Array(int size)
    {
        this->size = size;
        example = new int[size];
        for (int i = 0; i < size; i++)
            example[i] = i;
        for (int i = 0; i < examSize; i++)
            exam[i] = i; // == *(exam+i) = i
    }
};

class Entity
{
public:
    int x = 2;
    int y = 3;
};

int main()
{
    //arrays are a data type  (we can take their size), they store data contiguously.
    //they decay into pointers when passed to functions, losing the "array-ness".
    //arrays aggregate data, while pointers just point to a location in memory (taking their size yields the size of a ptr.
    //(8 bytes in 64 bit systems because in 64 bit systems memory addresses are 64 bit wide).
    //"example" yields the address of the first element of the array which is also considered as the address of the array.
    //example[0] yields the value stored at that address. &example and example yields the same result (address).

    int example[5]; //array of 5 ints allocation for 5 integers - stack allocated and "static", meaning its size cannot be changed!
    example[0] = 2; //access  and set
    example[4] = 4;
    //std::cout << example[0];
    //std::cout << example[-1]; // memory access violation - error doesn't belong to me -
    //in release mode its possible - possible to override a memory that exists elsewhere!, in debug- not. 
    //std::cout << example; // this is a pointer type! the adress of the array

    int size = 5;
    //Error! size can be manipulated and changed during run-time, meaning that 5 isn't necessarily
    //"size"'s "final" value. This collides with the fact that stack-allocated arrays are constructed
    //during compile-time, therefore the compiler raises an error, telling us that the value that "size"
    //holds during compile-time maybe modified during run-time, thus it cannot be trusted to allocate
    //the size of the array during compile-time. Therefore "size" must be const or constexpr to solve
    //this issue.
    //int array3[size]; 

    // possible to write <=4 but <= is more expensive than < (two actions vs one),
    //more readible since our array size is 5
    for (int i = 0; i < 5; i++) 
        example[i] = i;

    int* ptr = example; //example is an integer pointer
    *(ptr + 2) = 16; // equals to example[2]  +2 means to elements forward in memory

    //e.g: value of ptr (=of array example) - points to this address- 0x008FFC50 - elements in the row are 00000001(hex- represents 32 bits or 4 bytes, each hex,e.g 01 is 4 bits * 8 hexes ) (1st element) 00000002 (hex) ...
    //pointer arithemtics: the number of bytes added depened on the type! int pointer going to add 2*4 sizeof(ptr) is 4!
    //(two elements up * 4 bytes becuase 4 is thesize of each integer) leads us to the second elemnt in memory (8 bytes forward (4bytes(num1), 4bytes(num2), 4(bytes)(num3)<- the target!)
    //*(ptr+x)  == sizeof(ptr)*x <- this is the math
    std::cout << *(int*)((char*)ptr + 8); //char is 1 byte large * 8  = going 8 bytes forward (2nd element) will lead us to the same location
    //+ cast back to int* so we can tell back the compiler that there is an int there so we can dereference it
    //*(int*)((char*)ptr + 8) == *(ptr+2) // due to pointer arithmetic
    //Note: typcasting creates 
    //order of evaluation -
    //1.((char*)ptr + 8)  the outer parenthesis win! now going in
    //2. (char*) typecast  and + operator have the same precedence - evaluated together
    //3. the result is then typecasted via (int*) and then dereferenced via * operator  (typecast and dereference have the same precedence)

    int* another = new int[5]; // created on the heap ***no way to work out the size of the array!
    //... there is a way to know because since delete commands works somehow - it is compiler dependant
    //this type of arrray is also "static" meaning its size cannot be changed
    delete[] another; //array allocated (new int[5]) requires delete[]
    //same above rules apply


    int x = 2;
    int* x_ptr = &x;

    Entity e_stack; // <- this is a variable which holds the class's data! all stack allocated
    Entity *e_heap = new Entity; // <- this is a variable of type pointer which it's data is the memory adress of where the class data is located on the heap


    /*why allocate on the heap and not the stack? lifetime - stack is bound to scope, heap will always be around until deleted
    when ever possible - better to create on the stack to avoid indirection overhead. dereferncing a pointer costs performance since we need to get the adress (value of pointer e_heap)
    and than jump to the memory location of that address, instead of a variable that its values are directly the data itself (e_stack)

    */

    int a[5]; //we can find the size since it is allocated on the stack - the stack pointer contains the offset
    int count = sizeof(a) / sizeof(int) ; //20 (5*4)  count -num of elemnts , size - size in bytes
    sizeof(another); // retrieves size of integer pointer!


    int sizeArr = 4;
    //int arr[sizeArr]; error! when we allocate array on the stack it must be in a size of a compile time known constant - not a variable!
    
    const int sizeArr1 = 4; //correct! it is a const
    int arr[sizeArr1];

    // arrays in cpp11 library - offers bounds checking size of the array.
    std::array<int, 5> test2;
}
