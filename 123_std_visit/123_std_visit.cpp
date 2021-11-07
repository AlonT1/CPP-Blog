#include <iostream>
#include <variant>

//https://www.cppstories.com/2018/09/visit-variants/

struct Vehicle {};
struct Lamborghini : Vehicle {};
struct Mercedez : Vehicle {};

struct Rock 
{
    void operator()(const Mercedez&) { std::cout << "Mercedez\n"; }
    void operator()(const Lamborghini&) { std::cout << "Lamborghini\n"; }
};

int main() 
{
    /*
    tl;dr
    Animal* animal = &cat; animal->walk() - via rtti the compiler knows to run walk() of Cat.
    Vehicle* vehicle = &mercedez; rock.collide_with(Vehicle) - because functions are binded
    at compile time, the run-time type of vehicle(merecedez) cannot be deduced
    (RTTI available only at run-time), thus the call
    invokes collide_with which takes in the underlying type of the ptr (Vehicle*) => std::visit 
    ************************************************************************************************
    std::visit solves this - it can invoke the correct function based on the run-time type of a ptr/ref.
    ************************************************************************************************


    Long version
    what std::visit solves?
    RTTI is utlized when calling a function through a polymorphic pointer.
    e.g: Cat inherits from Animal and overrides Animal's virtual function walk().
    Animal* animal = &cat;  animal->walk()  will invoke walk() of Cat because
    the run time type of animal is Cat. This is called dynamic dispatch - binding
    of a function at runtime according to the runtime type of the ptr/ref which is deduced
    via RTTI - a mechanism that reveals the run-time type of Animal.
    (only possible when B inherits from A, B overrides a virtual function of A, 
    and A is polymorphic ptr/ref to B (inheritance, virutal function, polymorphism - 
    where one symbol is able to represent multiple symbols).

    RTTI cannot be utilized when the polymorphic pointer is used as an argument to a function.
    e.g: rock.collide_with(vehicle), where vehicle is a polymorphic pointer that points
    to Mercedez (rock can be polymorphic or not.). function calls based on arguments
    are only determined at compile-time via name mangling where the actual function name
    is mangled according the the argument names. Since RTTI is run-time only, and the
    run-time type of vehicle (Mercedz) isn't known at compile-time, the call
    rock.collide_with(vehicle) is binded to a colldie with which takes in a Vehicle*
    and not Mercedez*.

    This can be fixed via double dispatch, crtp or the simplest way - std::visit:
    */


    // car can be either Lamborghini or Mercedez - and we decide to init car
    // with Mercedez(), hence car is of type Mercedez
    std::variant <Lamborghini, Mercedez> car{ Mercedez()};
    Rock rock;

    // std::visit unveils the run-time type of car (Mercedez) and is fed to the correct
    //function of rock which takes in a Mercedez type.
    //this isn't possible with a "classic" call rock.collide_with(car) (explained above).
    std::visit(rock, car);
}