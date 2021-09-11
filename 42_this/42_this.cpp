/*
this - a pointer to the current object instance that the method belongs to. only accessible from a member function
"this" is of type of "Class *const", meaning that it is constant pointer to the current object instance of the class,
int other words, it can't point to anywhere else (constant pointer
*/

#include <iostream>

void print_entity(const Entity& e);

class Entity
{
public:
    int x, y;
    Entity(int x, int y)
    {
        //x=x  assigning the ctor parameter int x with itself - shadows the member variable
        //Entity *const e == this;  <-the same (in a non-const function, which can change member variables)
        //const Entity& e = *this //e is a reference to entity which is constant, 
        //assigned with the content of "this" which resides where "this" is pointing to. e is "this"

        this->x = x; // also equals to (*this).x
        this->y = y;

        //better alternative: m_x = x;  declaring member variables with prefix m, more readable than this-> syntax
        

        print_entity(*this); //pass the data at the memory location the pointer points to (dereference).
    }

    int set_x(int num)// equivalent to:   void set_id(Entity* this, int num)
        //"this" is a hidden parameter - a const pointer to entity (cannot point to anything else)
    {
        this->x = num;
    }

    int get_x() const  // equivalent to:   int get_x(Entity* const this)
        //"this" is a hidden parameter - a const pointer to entity (cannot point to anything else)
    {
       //Entity const* const e == this //  because the function is const, this points to a const entity, whose values cannot be modified.
        //the ptr itself is also const (reading right-to left: e is a constant pointer to constant entity)
        return x;
    }
};

void print_entity(const Entity& e)
{

}


int main()
{
    /*
    Assembly wise the address of "this" is 64 bit wide (because its stores a memory address...) in 64 bit architecture.
    if we create entity in main(), the stack frame of main will be allocated with "sizeof(entity)" free space
    that will hold the entity object. In this case, sizeof entity is 8 bytes because it holds 2 ints.
    when calling the constructor function, we pass the "this" pointer into the stack frame of the constructor and operate
    on the address that refers to the object in main()'s stack frame, although we operate in the constructor's stack frame().
    */
    Entity entity(1, 2);
    entity.set_x(4); // compiler equivalent to entity_set_x(&entity,4) - passing a reference 
    //of the current instance of entity (&entity) to the hidden parameter "Entity* const this".
    //********Entity prefix denotes that we are calling a function that belnogs to the Entity class
    std::cout << "Hello World!\n";
}