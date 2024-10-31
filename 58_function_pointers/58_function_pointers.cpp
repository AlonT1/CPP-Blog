#include <iostream>
#include <vector>
/*
* https://opensource.com/article/21/2/ccc-method-pointers
function pointers are a way to assign a function to a variable. traditionally, 
we only call functions as a symbol, without doing any logic on them.

are very useful when we want some function to take in some specific behaviour (a specific function)
to perform an action (a sort algo, that takes in a function (ascending/descending) to decide how to sort)
functions are cpu instructions. when we compile are code, any function gets
compiled into cpu instruction and they rest in the exe (binary). 

function pointer -  find the function from in exe and get the memory address of
the instruction.
*/

void hello_world(int x)
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

struct Foo
{
    void func(int x)
    {
        std::cout << "shalom11 " << x << '\n';
    }
};

int main()
{
    std::cout << hello_world << '\n'; //prints the address of hello_world - has an address, just like a variable!
    //the address of a func is where its code starts (compiled functions are stored in the "code" segment of the 
    // memory layout, separated from the stack and heap
    std::cout << &hello_world << '\n'; // identical to above

    void(*function1)(int) = &hello_world; //"hello_world" is identical to "&hello_world".
    //cpp implicitly converts "hello_world" to &hello_world if "hello_world" is used.
    //i prefer &hello_world because pointers point ti addresses 
    //according to the rules in 30a_reading_declerations:
    //function1 is a pointer to a function with int paraemeter returning void
    //THE TYPE IS: void(*)(int) and THE POINTER NAME IS: function1
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
    typedef void(*func_ptr)(int); 
    func_ptr test = hello_world;

    /***typedef note:
    typedef int* p  //int* is the type, p is the alias for int*  => p ptr;
    typedef void(*p)(int)  // void(*)(int) is the type, with using typedef p is not
    a pointer to a function but rather becomes an ALIAS for void(*)(int)

    //even better "using func_ptr = void(*)(int)". modern & clearer assignment
    */

    /**************pointers to a method (member functions) ********/
    void (Foo::* func_ptr1)(int) = &Foo::func; 
    // void (Foo::* func_ptr1)(int) means: func_ptr1 is a pointer to a member function of a class Foo that is 
    // a function that accepts int and returns a void
    // Note that this "    void (Foo* func_ptr1)(int) = &Foo::func;" won't work, we have to use the scope resolution
    // operator ::, because the latter declares a pointer to a Foo object, while the former declares
    // a pointer to a member function and indeed we want a pointer to a function, not to a to some object
    Foo foo;
    (foo.*func_ptr1)(5); // The trick: foo is an instance. member methods belong to an instance, not
    // to the class. therefore, when we invoke a member function we must do this through an instance:
    // foo.some_function(), some_function is the address of the function(probably) and we invoke it.
    // since *func_ptr1 returns the address of the function, by invoking foo.*func_ptr we MIMICK
    // a regular function call.
    // the parentheses on (foo.*func_ptr1) is mandatory!!! probably because "foo.*func_ptr1(5)" is parsed
    // foo.*(func_ptr(5)) because function call operator () has higher precedence than pointer-to-member operator
    // otherwise, without parentheses, "foo.*func_ptr1" is interpretd as a name of a function (?).
    // 
    // ****************************************************************
    // with funnction pointers to member functions we need to know:
    // 1. the class name (used as namespace)
    // 2. the parameters and return values
    // 3. an instance of the class through which we invoke the function
    // 
    // ordinary function pointers ONLY store the address of the function! 
    // here we need the additional components listed above, for which 
    // a simple function pointer doesn't have an answer alone.
    // https://stackoverflow.com/a/1486048
    //*****************************************************************

    
    std::vector<int> values = { 1,4,3,2,4 };
    for_each(values, print_value); // iterates over each value, and executes print_value on each value

    //alternative to the seperate print_value function -> lambda, anonymous throw-away function)
    // [](int value) {std::cout << value << std::endl;}  // is the lambda
    // function that accepts a "value" and prints it
    // [] is called captcure method thats how we pass variables in from the outside world and use them in the body of the lambda
    void(*theLambda1)(int) = [](int value) {std::cout << value << std::endl;}
    // "for_each" is the function above (not std::for_each)
    // "for_each" accepts "values" array, loops over "values" and feed each value into the lambda (which accepts "value" and prints it)
    for_each(values, theLambda1);
    // return type of lambda is based on the type of the value we return
    int (*theLambda2)(int) = [](int value){return value;} // since x1+value is int then the return value is an int;
    // but we can also explicily specify return type using -> syntax:
    int (*theLambda3)(int) = [](int value) -> int {return value;} // since x1+value is int then the return value is an int;

    // function pointers cannot point to lambdas that captures variables from the outside world, for this we need std::function
    // (see 59a_function_pointers_lambdas_example)
    // in short
    int x = 10;
     // Capturing x as reference. "<int(int)> the left int is the return param and (int) is the accepting arg.
    std::function<int(int)> func = [&x](int a) { return a + x; };
}
