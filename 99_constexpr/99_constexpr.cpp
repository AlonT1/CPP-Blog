/*
constexpr guarantees compile-time constants (and therefore offers compile-time optimizations).
const variables aren't necessarily compile-time (e.g: const vars an be initialized 
with a value from an std::cin known only at runtime, or be initialized with a return value
from a function, known only at run-time). in addition it possible to const cast (remove const)
from a constant or declare a member in a class as mutable, which enables to modify it in a const function.
const pretains to offer a "lock" for a variable and at most simple optimization such as 
replacing all const vars throught the program with the constant value they stored.

why the compile-time optimizations constexpr offers are interesting?
No runtime cost:
• No execution time
• Minimal executable footprint
• Errors found at compile or link time
• No synchronization concerns

*/

#include <iostream>
#include <chrono>

/*
constexpr function-
telling the compiler to evaluate constexpr function calls at compile-time
constexpr functions can only call other constexpr functions
*/
constexpr int product(int x, int y)
{
    return (x * y);
}

constexpr long int fib(int n)
{
    return (n <= 1) ? n : fib(n - 1) + fib(n - 2);
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
   //const keyword is crucial to ensure that the return value
   //from the constexpr function is indeed constexpr
    //int res =  product(2, 5);
   // calls constexpr function, but compile-time evaluation won't occur
   //becuase res2 isn't constexpr
   //int res2 = product(2, 5); 
    constexpr int res2 = fib(8);
    auto end = std::chrono::high_resolution_clock::now();
   //std::chrono::duration<float>(d)
    std::cout << std::chrono::duration<double>(end - start).count();
}