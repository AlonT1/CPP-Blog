/*
in CPP 3 visibility modifiers exist: private, protected, public. They don't affect performance don't call things you shouldn't
changing only private x,y of a button ui maybe not enough -> refresh is needed -> shouldn't assign x,y directly
better to use setposition(). it also aids the programmer - our intention for encapsulation. (getter/setters)
*/

#include <iostream>

struct Entita
{
//public: // implicit
};


//friend -  friend function can access private members of the class through the object whose the function is friends with
class Entity
{
private: //implicit - only this entity class can r/w to this variables, all dervied (inherited) + instances can't acess private members
    int x, y;
protected: //more visible than private less than public - derived classes (inherited) can access, instances can't
    void print_test()
    {

    }
public:
    void public_print() //accessible through all (in base, in derieved, in instances)
    {

    }
    Entity()
    {
        x = 0;
    }
};

class Player : public Entity
{
public:
    Player()
    {
        //x = 2; inaccessible! private in base class
        print_test(); // accessible protected in base class
    }
};

int main()
{
    std::cout << "Hello World!\n";
    Entity e;
    //e.x // private
    //e.print_test(); //protected
}
