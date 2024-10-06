//https://stackoverflow.com/a/24101297

/*
A dependent name is a name that depends on the type or the 
value of a template parameter. when the template parameter
represents a type we use "typename".examples:

1. https://www.youtube.com/watch?v=mNxAqLVIaW0&t=550s
at typename example of "typename iter::value_type" enabled by std::vector in the video to 140_dependant naes
EXPANNNNNNNNNNND

2. examples below:
*/

#include <iostream>
#include <vector>

struct X
{
    typedef int foo;
};

struct Y
{
    static constexpr int foo = 123;
};

template <typename T>
void func1(int y)
{
    T::foo* y; // no need for typename, T::foo represents a literal
}

template <typename T>
void func2()
{
    typename T::foo* y;
}

namespace N 
{
    template<class T>
    struct X
    { 
    };
}

int main()
{
    // Works! the compiler parses T::foo as Y:foo and Y:foo is not a type but rather is a literal
    // value: the standard states that const static ints are literal values, for example 123 and foo
    // are interpred as the same thing (prvalues). therefore the statement "T:foo* y" is interpreted
    // as "123 * 2" (multiplication).
    //******************************************************************************
    // whenever a template parameter represents a type, we prefix the template parameter with "typename"
    //******************************************************************************
    func1<Y>(2);

    // Works! "typename" denotes that "T::foo", interpreted as "X::foo" is not a "static const int"
    // but rather represents a type (i.e typename) - foo is a type (int)! therefore
    // y is apointer of type int
    func2<X>();

    N::X<int> a; // ...  legal
    N::template X<int> b; // (K), legal
    //typename template    X<int> c; // (L), ill-formed
    typename int x = 4;

}

// template????????????????