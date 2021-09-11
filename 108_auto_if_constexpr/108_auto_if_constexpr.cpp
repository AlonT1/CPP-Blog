#include <iostream>
#include <map>


/*
auto specifier - the type of a variable initialized or retuned from a
function is automatically deduced at COMPILE-TIME! (no run-time performance cost)

auto return type works well with templates - they are both evaluated at compile time.
for example, the following calls produce different types:
1. multiplty(5, 4) -> the template types are both parameterized as int and an int is returned.
2. multiply(5, 1.5) -> T is parameterized as an int and U is parmeterized as a double,
the calculation becomes -> 5 * 1.5 -> 5 undergoes promotion to the floating type (double) ->
5.0 * 1.5 = 7.5 -> a double is returned

so in call 1 a INT is returned and in the 2nd a DOUBLE. Therefore the return type can't be
fixed into either type T or U. for example a return type T in the second call, would
cause the function to be stenciled out with an int return type, and when will try to return
7.5, it will undergo conversion to an int (the return type), causing loss of data (7 will be returned).

the naive solution -  two template functions where the 1st returns T and 2nd returns U
BUT! - function in cpp cannot differ only in return type -
this is not considered function overloading which inspects differences in parameters list
and in the case of the template (the arugments are the same - T and U) in both cases.
- the compiler will complain that these 2 functions are the same, making this solution invalid.


The proper solution:
Thus, an auto return is appropriate -  because it is automatically deduced per each case -> but how?
During compilation the compiler evaluates the calls and their outputs and decides
to produce two different definitions for multiply() to accomodate our function calls.
The compiler sees that:
1. the arguments are of different type between calls (the 1st call (int,int), the 2nd call (int, double)
2. the return types are different (1st call reutrns int, 2nd call returns double)
Therefore, the compiler will stencil out two functions from the template below:

function 1: int multiply (int value1, int value2) {return value1 * value2;}
function 2: double multiply (int value1, double value2) {return value1 * value2;}

note that using auto can cause the deduction of the following functions:
for the call multiply(1, 2.5): double multiply (int value1, double value2) {...}
for the call multiply(2.5, 1): double multiply (double value1, int value2) {...}
its safe to assume that calling multiply (1, 3.5) will utilize the function generated
when we called (1, 2.5) and the compiler won't generate the same function again.


*/

template <typename T, typename U>
T multiply(T value1, U value2 )
{
    return value1 * value2 ;
}

template <typename T, typename U>
T multiply(U value1)
{
    return value1;
}


template<unsigned int dim> //denoting the dimensions of the vector (compile-time eval.)
class Vector
{
public:
    double m_vector[dim];

    //args is a template parameter pack whose type is a T universal (forward) reference.
    //This means that:
    //1. T&& will become either an lvalue ref or rvalue according to the value category
    //of each one of the values in parameter pack args (they can be either prvalues, lvalues or xvalues).
    //2. This ctor is a variadic function: "args" can "take in" infinite number of values as a parameter pack (
    //3. the values that "args" can take in must all be of type T (a single type), although the compiler
    //is smart enough to perform promotions to match different types, for example: despite that
    //the values (3.14, 5, 6) are double, int, int - the compiler will promote the ints to doubles 
    //to match a single type (read 69_casting)

    //With param. pack we directly assign pack to the vector (at compile time!), 
    //while std::init_list requires #include + looping the initializer list (105_std_initializer_list)

    template <typename ...T> //template parameter pack T
    Vector(T&&... args) 
        : m_vector{ args... } {} //vectors can be assigned with unpacked(... on the left) parameter pack!
    //note: in other words, args... is "expands" the pack.

    float operator[] (unsigned n) const
    {
        return m_vector[n];
    }


    /**********Constexpr and auto + templates
    tl;dr auto return type (evaluated at compile-time) can't be deduced from an if-else branches that return
    different types, because if-else are evaluated at run-time. "if constexpr" solves
    this by allowing to evaluate if-else at compile-time, thus we would know the return type
    also at compile time.

    Note: "if constexpr" is also relevant when using "auto" without the context of a template.
    both auto and template are evaluated at compile-time and "if constexpr" works well
    with both of them (example of templates and constexpr without auto can be found in
    111_type_traits where we use "if constexpr" to evaluate which function to select
    according the type parameter given to a templated function.

    cross_product() explained:

    because the cross product of 2 vectors of dim. 2 is a scalar (float)
    and because the cross product of 2 vecors of dim.3 is a vector 3
    cross_product() can return two_different types.
    The naive solution is to mark the return type as auto, but there's a problem
    (assuming that constexpr is missing at the code below):

    when the compiler stencils out a class of Vector<2> and a Vector<3>
    and tries to deduce the return type of cross_product() for each of these classes,
    the compiler does not know which type to deduce. This happens since if-else
    statements are ONLY EVALUATED AT RUN-TIME, and because the templates are generated
    at compile-time, the compiler can't gain any useful information from if-else statements
    about the possible return type. if there was a simple return "x"; like in the previous
    examples above, the compiler would have deduced the return type successfully.

    The Solutions:
    1. function overload - cross_product() which takes in Vector<3> and another
    cross_product() which takes in vector<2>, each return the appropriate type
    without if-else statements.

    2. THE BEST SOLUTION - "if constexpr" (cpp17) - allows evaluation of if-else
    statements at compile-time! when stenciling out Vector<2> and trying to deduce
    the return type of cross_product, the compiler can evaluate the branching of
    the if-else statements at compile time and decide the because dim==2, the return
    type of cross_product in the stenciled class Vector<2> will be a float!
    for Vector<3> the return type will be a Vector<3>.


    */
    auto cross_product(const Vector& other_vector)
    {
        if constexpr (dim == 2)
            return m_vector[0] * other_vector[1] - m_vector[1] * other_vector[0];
        else if constexpr (dim == 3)
            return Vector<3>{
                m_vector[1] * other_vector[2] - m_vector[2] * other_vector[1],
                m_vector[2] * other_vector[0] - m_vector[0] * other_vector[2],
                m_vector[0] * other_vector[1] - m_vector[1] * other_vector[0]
            };
    }
};

int main()
{

    //**************auto in the context of automatic type deduction of a variable
    std::map<int, int> m{ {1,100}, {2,200} };
    //the type is automatically deduced at compile-time (no run-time performance cost)
    auto iter = m.begin(); 
    //equivalent to:
    //std::map<int, int>::iterator iter = m.begin();

    //**************auto in the context of automatic type deduction of a function retrun value
    std::cout << multiply (5, 1.5);


    //**************auto in the context of a constexpr + templates
    double x = 4;
    Vector<2> xy1{ x, 2.0 };
    Vector<2> xy2{ 2.0, 3.0 };
    Vector<3> xyz1{ 1.0, 2.0, 3.5 };
    Vector<3> xyz2{ 2.0, 3.0, 4.5 };

    float res1 = xy1.cross_product(xy1);
    float res2 = xy2.cross_product(xy2);

    //note: auto cannot be used for function parameters
}   

//https://www.youtube.com/watch?v=wrwwa68JXNk&t=49s
//https://stackoverflow.com/questions/43434491/difference-between-if-constexpr-vs-if