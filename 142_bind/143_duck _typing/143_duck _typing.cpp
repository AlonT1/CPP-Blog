#include <iostream>

/*
templates in cpp perform duck typing:
a form of type checking -  in computer programming is an application of the duck test
"If it walks like a duck and it quacks like a duck, then it must be a duck":
in the exmaple below, both Duck and Whale can swim. therefore if whale can swim as a duck - it is a Duck.
because of these, with templates, animal.swim() work on any T that supports swim().
"Types are checked by their similarity, not by their names"
*/

#if 0
struct Duck
{
    void swim() { std::cout << "duck is swimming\n"; }
};

struct Whale
{
    void swim() { std::cout << "whale is swimming\n"; }
};

template <typename T> 
void foo(T animal) // Whale can swim and so is a Duck, thus Whale==Duck
{ 
    animal.swim(); 
}

int main()
{
    Duck duck;
    Whale whale;
    foo(duck); // == foo<Duck>(duck)
    foo(whale); // == foo<Whale>(whale)
}
#endif



/*
In classic OOP, interfaces are used to achieve the same affect:
*/

#if 1
struct Animal
{
    virtual void swim() = 0;
};

struct Duck : public Animal
{
    void swim() override
    {
        std::cout << "duck is swimming\n";
    }
};

struct Whale : public Animal
{
    void swim() override
    {
        std::cout << "whale is swimming\n";
    }
};

int main()
{
    Duck duck;
    Whale whale;
    Animal* animal = &duck;
    animal->swim(); // prints: duck is swimming

    animal = &whale;
    animal->swim();// prints: whale is swimming
}


#endif

/*
Other examples of duck typing in C# -Initializing a new collection by curly braces.
List<int> list = new List<int>() {1, 2};
The compiler searches for a suitable method called “Add” 
that takes a type specified in curly braces
(additional condition is that the type must implement IEnumerable).

*/