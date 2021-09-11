#include <iostream>
#include <typeinfo>

struct Animal
{
    virtual std::string Noise() const = 0;
    virtual ~Animal() = default;
};

struct Cat : public Animal
{
    std::string Noise() const override
    {
        return "meow";
    }
};

struct Dog : public Animal
{
    std::string Noise() const override
    {
        return "woof";
    }
};

struct Person
{
    void ReactTo(Animal* _animal)
    {
        if (typeid(*_animal) == typeid(Cat))
            RunAwayFrom(_animal);
        else if (typeid(*_animal) == typeid(Dog))
            TryToPet(_animal);
    }

    void RunAwayFrom(Animal* animal) { std::cout << "running away\n"; }
    void TryToPet(Animal* animal) { std::cout << "trying to pet\n"; }
};



int main()
{
    Cat dog;
    Animal* animal = &dog;
    Person person;
    person.ReactTo(animal);
}