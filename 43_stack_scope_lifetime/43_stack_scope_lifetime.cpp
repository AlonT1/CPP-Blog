/*
how things live on the stack?
stack - data structre which are stacked on top and popped. enter a scope - a data is pushed on the pile.
when the scope ends - the latest data is popped.
scope - if, function, empty scope {}, class 
*/
#include <iostream>


//bad user case!!!
int* create_array_stack()
{
    int array[4] = {0,2,4,6}; //array allocated on the stack!
    for (int i = 0; i < 4; i++)
        std::cout << array[i] << std::endl; // prints ok
    return array; //the array is decayed into a pointer to the first element of
    //the array (*array), but the array dies when we go out of scope!

    //an alternative is to create a stack array in main, and use a function to fill it up
    // going back to main, we can continue to manipulate the array

}

//good - lifetime of the array will stay around
int* create_array_heap()
{
    int* array = new int[50]; 
    return array; // return address allocated by new (int* recieves an &)
}

int main()
{

    int* arr = create_array_stack();
    for (int i = 0; i < 4; i++)
        std::cout << arr[i] << std::endl; // prints random values
}

