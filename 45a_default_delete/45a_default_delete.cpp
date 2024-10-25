#include <iostream>

class Entity
{
public:
    int x;
    /*
    why do we need to explicitly declare the default ctor if its implicitly given?
    Because if do decalre a custom ctor, such as "Entity(int val)", the default ctor will not be
    implicitly generated. Therefore if want to use it - we must declare it.
    */
    Entity() = default; //expilictly tell the compiler to use the default ctor of Entity

    /*noexcept - specifier telling the compiler the function is guaranteed not to throw any exception.
     can be used in all functions (not just ctor, also in dtors).
     *******************************************************************************************
     noexcept is an optimizations which means “if this throws, terminate the program by calling std::terminate .
     This is useful because the possibility of exceptions has some overhead.
     But the cost of crashing migh be too high.
     If your not shure if this function will throw in the future I would not use noexcept on it.
     As Jason Turner puts it. You should not sprinkel it around like const.
     *******************************************************************************************
     According to the C++ Standard, dtors are implicitly marked noexcept(true).
     This behavior was introduced with C++11 to make destructors safer and to encourage more predictable exception handling.
     Note that noexcept == noexcept(true)
     noexcept(false) - the function may throw exceptions - this is the default for functions
    */
    Entity(int val) noexcept : x{val} {}

    Entity(const Entity& other) = delete; //disables copy ctor (cpp11)
    //in older cpp, the copy ctor was moved to private, this solution isn't
    //sufficient since the copy ctor can still be invoked in-class
};

void test(Entity e)
{

}

int main()
{
    std::cout << "Hello World!\n";
    Entity e; // uses the default ctor
    //Entity e1 = e; copy ctor is deleted - this statement invokes an error
    //test(e); error - since passing e to test invokes the copy ctor which is deleted
}
