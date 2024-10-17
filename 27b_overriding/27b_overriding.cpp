
//function overriding - occurs only between classes and inherting subclasses: 
//when a function in a subclass has the same name and parameters as the function
//in base class, but with different definitions.

//as opposed to function overloading which evloves normal (outside-class) functions (9_functions)

#include <iostream>

class Animal
{
public:
    void eat(int meals) { std::cout << "Animal Eating" << '\n'; }
};

class Cat : public Animal
{
public:
    //public inheritance will inherit all public interface of Animal
    //therefore eat() of Animal exists here
};


class Character
{
public:
    void jump(int height) { std::cout << "character jump " << height << '\n'; }
};

class Mario : public Character
{
public:
    void jump(int height) { std::cout << "mario jumps " << height << '\n'; }
};

int main()
{
    //Non overriding example
    Cat cat;
    cat.eat(4); //executes the function in Animal

    //overriding example
    Mario mario;
    mario.jump(4); //because we overriden jump() in mario with the same params and name
    //this function will execute

    //but what if
    Character* charachter = &mario;
    charachter->jump(4); // executes jump of Character class although the pointer
    //points to mario object! this can be solved by polymorphism which is enabled
    //in cpp by virtual functions - explained in 28_virtual_function
    // (polymorphism is not possible for stack allocated object)
}
