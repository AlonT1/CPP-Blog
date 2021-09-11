/*
polymorphism - the use of a single symbol to represent multiple different types.
dog can be represented as an animal, when dog inherits from the animal, because dog 
is-a(n) animal but animal isn't a dog.

Polymorphism is implemented via virtual functions (see 28_virtual_functions) and 
only works with pointets.
Base base = derived will cause slicing (see 102_slicing) while Base* base = enables polymorphism.

*/

#include <iostream>

class Animal
{
public:
    int x = 10;
    virtual void run()
    {
        std::cout << "Animal runnin'" << std::endl;
    }
};

class Dog: public Animal
{
public:
    int y = 8;
    void run() override
    {
        std::cout << "Dog runnin'" << std::endl;
    }
};

class Cat: public Animal
{
public:
    void run() override
    {
        std::cout << "Cat runnin'" << std::endl;
    }
};


class A { int x = 10; };
class B : public A{ int y = 8; };

int main()
{
    int* x = new int[5]{ 1,2,3,4 };
    Animal animal;
    Dog dog;
    Animal* doggo = &dog; //polymorphism uses pointers, can point to stack/heap allocated obj, e.g: Animal* dog = new Dog; 
    //or in one line Animal* doggo = &Dog()
    /*
    alternitavely using heap - "new Dog" creates a Dog object on the heap (malloc) and returns an address which is copy-initialized 
    to the "dog" pointer of type animal (read 39a_new_operator_expression for further details).
    WHY THIS IS ALLOWED? The Dog type includes all the functionality
    of Animal (unless declared private in Animal). Therefore pointer to Animal can point to an address that contains Dog
    because Dog includes the behaviour of Animal.
    Dog* dog = new Animal; WON'T WORK. WHY?
    pointer to Dog cannot point to type Animal BECAUSE ANIMAL DOES NOT CONTAINS THE BEHAVIOUR OF DOG!
    ANIMAL is included both in ANIMAL and in DOG (superset of Animal) but Dog is not included in Animal.
    memory wise: see vtable diagram
    tl;dr doggo points to the Animal portion of dog which appear at the starting address of dog, this ensures
    that the Animal portion of dog aligns with the Animal portion of Animal.
    

    */
  
    doggo->run();
    /*
   Because the pointer is of type "Animal" we can only access the data that is available in Animal.
   the class members of Dog are inacessible. if we execute via "dog" pointer a virutal function that was declared in Animal
   and overriden in Dog - the function in Dog will be executed! this is because although
   dog is of type Animal, it was initialized by "new Dog" (polymorphism).
   */

    
    //A* a = new b;
  
    Animal* animals[] = { new Dog, new Cat };
    /*saving code! cat,dog are both Animals, we can loop over them and run the appropriate
    in this case the animal pointer is assigned with the Animal portion of Dog and Cat, in this process
    the pointer to the virtual table of Dog and Cat is aligned with the virutal table pointer of type Animal,
    and because Dog and Cat are Animals this is allowed. therefore, we can traverse Dog and Cat with an Animal 
    Pointer which "looks" at their Animal portion and is able to accept them due to alignemnt with Animal's data, and each time
    we execute a function, the virtual table pointer of Animal type points to the correct vtable that belongs to the underlying type we
    point to. THIS IS AN EXAMPLE OF LATE BINDING becuase during compilation we cannot know which run() function we'll execute,
    not until we traverse the animals[] array.
    */
    for (Animal* animal : animals)
    {
        animal->run();
    }
 
}
