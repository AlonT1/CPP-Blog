/*
virtual functions are invoked  through a polymorphic pointer/ref (at run time - dynamic dispatch) 
and work under 3 conditions:
1. Inheritance -  class B inherits from class A
2. Virtual function override - class A contains a virtual function that class B overrides
3. polymorphic pointer/ref - a polymorphic pointer/ref of type Base points
that points/refers to one of the Derived (inheriting) classes.
we exectue virtual functions through the polymorphic pointer.
Note: the pointer can point to either stack or heap allocated array! we both we can achieve virtual functions!
e.g:
Animal* animal = &cat;
animal->walk()  // walk() of Cat is exectued, assuming all 3 conditions above are met.

(polymorphism -  symbol that can represent multiple types -
the pointer/ref can represent either Dog/Cat)

virtual functions, implemented using vtable (explaine below) enable dynamic dispatch -
when a function call is linked to its implementation at run time
through a polymorphic pointer, which is especially critical during run
time when the function call cannot be resolved at compile time, for example:

1. if the user presses 1 then ptr of type animal points to dog and if the user presses 2 it points to cat - 
only at runtime we can tell if  we want to execute walk() of cat or dog.

2 another example is a polymorphic array of animals where a pointer executes walk()
function of each Animal in the array iterated by a for loop at run time.
Only at run time, during the iteration, we can tell via the Animal ptr, which walk()
to dynamically dispatch based on the run-time type of the ptr (run-time type - the type
at which the pointer points to).

Therefore, The vtables mechanism (explaine below) helps to resolve the call (animal->walk() (animal polymorphic pointer)
does walk() belongs to Animal::walk() or does it belong to Dog::walk())


polymorphism (implemented via virtual functions and only works through class pointers)
isn't free, 2 runtime costs -
1. additional memory to store the vtable so we can dispatch to the correct function. 
classes that implement virtual functions
2. classes that use virtual functions include a member pointer at  that points to the v-table.
every time we call a virtual function  we have to go through that pointer
and reach the table to determine to which function to actually map to 
(pointer jumps costs performance due to indirection).
The vtable itself is a static array (belongs to the class and share among all instances)
that contains function pointers.
in practice no difference in perfromance impact (maybe on embadded where every cpu cycle counts)
CPU cycle - time (unit) it takes for the cpu to perform a task. e.g: 3Ghz (3 billion cycles per second)
each cycle lasts 0.33 nanosecond.

Only member functions can be declared virtual (friend functions cannot be virtual, 
they are not part of the class)

Note that virtual functions in themselves are not necessary for polymorphism

Advantages:
can be found in 29_interface_pure_virtual_functions. in short
1. extensiblity+flexibility: Pay(CreditCard&) can accept Visa, AmericanExpress (all inherit from CreditCard
and override it's virtual functions)....
2. Encapsulation - CreditCard pointer hides methods that Visa, AmericanExpress, etc... didn't override
3. polymorphic array - we can store different credit cards in CreditCard Array and loop through
it and execute virtual functions that exist in CreditCard.

*/

#include <iostream>

class Entity // base
{
public:
    int x = 0;
    void foo() {};
    virtual std::string get_name() //virtual - tells the compiler to generate a vtable 
        //for this function so if its overriden, the compiler can point to the "overriden" function  
        //without "virtual": Entity* ptr = new Player();  ptr->get_name() // get_name() of Entity is invoked!
    {
        return "Entity";
    }
};


class Player : public Entity //derived
{
private:
    std::string m_name;
public:
    Player(const std::string& name) : m_name(name) {} // list initializer for constructors
    void boo() {}

    /*

    Note:
    1. An inherting class doesn't have to override a virtual function: Assume  class C inherits from B which inherits from A,
    if virtual func() exists in class A, is overriden in B and we instantiate c where func() is not overriden and
    we execute func() through c, the most derived overriden func() between A and our class (C) will execute,
    therefore func() of B will execute. This is in contrast to pure virtual functions which MUST be implemented
    in the inheriting class, otherwise the inheriting class becomes an abstract class, without the ability to
    instantiate it. A class must be completely "concrete" by implementing all pure virtual functions to be instantiatble.
    non-pure virtual functions already has a definition when we declare them (can be defined in
    a external cpp (declaration in a header, definition in a cpp). it is our choice whether inherting classes override them.
    pure-virtual functions don't have a definition by default, it our job to implement them somewhere, in
    order to instantiate the implementing class.
    

    3.  When class Animal has a non virtual function walk() and class Cat that inherits
    from Animal and also has a walk() function - This isn't an override!!!!!
    walk() of Cat simply HIDES (SHADOWING) walk() of Animal - no vtables are involved
    Note that "Animal* anim = &cat;" is still considered polymorphic pointer because
    Cat inherits from Animal, but invoking anim->walk() will not invoke walk() of Cat.
    (of course Cat cat; cat.walk() will invoke walk() of Cat).
    BUT! when Walk() of Animal is virtual and then Cat also defines a Walk() - then 
    it is considered an override, therefore only in this situation (virutal functions)
    override keyword can be used.
    
    4. override keyword not mandatory for functions that override virtual functions
    This keyword helps preventing bugs (raises error - overriding function doesn't
    match the base's virtual function or if the base function isn't marked as virtual when we
    try to virtually override it.)
    what if we use override on function that does not override anything? error:
    main.cpp:23:10: error: ‘void Mario::Jump()’ marked ‘override’, but does not override

    5. overriden virtual functions can be also marked as virtual - NOT MANDATORY:
    if Cat inherits from Animal and no one inherits from Cat (final), then Walk()
    of Cat can "live" without the virtual keyword and be satisfied only with "override".
    If Tiger inherits from Cat, Walk() of Cat SHOULD be marked with Virtual to help us see
    that Walk() of tiger overrides a virtual function - This is for readability only.
    As long as Walk() of Animal is marked virtual, all Walk() funcs down the line are implicitly
    virtual.

    combining "override" and "virtual" keywords on a function that overrides a virtual function
    helps readability, because we can be certain that the function overrides a virtual function.

    6. "override" can only be used only when defining/declaring a function inside the class.
    e.g: in a header file that contains the class delcaration,
    we can declare functions with "override" inside the clas, but the function's definition in .cpp
    file  annot be marked as override - The same goes for the virtual and friend keyword
    both can be used only inside the class during declaration / definition in the declaration
    of the class interface (usually done in the header). Here the declratation of th class interface
    and the actual implementation is all in the same cpp file, so its ok to use "override"  here.
    */
    virtual std::string get_name() override
    {
        return m_name;
    }
};

class Boss : public Entity //the implementation of pure virutal / virutal functions is not mandatory!
{

};


//looks at entity and calls get_name, but we want cpp to recognize  if this is a player, not an entity!!
//this is done via virtual functions
void print_name(Entity* entity)
{
    std::cout << entity->get_name() << std::endl;
}

int main()
{
    /****************** without declaring get_name() virtual*/
    Entity* e = new Entity();
    std::cout << e->get_name() << std::endl; //entity
    Player* p = new Player("player");
    std::cout << p->get_name() << std::endl; //player

    Entity* entity = p; //valid polymorphic entity points to the same spot as p
    std::cout << entity->get_name() << std::endl; //returns "Entity" not "Player", although the var points to player!!!
    //without virtualizing the get_name function, only "entity" will be returned,
    std::cout << "" << std::endl;

    /******************  declaring get_name() virtual*/
    Entity* e2 = new Player("player");
    /*
    Because the pointer e2 is of type "Entity" we can only access the data that is available in Entity.
    the class members of Player are inacessible. if we execute via e2 a virutal function that was declared in Entity
    and overriden in player - the function in player will be executed! this is because although
    e2 is a pointer of type Entity, it is copy-initialized with an address of "new Player" (=malloc) via polymorphism
    and a vtable which responsible for executing the correct overriden function according the rhs
    */

    /*
    vtables explained:
    Binding refers to the process that is used to convert identifiers (such as variable and function names) into addresses. 2 types exist:
    1. static binding: happens at compile time when all the info is available (faster performance) e.g: direct call to simple class method
    being  replaced with a machine language instruction that tells the CPU to jump to the address of the function.
    2. dynamic (late) binding - at runtime where the true identity of the called function is not resolved. e.g:
    function pointer activated after being assigned with a function based on input from user
    e.g: overriden method of virtual function
    
    every class that contains virtual function / overrides them, has 2 properties:
    1. Virtual table (vtable) a static array the the compiler sets up at compile time, 
    that contains function pointers to the virtual/overriden functions of the class. the vtable of a derived
    class holds all the virtual functions of its parent.
    2. hidden pointer - points to the virtual table of the class (increases the size of the class, inherited by derived classes),
    the virtual table pointer is unique per instance but they all point to the same static vtable of the class.
    Entity and Player both have their own vtables and vpointer to their respective vtables
    for more info see vtable diagram.

    given:
    Parent* ptr = new Derived(); // Derived inherits from Parent
    (memory layout, portion)
    Parent
    __vfptr: void**---------------------->  Parent Vtable
    x: int (10)                             ptr to Foo of Parent    
    virtual Foo: void                       ptr to FooNotOverriden of Parent
    virtual FooNotOverriden(): void
        |
        |
        ↓
    Derived------------------------------>  Derived Vtable
    __vfptr: void**                         ptr to Foo of ***Derived***
    x: int (10) // inherited from Parent    ptr to FooNotOverriden of Parent
    y: int (8)
    Foo() void override

    as seen in 'Deived Vtable" ptr to Foo actually points to Foo of Dervied!

    note about inheritance when the class has virtual to one of its function - 
    both the base and inherited instance are assigned with a vfptr ptr (8 bytes,
    like any other pointer) to their own unique virtual table:

    on a 64 bit system (8 byte memory address (64 bit), each hex character is 4 bit, total
    of 16 digits = 64 bit)
    Given "Parent p1" its memory block is:
    0x000000784C1FF9B8 00007ff63936bd90 cccccccc0000000a    
    (start address)    (add. of vfptr)  (add. of 4 byte int, cccccccc is for padding)

    Given 
    The memory block of "new Derived"
    0x000000784C1FFA18 00007ff63936bdb0 cccccccc0000000a cccccccc00000008
    (start address)    (add. of vfptr)   (inherited x)    (add. of y)

    Note that the inherited "Derived" has all the memory structure of Parent, and it appears first.
    
    */
    print_name(e2);//player


    //USING HEAP ALLOCATED OBJECTS IS NOT MANDATORY!
    Player player("ma");
    Entity* e4 = &player; //works!

    //what about assignment by value to stack allocated variables?
    Entity e3 = Player("ken");
    std::cout << e3.get_name() << std::endl;//Entity!
    /*
    This is a problem called "slicing" (explained in 102_slicing): we instantiated "Player" and assigned it by value to an Entity,
    this assignment by value "slices" the Player-ness from Player, and we remain only with Entity.
    This only happens when assigning the object by value to stack allocated objects,
    therefore in order to use polymorphism with base classes, we MUST USE POINTERS OR REFERENCES to heap allocated object.
    Virtual functions cannot be invoked using pointers to stack allocated objects for example:*/
    e4->get_name();
    /*
    Because e4 points to a stack allocated Player, the compiler knows exactly the type of E
    
    Important Note: in Java Entity e = Player(), means that e is a class reference to a Player object of type Entity.
    in other words e is just a reference/pointer to the location in memory where the data stored on the heap. in some sense it is
    similar to the new keyword which also allocates on the heap and returns a pointer address to that location.

    in CPP Entity e = Player() means that e is actually a variable that holds the data of Player directly (inline) on the stack
    it isn't a pointer to a location in memory. this is called value semantics. pointers/references are all about reference semantics.
    */

}
/*
Notes:
virtual functions are dynamic dispatch implementd by v-table
vtable- contains mapping for all the virtual functions inside the
base class so we can map to the correct overriden functions at run time
if we want to override the function we need to mark the function at the base class as virtual
*/
