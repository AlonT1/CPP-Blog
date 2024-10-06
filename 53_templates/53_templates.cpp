/*
generic programming is enabled in cpp by templates - type of macros.
they reduce the repitition of code (DRY - don't repeat yourself principle).
templates are cpp entity created only at compile-time:
* template functions - stenciled out (created) as concrete functions at compile-time if they are called
* template classes - stenciled out at compile-time as concrete instance only if an object is instantiated
* member functions (inside class T) - stenciled at compile-time out when an object of type T is instantiated
otherwise, if the function/class/member functions aren't called/instantiated,
the template won't even appear in the binary
*/

#include <iostream>
#include <array>

/*********************************Template functions**************************/


/*THREE OVERLOADs for the same statement!!!*/
int returner(int value)
{
    return value;
}

std::string returner(std::string value)
{
    return value;
}

float returner(float value)
{
    return value;
}

/*
* ********************************Template for functions*****************************

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
T returner_template(T value) //"value" is a "template function parameter" or "template parameter"
{
    return value;
}

//now calling the function as:
int res1 = returner_template<int>(5); //the compiler stencils out the following concrete function:

/*
template<>
int returner_template<int>(int value)
{
    return value;
}
*/

/*
the above is a concrete form of the template, also called SPECIALIZATION of the
template. note that once a specialization is created for an int, it will be
created only once and will serve all function calls with type int.
we can also manually specialize templates in the same way that the compiler does.
more info in 101_template_specialization.

with functions, cpp supports template argument deduction.
since the argument is an int the compiler deduces the template type as an int:*/
int res2 = returner_template(5);



/*in c templates can be emulated via preprocessor directives, for example:
#define min(a,b) (a < b ? a : b)
a, b can be of any typeand min(a, b) is replaced  by ther ternary(a < b ? a : b)
    at preprocessor stage before compile time(on the level of the source code text).
    this means that parameters are not type - checked and cannot be controlled
    and arguments are not evaluated as expected :
min(++i, ++j) evaluated as(++i < ++j ? ++i : ++j)

The template equivalent:
*/

// default template argument: if the template type is not specified during
// template instantiation, (e.g boo(3) instead of boo<int>(3)), or if
// the compiler cannot deduce the the template type from the function call argument
// then the template will revert to the default template type.

template <typename T = int> 
T min(T x, T y)
{
    return x < y ? x : y;
}


/*
the call "min(2,4)" (equivalent to min<int>(2,4) due to type deduction)
works will and the function:
int min(int x, int y) { return x < y ? x : y; }
is stenciled out at compile time.

but min(2, 5.5) won't work because the compiler can't deduce the 
type of the template parameter T - is it an int or double? 
solution: use two typenames T and U, but which one would be the return value?
this question is answered in (108 auto_if_constexpr) - in short use auto,
as a return type, the compiler will deduce the return type at compile time
based on the underlying type of the return value.*/




/*********************************Template Classes**************************/
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

example:
*/
template<typename T, int N> //can also hold multiple typenames. each typename represents a template type
//typename T is a template type, N is a non-template type(parameter) because it is explicilty an int!
class Array
{
private:

    /*the size of stack-allocated array must be known at compile time!
    since templates are created at compiled-time, N will be the size of the array
    N in this case is a sort of a function parameter, a value which is passed into the class.
    N must be known at compile time. why?
    for example: int N=5;  int arr[N] is not possible because N can potentially be changed
    during run-time (unless declared const), and to create the stack allocated array N must be known at compilet time.
    therefore, const int N=5; int arr[N] is valid because N is known at compile time.
    for the same reason, the paramenter int N in the template is also valid, since when we create the array:
    const int N=5; Array<int, N> arr; or  Array<int, 5>  are both valid since the argument for N is
    known at compile time. In addition N doesn't occupy memory, it is a form of a constant value!*/
    T m_array[N]; 

public:
    //**************************************************************************************
    //when denoting the name of the class ("Array") it is implicitly equivalent to "Array<T,N>",
    //which can be written explicitly.
    //**************************************************************************************
    void test(Array& array) {}
    T get_size()const { return N; }
};
//see 100_template_header for more information about template classes and header files



Array<int, 5> arr; //equivalent to "Arra<int,5>

/*
template<>
class Array<int, 5>
{

private:
    int m_array[5];

public:
    inline void test(Array<int, 5>& array); //specialized in the same way as template functions

    inline int get_size() const;

    // inline Array() noexcept = default;
};
*/

/*
The above is a concrete form of the template, also called full specialization - 
can also be done manually (as with functions). template classes can also be partially
specialized as opposed to template functions, as seen in 101_template_specialization.

as seen above, template type deduction for functions is supported, and since cpp17
class template type deduction (CATD) is also supported:
*/
std::array array1{ 4, 5, 6 }; //the template arguemnt is deduced as <int, 3>, 
// i.e the type is int, and the size of the array 3.

//custom example for CATD:
template <typename T, typename W>
class Array1
{
public:
    Array1(const T& t, const W& w)
    {
        std::cout << t << w;
    }
};

Array1 arr(4, 5); // automatically deduced to "Array1<int, int> arr(4,5)"

/*the template is specialized as the following:
template<>
class Array<int, int>
{

  public:
  inline Array(const int & t, const int & w)
  {
    std::cout.operator<<(t).operator<<(w);
  }
};

*/


int main()
{
 
  
}



// static variables:

template <typename T>
struct S
{
    static double something_relevant;
};

template <typename T>
double S<T>::something_relevant = 1.5;


/*
for   "S<int> s;"
the compiler generates the following code:

template<>
struct S<int>
{
  static double var;
};

double S<int>::var = 1.5;

*/

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


4. if a function inside a template class isn't used, it isn't instantiated!!!!


5. Templates are a means to create classes/functions. If we want a class or function
to work with all cases we need to template them. If we want a template function/class 
to work differently given a specific template parameter, we need to specialize the
function/class (give the template a concrete definition).


6. automatic template deduction, always evaluates to the type of the object - 
a simple lvalue (e.g: "int") regardless if we provide the call with a prvalue, xvalue or lvalue.

7.
when we specify a template parameter - the generated template recieves
the type as-is (like a preprocessor):

if foo is a function: foo<int&>(x) //remember that x is an identifier for an already-created object
// and thus an lvalue expression.
the generated template will be:
template<>
void foo<int &>(int &x) {}

i
*/