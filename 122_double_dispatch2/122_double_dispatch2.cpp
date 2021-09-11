#include <iostream>
#include <typeinfo>

class Person;
class Dog;
class Cat;

struct ReactionVisitor
{
    Person* m_person = nullptr;
    explicit ReactionVisitor(Person* person) : m_person{ person } {}

};

struct AnimalVisitor
{
    virtual void Visit(Cat*) = 0;
    virtual void Visit(Dog*) = 0;
    /*...*/
};

struct ReactionVisitor : public AnimalVisitor
{
    Person* m_person = nullptr; // person doing the reacting

    void Visit(Cat*) override {}

    void Visit(Dog* _dog) override 
    {
        m_person->RunAwayFrom(this);
    }

};

struct Animal
{
    virtual std::string Noise() const = 0;
    virtual ~Animal() = default;
    virtual void Visit(ReactionVisitor& visitor) = 0;
};


struct Person
{
    void ReactTo(Animal* _animal)
    {
        ReactionVisitor visitor{ this };
        _animal->Visit(visitor);
    }

    void RunAwayFrom(Animal* animal) { std::cout << "running away\n"; }
    void TryToPet(Animal* animal) { std::cout << "trying to pet\n"; }
};


struct Cat : public Animal
{
    void Visit(AnimalVisitor* visitor)
    {
        visitor->Visit(this);
    }

    std::string Noise() const override
    {
        return "meow";
    }
};

struct Dog : public Animal
{
    void Visit(AnimalVisitor* visitor)
    {
        visitor->Visit(this);
    }

    std::string Noise() const override
    {
        return "woof";
    }
};



int main()
{
    Dog dog;
    Animal* animal = &dog;
    Person person;
    person.ReactTo(animal);
}

//https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/#more-1692