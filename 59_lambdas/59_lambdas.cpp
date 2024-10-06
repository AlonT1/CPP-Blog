#include <iostream>
#include <vector>
#include <algorithm> // for std::for_each
#include <functional> //for std::function



void for_each(const std::vector<int>& values, void(*func)(int)) 
{
    for (int value : values)
        func(value);
}

//void(int) - function that gets an int, returns void
void for_each2(const std::vector<int>& values, const std::function<void(int)>& func)  
{
    for (int value : values)
        func(value);
}

int main()
{
    std::vector<int> values = { 1,5,4,3,5,4 };
    /*
    lambdas - an unnamed function object capable of capturing variables in scope.
    lambdas are syntactic sugar for classes that act like a function (functors see 58a_functor for details).
    this means that we can use lambdas in a similar syntax as we use functions,
    but as opposed to functions, lambdas are able to preserve the utilize variables
    that exist outside the lambda and keep track of their state between calls (shown in example 2) .

    lambda form:

    [ captures ] ( parameters ) lambda-specifiers -> trailing-return-type { body }	

    1. captures is optional
    2. parameters are optional
    3. lambda-specifiers are optional (mutable, const, constexpr)
    4. trailing-return-type can be automatically deduced by the compiler
    5. body is optional
    
    the most stripped down lambda looks like this:*/

    auto lambda_function = []() {}; 
    //even more stripped down: "[]{}" missing () if the lambda doesn't take any parameters.
    //empty "captures", no parameters and no body. return type is deduced to "void" because
    //the statements in the body (which do not exist) do not suggest that the function returns something.


    /***********************Lambda example 1 (no captures)*************************/
    auto lambda1 = [](int value) -> void {std::cout << value << std::endl;};
    /* Notes:
    
    1. return type is void and can be automatically deduced by the compiler
    according to what the statements in the body actually return (if any).
    hence the statement above is equivalent to:*/
    auto lambda2 = [](int value){std::cout << value << std::endl;};

    /*2. lambdas are syntactic sugar for functors (shown below), they are translated
    to a user defined class whose name is known only at compile time. For instance,
    in the example below, the type of the lambda is __lambda_6_18 (a compiler-generated class).
    therefore we have 2 options:
        a. use auto which is deduced at compile time, at the same time that the lambdas are generated
        as a functor and given a name.
        b. assign the lambda to a function pointer: fp is a function ptr to function accepting int, returning void*/
    void(*fp)(int) = [](int value) {std::cout << value;};

        /*the example above only works with non-capturing lambda for the reason expalained below
        c. use std::function (requires to include <functional> header) which is a general-purpose function wrapper:
        note: std::function<void(int)> means a function that accepts an int and returns a void:*/
    std::function<void(int)> lambda3 = [](int value) -> void {std::cout << value << std::endl;};

    // although the lambda is a class behind the scenes, we can still us it as if it was a function.
    //this is possible because the class of the lambda overloads operator() as shown below.
    lambda1(4);


    /*if we want to pass the lambda to a function "func" we can specify "func" to have a
    parameter of function pointer. But how does this work if a lambda is a class behind the scenes?
    The compiler generates a conversion member function inside the functor class that represents the lambda
    behind the scenes. This conversion function handles what happens when the lambda is assigned
    to a function pointer. The conversion function is demonstrated below, it essentially returns
    the address of one of the functor's member function that represents the body of the lambda,
    this address is later assigned to the function pointer.

    But there is a catch...

    This conversion happens ONLY WITH NON-CAPTURES lambdas (lambdas who don't capture variables
    from the local scope as shown in example 2), therefore only this type of lambdas can be assigned
    to function pointers

    *********************************************************************************************
    tl;dr: non-capturing lambdas are self contained, they do not have class members, they simply
    hold a function, and by passing these types of lambdas to a function, the class that represents
    the lambda has conversion function when that returns a function pointer to the method that holds
    the function body, therefore this lambdas can be passed to a function that accepts a regular func pointer.
    As opposed to capturing lambdas who have class members (the captured variables)
    therefore, they are more complex (contains a method that r/w class members), therefore
    they require to be passed to a function that accepts std::function.
    *********************************************************************************************

    To understand why, we need to understand the difference between capture and non-capture lambdas
    behind the scenes. Both are represented by a class (a functor) that acts like a function (because
    they override operator()).

    The difference: non-capture lambdas behind-the-scenes class is a class that conceptually
    has only one function that represents the body of the lambda (have the same statements)
    (and an additional conversion function to support passing the lambda object to functions).
    So basically we have a class with functions)

    On the contrary, capture lambdas behind-the-scenes class also has HAS MEMBER VARIABLES that
    represent the captured variables (some are references (if captured by ref) and some
    are copies (if captured by value). 
    The problem is that the function that represents the body of the lambda, performs r/w
    operations on the member variables (the captured data) (also called state-preserving lambdas,
    they "remember" the values of these members between consequtive lambda calls.)
    Therefore, the "body-function" isn't self contained as it is in the non-capture lambda.
    And because we can't pass the address of the "body-function" AND the class member variables
    (that the function manipulates) together to a function pointer - THERE IS NO CONVERSION FUNCTION FOR captured lambdas,
    as opposed to the existing conversion function of the non-capture lambdas which simply returns
    the address of the "body function" to a function pointer that the lambda is being assigned to.

    Also note the following from the standard:
    A lambda can only be converted to a function pointer if it does not capture,
    from the draft C++11 standard section 5.1.2 [expr.prim.lambda] says (emphasis mine).
    https://stackoverflow.com/a/28746827


    So how do we pass a capturing lambda to a function?
    we can use std::function which is a function wrapper.
    shown in example2 with function for_each.
    */

    //lambda1 is non-capturing, can bind to a function pointer, thanks to the built-in
    //compiler generated conversion function (shown below)
    for_each(values, lambda1);


    /*lambda above is equal to the following compiler generated code (cppinsights.io):
    lambdas are functors (read 58a_functors - classes that acts like functions) behind the scenes:*/

    class __lambda_6_18
    {
    public:

        inline void operator()(int value) const //overlading operator(), emulating function call
        {
            std::cout.operator<<(value).operator<<(std::endl);
        }

        /*
        conversion function! (exists to support the abillity of passing the functor (a.k.a lambda) as an
        argument to a function that takes in a function pointer:
        when the "lambda" object is assigned to a "void(*)(int) variable"
        (happens when passing the "lambda" object to a function that has a parameter "void(*)(int) func"; :
        when the compiler tries to assign (and convert) "lambda" (of type __lambda_6_18) to "func" (of type void(*)(int)), 
        this conversion function will execute and it will return the address of the __invoke function that represents
        the body of the lambda ("the actual statements"). This results in function ptr "func" being assigned 
        with the address of the "_invoke" which is static function. Then, when executing function pointer "func"
        via "func();" we will actually execute "__invoke()" function*/
        using retType_6_18 = void (*)(int);
        inline operator retType_6_18 () const noexcept
        {
            return &__invoke;
        };

    private:
        static inline void __invoke(int value)
        {
            std::cout.operator<<(value).operator<<(std::endl);
        }
    };

    __lambda_6_18 lambda = __lambda_6_18{}; //initialiazing a lambda object
    lambda.operator()(4); // equals to lambda(4); //function call
	


    /*********************More "complex" version of a lambda (with captures)**********************/
    int a = 2, b = 5;

    //std::function <void(int)> means that the function accepts an int and returns a void
    std::function <void(int)> lambda_x = [&a, b](int value) mutable ->
        void {a = 4; std::cout << value << b << ' ';};

    /********************************************************************************
    Capturing lambdas CANNOT be assigned to function pointers for reasons explained above.
    they need to be assigned to an std::function wrapper, and can only be passed
    to functions that accept an std::function
    ********************************************************************************/


    /*Captures explained:
  
    "To capture" means to put a copy or a reference to a variable into a lambda object itself.
    The whole reason for the capture machinery is so that the lambda can locate
    variables and safely use them, even if they have a time-varying lifetime.
    
    in the example above we captured "a" by ref and "b" by value ([&a,b]).
    note that "b" is considered read-only, meaning it is unmodifiable, as opposed
    to "a".

    mutable lambda-specifier explained:

    the local variables "a" and "b" exists outside the scope of the lambda,
    and by capturing them we make them available to be read by the function.
    if we want to write (modify) the captured variables we must use the mutable
    lambda-specifier.

    we can capture multiple variables, separated by comma.

    1. captures by reference:
        Using "&" passes variables in the current scope by reference.
        in the example above we pass "a" by reference to the lambda, this means
        that the "a" we use in the body of the lambda IS the "a" that exists
        outside the lambda (an alias, same memory-wise).
        captures in the form of: [&] means capturing ALL the variables in the 
        current local scope by ref and making them avaible to use in the lambda
        Any changes to "a" within the body of the lambda affect variable "a"
        outside of the lambda.


    2. captures by value:
        = pass all variables in the current scope by value.
        "b" is captured by value in the example above. changing "b" in the body
        of the lambda won't affect the "b" that exists outside the function.
        using [=] captures ALL local variables by value, making them available
        inside the function.

    we can also declare a or b as static, this will render them visible in the lambda
    this works because static variables (local or global) have fixed memory location
    which is known at compile time and doesn't change, and because identifier lookup within the lambda finds an 
    object with static lifetime in an enclosing scope, so it gets used.
    The lambda  is sure that the object won't die so the captures mechanism is not necessary
    (its all purpose is to capture vars and use them safely, regardless of their lifetime).
    */


    /*lambda above is equal to the following compiler generated code (cppinsights.io):
     lambdas are functors (read 58a_functors - classes that acts like functions) behind the scenes:*/

    class __lambda_9_40
    {
    private:
        int& a; // a is captured by ref
        int b; //b is captured by value

    //ctor (takes the values by ref to save copying)
    public:
        __lambda_9_40(int& _a, int& _b) : a{ _a }, b{ _b } {}

        //the "body function" actual body of the lambda
        inline void operator()(int value)
        {
            a = 4;
            std::cout.operator<<(value).operator<<(b).operator<<(std::endl);
        }
    };

    lambda_x.operator()(8);



    /*because there is no conversion function within capture-lambdas behind-the-scenes (reason explained above)
    they can only be passed to functions that accept std::function */
    for_each2(values, lambda_x);




    /*********************Lambda Interaction with STL**********************/

    //find_if takes a predicate (functions that returns a boolean value) and returns an iterator
    //to the 1st element that the predicate retuns true.
    //*find simply compares (==) to a value
    auto result = std::find_if(values.begin(), values.end(), [](int value) {return value > 3;});
    if (result != values.end())
        std::cout << *result << std::endl;

    lambda_x(35454); // lambda can be called like a regular function!



    //even better! std::for_each works the same as the custom for_each above!
    //the parameter "int value" is the current iterated "int" value from "values" int vector
    //(equivalent to "for value: values"):
    //the "first" pointer (values.begin()) points to 1 and the "last" pointer (values.end()) points to one item
    //after the end of the array (one item after 4). 
    std::for_each(values.begin(), values.end(), [](int value) -> void {std::cout << value << std::endl;});

}