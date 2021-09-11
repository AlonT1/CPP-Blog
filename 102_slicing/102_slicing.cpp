#include <iostream>
#include <string>

class Entity
{
public:
    std::string m_name;
    Entity(std::string name) : m_name{name} {}
    virtual void move() { std::cout << "Entity move\n"; }
    virtual void rotate() { std::cout << "Entity rotate\n"; }
    void scale() { std::cout << "Entity scale\n"; }
};

class Animal : public Entity
{
public:
    int m_num_legs;
    Animal(std::string name, int legs) : Entity{ name }, m_num_legs{ legs } {}
    virtual void move() override { std::cout << "Animal move\n"; }
    virtual void vocalize() { std::cout << "Animal vocalize\n"; }
    virtual void rotate() { std::cout << "Animal rotate\n"; }
    void sleep() { std::cout << "Animal sleeping\n"; }
};

class Cat : public Animal
{
public:
    bool m_has_fur;
    Cat(std::string name, int num_legs, bool has_fur) : Animal{ name, num_legs }, m_has_fur{ has_fur } {}
    void move() override { std::cout << "Cat move\n"; }
    virtual void attack() { std::cout << "Cat attack\n"; }
    virtual void vocalize() override { std::cout << "Cat vocalize\n"; }
    void jump() { std::cout << "Cat attack\n"; }
};

int main()
{
    Entity entity("entity");
    Animal animal("animal", 4);
    Cat cat("cat", 4, true);

    //polymorphism - Cat has 2 sections in memory in the following order:
    //1. Entity section - has a ptr to vtable which was constructed according to
    //the information from Entity, Animal and Cat. also contains m_name
    //2. Animal section - contains m_num_legs
    //3. Cat section - contains m_has_fur

    //when creating a ptr of type Entity to cat object, entity_ptr aligns
    //with section 1 of cat - the Entity section. Since Entity type has also a ptr to a vtable
    //this ptr is now aligned and assigned with the vtable of Cat (looking at a cat object through 
    //the eyes of an Entity object, all of its members are aligned with Entity members that exists in Entity section of cat.
    //when executing move(), the entity_ptr queries the vtable of Cat and is redirected to move() of cat
    //because it overrides the move of Entity.
    Entity* entity_ptr = &cat;
    entity_ptr->move(); // prints "Cat move"
    Entity& entity_ref = cat;
    entity_ref.move(); //polymorphism not only works with pointers but also with lvalue references!

    //slicing! - in the same way that the polymoprhic pointer of type Entity "entity_ptr" is assigned with the
    //Entity section of cat, the class reference entity2 is now assigned with the Entity section of cat.
    //but because****** polymorphism is implemented with pointers/references in cpp, we cannot reach virtual functions of Entity that
    //are overriden in cat******. So we are left with a sliced cat object from its "cat-ness", since entity2 just grabs
    //the entity data of cat - without vtable pointer that points to vtable of cat (with which polymorphism is implemented!)
    //- this leaves out the Animal and Cat data behind.

    //entity2 is a stack allocated object data type, aggregates all data in the stack.
    //vstudio shows that entity2 contains a vtable, but the compiler doesn't allow us to access this vtable
    //through entity2, only thorugh pointers/references, rendering polymorphism and the use of virtual
    //functions as disabled when using class objects such as "entity2" and leaving us with a sliced object.
    
    //aside from the advantages of polymorphism enabled by ptrs/refs, they also allow passing data
    //to functions more efficiently (by copy or by address).
    Entity entity2 = cat; //SLICING
    
}

/*
vtables contain only virtual functions that:
1. exist in a Base class that is inherited by other classes
whether the derived classes override those functions or not, e.g:

Class A contains virutal functions x, y =>
vtable Of A ==> function ptrs to "x() of A", "y() of A" in vtable of A.

Class B inherits from A, contains virutal function z, and overrides func x but not y  =>
vtable of B ==> function ptrs to  "x() of B" ,"y() of B" ,"z() of B" in vtable of B

Class C inherits from B, overrides func z and has ******virtual function p******=>
vtable of C ==> func ptrs to "x() of B", "y() of A", "z() of B"

Notes:
1. virtual function p in class C is not in the vtable of C, because C is never inherited. vtables contain
virtual functions that are passed by inheritance. no inhertiance - no vtable/virtual function/polymorphism.

2. The reason that the vtable of C has function pointer to "x() of B" and not "x() of A" is because
the compiler will execute the overriden virtual function in the most derived class between the Base (Entity) and derived (Cat).
therefore since the most overriden version of x() is in B, the vtable of B
holds that function pointer to fulfill this rule.

3. vtables are constructed per class and each instance contains only 1 vtable in the Base section of memory, 
the vtable is constructed according to what it overrides and the classes it inherits from, e.g:
Cat inherits from Animal which inherits from Entity, therefore the vtable of Cat will contain all the inherited virtual
functions from Animal and cat regardless if they override them or not. later the function pointers inside Cat's vtable
will point to the functions according to whether Cat overriden them or not, for example Cat overrides move(), therefore func ptr of
move() in vtable of Cat will point to move() of Cat(). Cat didn't override Vocalize() therefore func ptr to vocalize() in vtable of Cat
will point to vocalize() of Animal (this vocalize() is the most derived vocalize() between Base(Entity) and most derived(Cat))
The vtable of Cat is stored in the Entity section of memory of cat.


4. the reason Entity section of all inerhiting objects houses the vtable is because all classes that want to use
the virtual functions of Entity, must inherit from Entity, therefore they all have an Entity section
that they can align with (Animal* animal = &cat ==> animal's entity section is now aligned with cat entity section because
they are both Entities. Thus, the vtable pointer of animal that is housed in animal Entity section, is now aligned
with the Entity section of cat that holds a pointer to vtable of cat, therefore animal's vtable pointer now points
to the vtable of cat.


5. vstudio doesn't show vtables correctly: vtables of Entity, Animal, Cat contain only the virtual functions that were declared
at the top most class (Entity) (move() and rotate()). No indication of the virtual functions between Animal and Cat (vocalize() doesn't
appear in the vtable of Animal and Cat), although calling virtual
Cat functions through Animal ptr works fine (the Cat functions are chosen)
6. pure abstract virutal functions must implemented inside an inherting class otherwise it 
cannot be instantiated and becomes an abstract calss

*/