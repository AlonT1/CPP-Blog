/*
Smart pointers automate the deletion of "new" objects - because sometimes the programmer forgets to "delete"
dynamically allocated objects. Smart pointer classes are allocated on the stack
and include a real raw pointer to a dynamically allocated object. see 43_stack_scope_lifetime.
when smart pointers go out of scope, they invoke their own dtor automatically (without calling delete,
because they are allocated on the stack). Inside the dtor the smart ptr has
performs a  "delete" on the pointer it houses that is pointing to dynamically allocated object.
This is technqiue is called RAII (Resource Acquisition IS INITIALIZATION) (26_destructor).
**********************************************************
Therefore, smart pointers can only point to heap allocated objects, otherwise
the delete operator in its dtor will fail because "delete" cannot be executed on
pointers that point to stack allocated objects.
**********************************************************

This solves the problem of dealing straight with dynamicaly allocated objects
and forgetting to delete them - which leads to memory leaks.

smart pointers help in solving the issue of ownership:
if two functions handle the same raw pointer we have to decide who frees up that pointer.
this is mitigated by smart pointers.

Smart pointers are essentialy scoped pointers, "see 26_destructor" ScopedPointer class.
*/


#include <iostream>
#include <memory> //necessary for smart pointers

void* operator new(size_t size)
{
    std::cout << "new \n";
    return malloc(size);
}



class Entity
{
public:
    int m_x;

    Entity(int x) : m_x(x)
    {
        std::cout << "created" << std::endl;
    }
    ~Entity()
    {
        std::cout << "destroyed" << std::endl;
    }
    
    void print()
    {

    }
};

class Player : public Entity
{
public:
    int m_y;
    Player(int x, int y) : Entity(x),  m_y{ y } {}
    std::unique_ptr<Entity> test()
    {
        // rvo - no copies of unique_ptr's at return, built on the stack frame of the caller.
        // Because smart pointers warp around heap objects (Player created on the heap), the
        //Player object won't be destroyed when leaving this scope (it will be destroyed when
        //the unique pointer will go out of scope - RAII
        return std::make_unique<Player>(5, 4); 
    }
};

int main()
{
    /****************Unique pointers - internally, holds a single pointer to
    the managed instance, can't be shared*/

    //smart pointers can also be instantiated using "new":
    std::unique_ptr<Entity> ptr2(new Entity(3)); // the ctor of unique_ptr has a pointer which receives the argument
    //the dtor of unique_ptr will execute a "delete" when reaching the end of a scope,
    //so that we won't have to call a delete on "new Entity(3)"

    //make_unique is better than "new" because the rule of "never use new" of exception safety:
    //suppose that in the previous example "new Entity(3)" would have failed due to memory allocation,
    //then the pointer inside unique_ptr class would have been constructed with an undefined
    //location in memory, essentially becoming a dangling pointer. Therefore all the make_x functions
    //(make_shared, make_unique...) throw std::bad_alloc exception (or any possible exceptions
    //that might be thrown by the ctor of T (the type that the smart pointer wraps around.
    //the following provides excepetion safety - safer if ctor throws exception so we won't end up with a 
    //dangling pointer - memory leak
    std::unique_ptr<Entity> entity = std::make_unique<Entity>(3); //<Entity> is a template parameter
    //3 is an arg used for a matching ctor. the args are separated by commas and a matching ctor is chosen.
    //make_unique constructs an object of type Entity and wraps it in a std::unique_ptr.

    entity->print();

    //entity ptr is destroyed when going out of scope has low overhead. when entity ptr (stack allocated) dies,
    //the object it wraps around dies with it, meaning its dtor gets called without using explicitly delete
    //(automates "delete" process a.k.a RAII).

    // std::unique_ptr<Entity> e0 = entity; // pointer can't be copied (it's unique)
    //copy assignemnt operator is deleted behind the scenes, but they can be moved:
    //now ptr is the owner of the resource that ptr wraps around!
    //the original entity pointer has all of its fields set to null (done by the move ctor
    //unique_ptr when constructing the ptr object).
    //because the copy assignment operator is deleted we cant pass the unique pointer
    //to functions by value, only by address/ref/std::move
    std::unique_ptr<Entity> ptr{ std::move(entity) };

    //smart pointers can be polymorphic!!!!
    std::unique_ptr<Entity> poly_ptr = std::make_unique<Player>(4, 5);
    std::cout << poly_ptr->m_x;

    //unique pointers hold a specific advantage over raw/shared/weak pointers - they limit shared
    //ownership that can occur with raw/shared/weak pointers. Shared ownership over a resource
    //(multiple pointers pointing to the same resource) can cause bugs, coupling issues, difficult
    //track of changes, and harder maintenance. with unique pointer we know who is in charge.


    /********************shared pointers - internally, holds 2 pointers one for the managed instance, and a second
    pointer to a control block that supports reference counting - keeps track of how many reference we
    have for the pointer - 2 shared pointers - 2 ref count, only when both die, the memory gets freed*/
    std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>(3);

    /*
    alternative std::shared_ptr<Entity> sharedEntity (new Entity()); not recommended - in unique pointers
    the reason to avoid this construction is exception safety, however for shared pointers, theres is another
    advantage to use make_shared:

    make_shared creates a dynamic control block memory where it stores the reference count (increases to 1)
    if we use shared_ptr ctor, instead of make_shared, we essentialy:
    1. create new instance and we pass it to the shared pointer ctor
    2. smart_ptr allcates the control block

    But, if we construct a shared pointer with std::make_shared it the two constructions are optimized
    into a single memory allocation, better performance.
    */

    /*
    SHARING POINTERS CAN ONLY BE DONE USING OPERATOR= :
    in the following example the two ptrs DO NOT SHARE THE RESOURCE:
    std::shared_ptr<Entity> ptr1 (resource);
    std::shared_ptr<Entity> ptr2 (resource)
    each std::shared_ptr will independently allocate its own control block,
    which will indicate that it is the only pointer owning that resource.
    Thus, when if ptr2 goes out of scope, it will deallocate the resource,
    thus ptr1 will point to a dead object.
    therefore we should use make_shared which mitigates this issue.

    ****************************
    Sharing can only be done via operator=  :
    ptr1 = ptr2; now both of them will hold the same resource and
    the reference count will increase to 2
    *****************************
    .*/


    std::shared_ptr<Entity> entity1;
    {
        std::shared_ptr<Entity> entity2 = std::make_shared<Entity>(3);
        entity1 = entity2; // (resource sharing via operator=) reference count is now 2***************
        //entity1 holds the same resource as entity2
        //although we are about to leave the scope entity2 won't be destructed because entity1
        //"points" to the same location and occupies the same resource. In other words, the pointer itself
        //will go out of scope, but the destructor will not execute, leaving the wrapped resource intact.
        //that being said, the reference count will be lowered by 1, since there is 1 less pointer sharing the resourece.
        //shared ptr - we cant perform "delete" on the pointer (to delete the object it points to) because the ref count is 2 
    }
    //when leaving the outer scope entity1 will die, reference count will be lowered, and the memory will be freed


    //***************************weak pointer - multiple weak pointers can hold the same resource, 
    //but there is no control block for counting references. as soon as one dies, the content 
    //(resource) will be destroyed, leaving all other pointers to become a wrapper for a dead object.
    std::weak_ptr<Entity> weak;
    {
        std::shared_ptr<Entity> entity2 = std::make_shared<Entity>(3);
        weak = entity2; //weak pointers won't increase ref count!! - we can ask weak pointers are they valid or expired(?)
        //leaving this scope, entity2 will get destroyed with the data, leaving "weak" pointing to invalid space
        //as opposed to the shared_ptr, here we CAN perform a "delete" on one of the pointers.
    }

    /*
    Notes:
    1. In general, you should not return std::unique_ptr by pointer (ever) or by value
    ******thus they are recommended to be passed between functions via std::move********.
    passing by value will cause a copy, which is especially bad for all smart pointers
    (impossible for unique, increases ref count for shared pointers)
    *although the copy might be elided by the compiler completely, it is better to use 
    move semantics, to be sure and not rely on compiler optimiazations

    2. USE THEM ALL THE TIME, automate memory management. have overhead due to reference counting system. unique has 
    lower overhead due to lack of reference counting, so unique 1st, shared 2nd
    */
    
}
