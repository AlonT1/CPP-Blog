/*
generic programming is enabled in cpp by templates - type of macros.
they reduce the repitition of code (DRY - don't repeat yourself principle).
templates are created only at compile-time:
* template functions - stenciled out (created) as concrete functions at compile-time if they are called
* template classes - stenciled out at compile-time as concrete instance only if an object is instantiated
* member functions (inside class T) - stenciled at compile-time out when an object of type T is instantiated
otherwise, if the function/class/member functions aren't called/instantiated,
the template won't even appear in the binary
*/

#include <iostream>

/*THREE OVERLOADs for the same statement!!!*/
void print(int value)
{
    std::cout << value << std::endl;
}

void print(std::string value)
{
    std::cout << value << std::endl;
}

void print(float value)
{
    std::cout << value << std::endl;
}

/*
Template for functions:
templates solve this, cleaning up the repeating code
evaluated at compile time when we call it!! only then it is created in the source code
typename T - the name of the type. templates implicitly identifies the PRIMITIVE parameters through their type. 
instead of typename we can also type "class" - the same!
although it is possible to explicitly call the function with a type: print_template<int>(5) instead of print_template(5)
without calling the function, it won't appear in the machine code!
the template is A TEMPLATE THAT SPECIFIES HOW to create functions with different types - created automatically by
the compiler 


should we use typename T or class T? typename T! why? 
stroustrup originally used class to specify types in templates to avoid introducing a new keyword. The
committee introduced a new keyword typename to resolve ambiguity, but kept class as an option
*/
template<typename T> //T is called a "template type"
void print_template(T value) //"value" is a "template function parameter" or "template parameter"
{
    std::cout << value << std::endl;
}



/*in c templates can be emulated via preprocessor directives, for example:
#define min(a,b) (a < b ? a : b)
a, b can be of any typeand min(a, b) is replaced  by ther ternary(a < b ? a : b)
    at preprocessor stage before compile time(on the level of the source code text).
    this means that parameters are not type - checked and cannot be controlled
    and arguments are not evaluated as expected :
min(++i, ++j) evaluated as(++i < ++j ? ++i : ++j)

The template equivalent:
*/

template <typename T>
T min(T x, T y)
{
    return x < y ? x : y;
}


/*
the call "min(2,4)" (equivalent to min<int>(2,4) due to auto deduction in cpp17)
works will and the function:
int min(int x, int y) { return x < y ? x : y; }
is stenciled out at compile time.

but min(2, 5.5) won't work because the compiler can't deduce the 
type of the template parameter T - is it an int or double?
*/


/*
Template for classes:
As with function templates the compiler generates multiple data and function 
declarations relating to the types requested in class template declarations:
Array<3> will stencil out a an Array class with N replaced by 3
Array<4> will stencil out another Array class with N replaced by 3
this may lead to code bloat in the final source code

// declaration
template <class T>
class C;

// definition
template <class T>
class C {};

another use of templates - similar to the standarrd template library
metaprogramming - programming what the compiler do in the compile time, instead of programming it ourself.
the compiler writes code for us based on the rules of the template
*/
template<typename T, int N> //can also hold multiple typenames. each typename represents a template type
//typename T is a template type, N is a non-template type(parameter) because it is explicilty an int!
class Array
{
private:
    T m_array[N]; //the size of stack-allocated array must be known at compile time!
    /*since templates are created at compiled-time, N will be the size of the array
    N in this case is a sort of a function parameter, a value which is passed into the class.
    N must be known at compile time. why?
    for example: int N=5;  int arr[N] is not possible because N can potentially be changed
    during run-time (unless declared const), and to create the stack allocated array N must be known at compilet time.
    therefore, const int N=5; int arr[N] is valid because N is known at compile time.
    for the same reason, the paramenter int N in the template is also valid, since when we create the array:
    const int N=5; Array<int, N> arr; or  Array<int, 5>  are both valid since the argument for N is
    known at compile time. In addition N doesn't occupy memory, it is a form of a constant value!
    */

public:
    T get_size()const { return N; }
};
//see 100_template_header for more information about template classes and header files


int main()
{
    print_template<std::string>("ma"); //print_template is parameterized with std::string

    //also works in cpp17, the compiler auto deduces the template type
    //from the argument, so no need to parameterize the template type explicitly as <std::string>
    print_template("ma");
    Array<std::string, 5> array; // we parameterize T as an std::string
    std::cout << "Hello World!\n";
    std::vector w{ 1,2,3 };
}

/*
Notes:

1.
    disadvantges associated with templates:
    1. template functions often produce crazy-looking error messages that 
    are much harder to decipher than those of regular functions.
    2. template functions can increase your compile time and code size (code bloat)s,
    as a single template might be “realized” and recompiled in many files
    (there are ways to work around this one). This happens because, in the non-templated
    way, cpp files can use functions defined in some cpp, only by including their forward declarations-
    this satisfies the compiler by letting him know that somewhere, some cpp holds the defnition for the
    function we are calling. The linker will later link the definition with the forward declaration at linking stage.
    This saves binary size because we define functions only once and use them via forward declarations,
    especially crcucial if functions are very long, or used by many files.

    On the other hand, because templates are stenciled out at compile-time, they demand
    that the definition will be available inside the compiled translation unit
    (compilation process happens atomically each file doesn't know whats happening in other cpp files).
    The function definition cannot sit in a different file and be referenced via a forward declaration and linked up
    at linking stage, simply because templates work solely in compile-time, they don't rely on
    linking. Therefore in each cpp file we call a template function/instantiate a template class,
    that translation unit (cpp file) must house the full definition of the class/function,
    so that the compiler will know how to stencil out the class/function when it reaches 
    the cpp file.

    This causes the binary to inflate because each cpp file now holds a complete definition
    of the template class/function.

2.
    note: cpp 17 supports template deduction
    std::array myArray { 9, 7, 5, 3, 1 }; // The type is deduced to std::array<int, 5>
    std::array myArray { 9.7, 7.31 }; // The type is deduced to std::array<double, 2>
    std::array<int, 5> myArray { 9, 7, 5, 3, 1 }; // Before C++17

    // as with std::array, the type can be omitted since C++17
    std::vector array4 { 9, 7, 5, 3, 1 }; // deduced to std::vector<int>



3. 
although function template can be overloaded - this isn't considered an overload
and causes an error:

template <typename T, typename U>       
T multiply (T x, U y) {}

template <typename T, typename U>
T multiply (U x, T y) {}

but these two are valid:

template <typename T, typename U>
T multiply (T x, U y) {}

template <typename T, typename U, typename V>
T multiply (U x, T y, V z) {}


*/