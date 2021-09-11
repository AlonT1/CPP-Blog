#include <iostream>
class Entity
{
private:
    int m_num;

public:
    Entity(int num) : m_num( num )
    {
    }

    int get_value() { return m_num; }
    
    /*
    void add(int value) { m_num += value; }
    void sub(int value) { m_num -= value; }
    void mult(int value) { m_num *= value; }
    */
    
    Entity& add(int value) { m_num += value; return *this; } // dereferncing "this" pointer retrieves the actual data of "this"
    //and returning it by reference means that we return the current object (an alias)
    Entity& sub(int value) { m_num -= value; return *this; }
    Entity& mult(int value) { m_num *= value; return *this; }

};


int main()
{
    Entity entity(1);
    entity.add(4).mult(2).sub(1);
    /*
    Evaluated by:
    1. entity.add(4) - > the same object that was incremented by 4 is returned
    2. "same object from 1".mult(2)
    3."same object from 2".sub(1)
    */
    std::cout << entity.get_value() << std::endl;
}