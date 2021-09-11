#include <iostream>



/*
Types bind to (store) expressions of the same type from different value categories.
"binding" is practically done via copy assignment operator (e.g: int x = 4) or list initialization ("int x {y}")
or when passing an expression to as an argument to a parameter of a function (e.g: func(x)).

1. variable of type T can bind to all expressions of type T from any value categories:
 "int x;" can bind to  lvalue expressions such as int y. prvalues such as 4 .
 xvalues such as std::move(y) or std::move(4)

2. variable of type T& can bind only to lvalues (as the name implies, lvalue reference).
e.g: "int& x;" can bind to lvalue expression y.

Notes: 
    1. a call such as func(i) is an ambiguous function call when the following two overloads exist:
    func(int), func(int&) because the lvalue expression i can bind both to type "int" and "int&"
    2. T& is a reference to the primary value category "lvalue" from the glvalue non-primary category
    family. T& cannot bind to an xvalue although xvalue and lvalues are both from the glvalue family.
    lvalue can only bind to lvalue expressions.

3. the same can be said about "const T&" types, it also can bind to expressions from all value categories of type T.
Of course, const T& variables are constant, must be initialized at declaration and cannot be reassigned,
but offers the advantage of passing by reference and saving redundant copies being made.

4. variable of type T&& can bind to expressions from the rvalue non-primary category: xvalues and prvalues:
e.g: "int&& x;" can bind to expression std::move(y) can bind to expression 4 and


for the relationship between passing an object to a function (such as a constructor)
see 25_constructor.
*/



class Entity
{
public:
    int m_x;
    //Entity(int x) : m_x{ x } { std::cout << "Entity (int x) \n"; }
    //Entity(int& x) : m_x{ x } { std::cout << "Entity (int& x) \n"; }
    Entity(const int& x) : m_x{ x } { std::cout << "Entity (const int& x) \n"; }
    Entity(int&& x) : m_x{ x } { std::cout << "Entity (int&& x) \n"; }
};


/*
in the following examples we see which functions (ctors in this case) can accept
specific expressions, based on the type of the parameter in the function, and the
expression we provide to that function as an argument.

note: decltype((e)) (when expression e is parenthesized, decltype yields the value category of the expression)
*/

int main()
{

    Entity a{ 2 }; 
    // expression 2  |  type specifier: int  |  value category: decltype((2)) => int (prvalue)
    //accepting ctors (functions that support a parameter that can bind to the expression we provide as an argument):
    //1. Entity(int x): because x's type specifier is an int 
    //2. Entity (int&& x):
    //3. Entity(const int& x)

    int x = 3;
    //Entity b{ x };
    // expression x  |  type specifier: int  |  value category: decltype((x)) => int& (lvalue)
    //accepting ctors ():
    //1. Entity (int x)
    //2. Entity (int& x)
    //3. Entity(const int& x)


    int& z = x;
    //Entity c{ z };
    // expression z  |  type specifier: int&  |  value category: decltype((x)) => int& (lvalue)
    //accepting ctors (functions):
    //1. Entity (int x)
    //2. Entity (int& x)
    //3. Entity(const int& x)


    int&& y = 4;
    //Entity c{ y };
    // expression y  |  type specifier: int&&  |  value category: decltype((x)) => int& (lvalue)
    //accepting ctors (functions):
    //1. Entity (int x)
    //2. Entity (int& x)
    //3. Entity(const int& x)

    //expression std::move(x)  |  type specifier: int  |   value category: decltype((std::move(x))) => int&& (xvalue)
    //accepting ctors (functions):
    //1. Entity(int x)
    //2. Entity(int&& x)
    //3. Entity(const int& x)
    //Entity d{ std::move(x) };
}