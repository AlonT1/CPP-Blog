#include <iostream>

//a reference is a stubborn alias of an object. 

//note: passing by reference to a function can be emulated via a pointer to pointer:
//caller: int x = 5  foo(x), callee: foo(&x)    can be emulated by  int* x = &y  f caller: foo(&x)   callee (int **x)
//derefercing x will access a pointer which has the same address and same value that exists in the scope of the caller,
//in the same way that references have the same address and the same value as the value they refer to (aliases)

//references cannot be reassigned, be assigned with a nullptr
//C++ references are favored over pointers wherever possible because they cannot be
//null, appropriately express by-reference intent, and have better readability 
//because of no dereferencing (*, ->) jumble.

//Also, we cannot get the address of a reference itself because a reference is
//not an object (a region of storage) according to the C++ standard.
//when taking the address of a reference, the compiler retrieves the address
//of the object represented by the refrence.

/*
Note 1: array of references and reference to array
int nums[]{1,2};
int& arr[2]{nums} // array of references are not allowed!!!!
C++ Standard §8.3.2/4:
There shall be no references to references, no arrays of references, 
and no pointers to references.
but a reference to an array CAN exist:
int (&arr)[2]{nums} // arr is a reference to an array of 2 ints
we can then access the nums array through the reference:
arr[0], arr[1] -> arr is an alias for nums array - IT IS the array

Note 2: if func() accpets a variable by ref - when this variable gets out of scope,
only the reference is gone, not the underlying object:

void func(int& x)
{
    //reference x goes out of scope - only the reference dies, not the underlying object.
}

int main()
{
    int x=5;
    func(x)
    std::cout << x // x is still alive!!!
}
*/


/*the argument that "int* value" parameter recieves is a memory address.
equivalnet to  int* value (parameter) = &num (argument- the sent value to this function)
the pointer points to a memory address (assigned with a memory address)*/
void increment_byptr(int* value) 
{
    ++(*value); //prefix++ and * same precdence and rl associative
}

/*the argument that (int& value) parameter recieves is the variable itself(int) - the ref is the variable!
equivalnet to  int& value (parameter) = num  (num is an int)(argument- the sent value to this function)
the reference IS the assigned variable. "value" is a reference to an int*/
void increment_byref(int& value) //equivalent to int& value = int a; 
{
    value++;
}

void print(const char*& str)
{
    std::cout << str << std::endl;
}

int* test()
{
    int x = 4;
    //return x;  undefined memory since x will be popped of the stack - nothing to return
}

int vals[] = { 1,2,3,4,5 };
int& set_values(int i)
{
    return vals[i]; //returning  an alias of that memory location - this is not the simple value!
    //we return an lvalue of that number - a variable with location in memory, not a temporary prvalue
    //changing the returned reference, will change the number "inside" the array.
    
    //IMPORTANT NOTE: if vals[] was a local variable, we would have returned 
    //a reference to an object who would have been destroyed when leaving this
    //scope (Returning a dangling reference). because vals[] is a global variable,
    //returning a reference actually works.
}

int main()
{
    int a = 5;
    int b = 8;
    int& ref = a; // when declaring+defining an lvalue reference it must be initialized inline - Read: Reference to an int
    //lvalue means an object that lives an memory. int& ref, is a reference to an lvalue
    ref = b; // value b gets into ref which gets into a - the ref IS the "a" variable, ref is an alias
    // both below are equal
    increment_byref(a);
    increment_byptr(&a);

    int* c = &a; // here & operator means address-of a, which return a pointer to the type of "a" (pointer to an int)
    //implicitly equiavlent to: int* c = (int*)(&a)
    int*& d = c;  //reference to a pointer - d is an alias for the c pointer

    const char* e= "shalom"; //explained in 32_strings
    print(e);
    //std::cout << *d << std::endl;
    
    set_values(1) = 4; // changing the value at index 1 in set_values, set_values(1) returns an lvalue, the location in memory of index 1

    const int f = a;
    auto& g = f; // evaluated to "const int& g = f"

    //references cannot be rebinded!
    int w = 4;
    int y = 5;

    int& x = w;
    //doesn't rebind x to refer to y, but rather x, that is an alias for w is being
    //assigned with 5, affecting both x and w (changes value from 4 to 5)
    //in other words, we assign a value to x (an alias to w), not rebinding x to y.
    x = y; 

    std::cout << w;

}

