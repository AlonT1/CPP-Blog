#include <iostream>
#include <vector>
/*
function pointers are a way to assign a function to a variable. traditionally, 
we only call functions as a symbo, without doing any logic on them.

are very useful when we want some function to take in some specific behaviour (a specific function)
to perform an action (a sort algo, that takes in a function (ascending/descending) to decide how to sort)
functions are cpu instructions. when we compile are code, any function gets
compiled into cpu instruction and they rest in the exe (binary). 

function pointer -  find the function from in exe and get the memory address of
the instruction.
*/

void hello_world(int x )
{
    std::cout << x << std::endl;
}


void print_value(int value)
{
    std::cout << "value: " << value << std::endl;
}

void for_each(const std::vector<int>& values, void(*func)(int)) // func is the actual function
{
    for (int value : values)
    {
        func(value);
    }
}

int main()
{
    std::cout << hello_world << '\n'; //prints the address of hello_world - has an address, just like a variable!
    //the address of a func is where its code starts.
    std::cout << &hello_world << '\n'; // identical to above

    void(*function1)(int) = &hello_world; //"hello_world" is identical to "&hello_world".
    //cpp implicitly converts "hello_world" to &hello_world if "hello_world" is used.
    //i prefer &hello_world because pointers point ti addresses 
    //according to the rules in 30a_reading_declerations:
    //function1 is a pointer to a function with int paraemeter returning void
    //THE TYPE IS: void(*)(int) and THE NAME IS: function1
    //function pointers must initialized with a function when declared
   //**********NOTE: void is the return type which is never allowed to be in a parenthesis!
    function1(4);

    void(*function3)(int) = hello_world; //cpp implicitly converts hello_world
    //to a pointer (just as & operator does),
    //so address-of operator is redundant, but makes more sense (meaning &hello_world 
    //and hello_world are the same)

    //calling the function:
    function3(5); //this is an implicilt dereference - implicitly converted to (*function3)(5);

    auto function = hello_world; // equivalent to above
    function(3);

    //alternative 
    typedef void(*func_ptr)(int); // func_ptr is the type in the typedef
    func_ptr test = hello_world;

    /***typedef note:
    typedef int* p  //int* is the type, p is the alias for int*  => p ptr;
    typedef void(*p)(int)  // void(*)(int) is the type, p is the alias for void(*)(int)
    */
    
    std::vector<int> values = { 1,4,3,2,4 };
    for_each(values, print_value); //iterates over each value, and executes print_value on each value

    //alternative to the seperate print_value function -> lambda, anonymous throw-away function)
    for_each(values, [](int value) {std::cout << value << std::endl;}); //iterates over values, and executes print_value on them
    //[] is called captcure method thats how we pass variables in from the outside world

}
