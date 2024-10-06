
#include <iostream>

int main()
{
    /********************for loop**********************
    
    structure:
    for ((1)init-statement; (2)condition; (3)iteration expression) {statements}
    notes:
    a. (1),(2) & (3) are optional!
    b. (2) must be convertible to bool

    steps:
    1. executes init statement (int i{0})
    2. check condition (i<5) returns true (otherwise the loop won't execute)
    3. execute statement (std::cout << "shalom\n";)
    4. iterate expression (++i)
    5. repeat steps 2-5 until the condition (i<5) returns false
    */
    for (int i{0}; i < 5; ++i)
        std::cout << "shalom ";
    
    /********************range based loop*******************
    structure:
    for ( (1)init statement  ; (2)range-declaration : (3)range expression) 
    notes:
    a. (1) is optional and only relveant from c++ 20, the init-statment is used
    to initialize some object that is scoped to the for each loop and used there
    for any purpose.

    b. (2) means a declaration of a variable whose type is the same type of the
    sequence represented by (3).

    c. (3) can be any sequence we can iteratable by iterators, including arrays
    and braced init-list (which yields std::initializer list)
    */
	int arr[5]{ 1,2,3,4,5 };

    //reference is faster, no copying is performed, 
    //n is an alias for the objects in the array
    for (int& n : arr)
        std::cout << n << ' ';


    // iterate over a braced-init-list
    for (int x : {1, 2, 3, 4, 5})
        std::cout << x << ' ';

    /**************how does a range-based loop looks behind the scenes?
    range-based loop is a syntactic sugar (equiavlent) for the following */

    //__range1 is a reference to an array of [5] ints. An alias for our "arr".
    int(&__range1)[5] = arr;
    int* __begin1 = __range1; // a pointer to the beginning of "arr", equivalent to "int* __begin1 = &__range1;"
    // a pointer to one element-after the ending of "arr" (beyond the boundary of 0-4 cells).
    //this memory location is undefined dereferencing it may lead to unexpected behaviour
    int* __end1 = __range1 + 5;
    // a "classic" for-loop - iterate "begin1" as long as it doesn't reach end1,
    //put simply, iterate forward as long as you're in the boundaries of the array.
    for (; __begin1 != __end1; ++__begin1)
    {
        // create a reference to the current dereferenced element from the array
        //looping by value (e.g: (int n: arr){}) would have cause this variable
        //also to be an lvalue, and not an lvalue ref as it is now, since we iterate
        //by ref
        int& n = *__begin1;
    }
    
    //see 5a_initialization for an example of braced-init-list inside a range-based loop.
    

}