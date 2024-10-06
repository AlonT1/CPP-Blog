/*inheritance helps to avoid code duplication extend base class
derived class extends base class.
A cat has everything an animal has but an animal doesn't have everything a cat has
*/

#include <iostream>

//"class" is the "class-key" - can be "struct" or a "class". "Entity" is the class-head-name
class Entity //every game character has these properties. the following block is called the "member-specification".
{
private:
    float x, y;
public:
    int pub_entity_x;
    Entity(float x, float y) : x(x), y(y) 
    {
        std::cout << "Entity ctor" << std::endl;
    }
    void move(float xa, float ya)
    {
        x += xa;
        y += ya;
    }
    void print_members()
    {
        std::cout << x << " " << y << std::endl;
    }
};

// also called derived class
// "public Entity" is called the "base-clause" - list of 1 or more parent classes
// "public" is the access specifier of the class - examples below (also called the model of inheritance??)
class Player : public Entity 
{
public:
    const char* name;
    //float x; //Note: we cannot override data members that exist in the base class.

    //if we instantiate "Player p;" p's memory will be comprised of Entity memory portion
    //and a memory portion that stores its own data (*name). therefore we'll have an x in
    //p's Entity portion, additionl x in p's own data portion. therefore 
    Player(const char* str, float x, float y = 2.0) : Entity{ x, y }, name{ str } //calling Entity ctor
    /*y=2.0 is a  default parameter.
    Entity {x,y} is a delegating ctor (giving Entity the info needed to construct the base class)
    when deailing with inheritance, member initialization can continue after calling delegating ctor.
    when not dealing with inheritance (delegating a ctor in the same class), no member initialization
    is allowed after the delegation.

    default parameters work because the default arguments must be filled from the end
    to allow statements such as  Player player("shalom", 4.0f), 1.0f for y is default, although
    y is not specified in the ctor call. why default arguments must be filled from the end (rightmost (trailing))?
    because for example if this was allowed: Player(const char* str, float x = 0.0, float y),
    then the ctor call Player player("x",3) is invalid because it is not clear wheter 3 belongs to x or y
    this ctor would have worked if we had followed the convention:
    Player(const char* str, float x, float y = 2.0), now everything ALIGNS

    Note:  When implementing your constructors, consider how you might keep 
    the number of constructors down through smart defaulting of values.

    */
    {
        std::cout << "Player ctor" << std::endl;
    }
    void print()
    {
        print_members();
        std::cout << name << std::endl;
    }
};


class Animal
{
public:
    int x = 10;
    Animal()
    {
        std::cout << "Animal ctor\n";
    }
    ~Animal()
    {
        std::cout << "Animal dtor\n";
    }
};

class Dog : public Animal
{
public:
    int y = 8;
    Dog()
    {
        std::cout << "Dog ctor\n";
    }
    ~Dog() 
    { 
        std::cout << "Dog dtor\n";
    }
};

/*
Access specifiers of a class -
they "cap" the access specifiers of public/protected inherited members.
private members remain private in inherting class regardless of the inheritance specifier
examples - assume B inherits from A by public:
1. public inheritance:  no capping occurs, the access specifiers of members of A
are exactly inherited into B
2. protected inheritance: public members in A are capped into protected members in B (privates remain private)
3. private inheritance: public and inheritance specifiers from A are capped in private members in B.


difference between private, public, and protected inheritance:

class A
{
    public:
       int x;
    protected:
       int y;
    private:
       int z;
};

class B : public A
{
    // x is public
    // y is protected
    // z is not accessible from B
};

class C : protected A
{
    // x is protected
    // y is protected
    // z is not accessible from C
};

class D : private A    //************** 'private' is default for classes!!!*****************
{
    // x is private
    // y is private
    // z is not accessible from D
};

*/


int main()
{
    //everything that is not private in Entity is accessible in player
    Player player("shalom", 4.0f);
    player.print();
    /*
    Construction order starts from the outermost class (Base) and keeps constructing inwards.
    destruction is in the opposite order (outwards):

    Here’s what actually happens when player is instantiated:

    1. Memory for player is allocated (enough for both the Base and Derived portions)
    2. The appropriate Player constructor is called: Player(const char* str, float x, float y)
    3. The member initializer list of this ctor is invoked and calls the base Entity ctor. therefore the memeber initalizer list
    of this ctor is halted for now, after constructing the Parent we will return to the member initalizer list to initialize "name".
    calling the base ctor is only possible through the member initialization list, and not through the body! why? because:
        a. cpp guarantees that the parent will be finished constructing after we finish with the child's member initializer list
        and before we enter the body of the ctor. this is visible by the fact that when we finish with the member initializer list,
        the compiler doesn't enter the body of the ctor, but rather jumps to the ctor of the parent (step 4).
        b. if there wasn't a call to the parent ctor in the member initializer list of Player, the call to the parent's
        default ctor would have happened implicitly! therefore by explicilt calling the ctor we want we override the implicit call
        c. calling the parent's ctor in the body just creates an instance of the parent
        d. Unlike Java we can't use super() to call the paren't ctor, we must use the explicit name of the Parent's class
        because cpp support multiple inheritance.
    x and y are not yet initialized
    4. The Entity object is constructed  using the appropriate Entity constructor member initializer list.
    If no base ctor is specified, the default constructor will be used.
    5. After completing the member initialization list, The compiler enters into the body of the Entity ctor
    6. Returning back to the member initialization list of Player to continue from where we left: we already called the Entity
    ctor, now whats left is to initialize "name", 
    7. The compiler enters the body of the ctor
    8. the ctor returns a Player object

    ***********************************
    The main rationale behind inheritance is to reduce code duplication and improve code organization
    ************************************

    Note: the newly created player object is created with 2 "recipes" the Base class (Entity) and the Derived
    class (Player). In terms of member variables, "player" contains ALL THE MEMBER VARIABLES that "Entity"
    "Player" have. the member variables created via the "Entity" recipe exist in the "Entity" memory section of the
    "player" object (visible in the debugger). 
    In terms of member functions (a.k.a methods)- 

    each function, whether it is static or not, is namespaced to to the class it belongs to.
    so even if Player inherits from Entity that houses a shoot() function (whether it is static or not),
    if the same shoot() function is invoked through a Player instance, the assembly will execute Entity::shoot();

    with virtual functions the same thing happens, only that the vtable mechanism exists
    which enables the dynamic dispatch of virtual functions, specifically when the function call
    cannot be linked to its implementation (in the code section of memory) during compile time
    due to run time restrictions (more details in 28_virtual_functions).
    The vtables mechanism helps to resolve the call (animal->walk() (animal polymorphic pointer)
    does walk() belongs to Animal::walk() or does it belong to Dog::walk())

    */
    player.pub_entity_x; // derived instances can access its own members and members of inherited!
    std::cout<< sizeof(float) << std::endl;
    std::cout << sizeof(Player) << std::endl;  //player is bigger -> it extends Entity (contains all of its features - even if
    //if those features are private, for example a private class member, they will still be a part of player's memory
    //although not accessible by the compiler during coding.


    Dog dog;
    /*
    memory wise, the first 4 bytes starting from the memory location of dog contain 0000000a (decimal 10 from x=10 of Animal)
    and the next 4 bytes contain 8 for int y = 8 of Dog. The compiler actually recognizes x=10 as the "Animal portion" of Dog,
    but memory wise they are just two ints one after another .dog contains everything that Animal has! memory wise, dog can exist
    alone with all the features of Animal which are just "copied" into dog.
    Inheritance is just a tool to avoid code duplications.
    */


    dog.Animal::x; //alternative to dog.x. This syntax shows that x is in 
    //the Animal memory section of dog (the first 4 bytes of dog representing int x=10).
}

//polymorphism - treating differet types as single type - player is superset of entity - it has
//everything Entity has and more. function which recieves Entity objects can also recieve player object
// even though it accepts only Entity, because player is guaranteed to have entities proprties