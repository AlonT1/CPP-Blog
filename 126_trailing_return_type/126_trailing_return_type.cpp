
#include <iostream>

//instead of the classic syntax:
int func(double x, int y) {}

//we can write
auto func1(double x, int y) -> int {}


int main()
{
    std::cout << "Hello World!\n";
}