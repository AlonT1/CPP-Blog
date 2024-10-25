/*
-> is called element selection through pointer (either on stack or heap) - overloadable
. is called element selection by reference (object on stack/heap) (indirection) -  not overloadable
they have the same precedence and associativity (read left-to-right)
*/
#include <iostream>

class Entity
{
public:
    int x = 2;
    void print() const
    {
        std::cout << "hello" << std::endl;
    }
};

class ScopedPtr //wraps around dynamically allocated entity - deletes it when going out of scope
{
private:
    Entity* m_obj;
public:
    ScopedPtr(Entity* entity) : m_obj(entity) {}
    ~ScopedPtr() { delete m_obj; }

    //entity->print() which is equivalent to: which is equivalent to entity.operator->(print()), becomes entity.(*m_obj)
    Entity* operator->() { return m_obj; } 
    //the above replaces Entity* get_obj () {return m_obj;}, arrow looks better than entity.get_obj()->x (get_obj returns pointer)
    //const can be added, as in const Entity* return value to accommodate
    //const ScopedPtr entity2 = new Entity(); // implicit conversion 
};

struct Vector3
{
    float x, y, z; //the offset of each variable is 4

};

int main()
{
    Entity e; // class reference  (value variable)
    e.print(); // . operator => element selection by reference

    Entity* ptr = &e;
    (*ptr).print(); // equals to e.print() - element selection by reference (indirection)
    ptr->print(); // equals to the line above - element selection through pointer

    //alternative
    Entity& entity = *ptr; //e's data is assigned to an entity reference
    entity.print();  //works

    //alternative - better
    (*ptr).print(); //. operator -> element selection by reference.
    //(*ptr) acts like a class reference, directly refering to the data
    //. has higher precedence than *, therefroe parenthesis are needed

    //even better alternative - arrow -> is just a shortcut for (*ptr).print();
    ptr->print(); // -> only works for pointers! for class references (value variables) we use "." operator

    // "ptr->" is equivalent to "(*ptr)." !!!!!!!!!!!!!!!!!!!!


    //OVERLOADING ARROW OPERATOR
    ScopedPtr entity1 = new Entity(); // implicit conversion
    
    /*
     scoped ptr is just wraps around ptr to an entity obj. to access members of wrapped object entity (such as x member)
     we can override ->. -> is free here because entity is a class reference,
     which doesn't use -> (-> is for pointers) and class references uses . operator
     therefore we can utilize & override -> to acess members of entity through the scoped pointer

     "entity1->" is now equivalent to "(*entity1)."
     */
    entity1->x;
    entity1->print();

    /*GET MEMORY OFFSET OF A VARIABLE
     int* w; // declaring w as a pointer to an int (read right to left) - it isn't initialized yet!
    //doesn't points to nowhere! but the variable itself IS defined in a location in memmory
    int* t = (int*)0; // equivalent to nullptr, means that t is pointing to address 00000000  and there is an int there
    (int*)0 in c/cpp is a constant expression that means null pointer to an int which starts from 00000000 memory address
    (int*)1  starts from 00000001 memory address
    this creates a pointer to Vector3 which starts at 00000000. the pointer is not initialized with an object!
    In other words,
    (Vector3*)0 points to 00000000 as if there is a Vector3 object there (but there isn't!)
    when we  access class memebers from it and get their address we can infer their memory offset by their number of byes. 
    1.(each float is 4 bytes) (Vector3*)0 ->  pointer to a vec3 which starts at 00000000 -> 
    2.(*(Vector3*)0).y dereference y -this is derefences an undefined memory location!
    (maybe there is some contnent there made by another program, but this program didn't define it
     3. &(*((Vector3*)0)).y  retrieives 00000004 which is the mem address of y starting from 00000000
    4. (int)&(*(Vector3*) 0).y  converts 00000004 to 4, all of this is equal to
    (int)&((Vector3*)0)->y  replacing dereference operator by ->    (*(Vector3*)0).y   ==  ((Vector3*)0)->y */

    std::cout << (int)&(*(Vector3*)0).y << std::endl;
    //tl;dr taking an address of derefenced y at "simulated" vector 3 at address 0 -> yields (0000000004) and convert it to int-> yields 4
    //which means 4 byte memory offset for an int (simulated because there isn't really an allocated vector3 at 0000,
    //we just declare a pointer of type vector3 to that location! derefercing y yields udefined (?), but taking its address
    //yields 000000004 since the compiler recognizes that a pointer of type vector3 needs to point 4 bytes forward to reach 
    //the second int y as its 4 bytes forward from the starting location (x is at zero location, z at 8th)




    //https://stackoverflow.com/questions/10677804/how-arrow-operator-overloading-works-internally-in-c
}
