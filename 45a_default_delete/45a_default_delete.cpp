#include <iostream>

class Entity
{
public:
    int x;
    Entity() = default; //expilictly tell the compiler to use the default ctor of Entity
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
    Entity e;
    //Entity e1 = e; copy ctor is deleted - this statement invokes an error
    //test(e); error - since passing e to test invokes the copy ctor which is deleted
}
