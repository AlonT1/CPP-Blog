/*
implicit conversion (construction). Objects can be constructed implicitly through assignment
as long as there is a compatible constructor for that assignment. only fit for single variable ctors (?)
implicit conversion pointers can be assigned only to class references (no pointers!)
*/

#include <iostream>

class Entity
{
private:
    std::string m_name;
    int m_age = 4;
    int x = 2;
public:
    Entity() {};
    Entity(const std::string& name) : m_name(name), m_age(-1) { std::cout << "shalom" << std::endl; }
    explicit Entity(int age) : m_name("Unknown"), m_age(age) {} //explicit constructor
    Entity(const char*& c) : m_name(std::string(c)), m_age(-1) {}
    Entity(Entity* ptr) { m_age = 5; }
    ~Entity()
    {
        std::cout << "destroyed" << std::endl;
    }

};

int main()
{
    Entity a = std::string("shalom"); 
    //copy initialization via copy ctor through implicit type conversion of rvalue rhs where lvalue is not-yet-created
    //explained in 25_constructor

    //Entity e = 4; // the ctor  Entity(int age) is marked explicit! therefore this statement won't work:
    //the copy initialization via copy ctor through implicit type conversion of temp rvalue rhs won't work
    //note that making a constructor explicit only prevents
    //implicit conversions.Explicit conversions(via casting) are still allowed :
    //std::cout << static_cast<Entity>(5);  //we can't convert 5 into an Entity object with age member that stores 5

    std::cout << "Hello World!\n";
}

