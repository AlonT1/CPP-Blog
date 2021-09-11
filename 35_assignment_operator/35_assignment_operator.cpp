#include <iostream>

class Foo
{
public:
    Foo() {}
    Foo(int x) { std::cout << x << std::endl; }
};

int main()
{
    Foo foo = 1; // 
}