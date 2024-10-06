class SpaceShip {};
class ApolloSpacecraft : public SpaceShip {};

class Asteroid
{
public:
    virtual void collide_with(SpaceShip&) { std::cout << "Asteroid hit a SpaceShip\n"; }
    virtual void collide_with(ApolloSpacecraft&) { std::cout << "Asteroid hit an ApolloSpacecraft\n"; }
};

class ExplodingAsteroid : public Asteroid
{
public:
    virtual void collide_with(SpaceShip&) override { std::cout << "ExplodingAsteroid hit a SpaceShip\n"; }
    virtual void collide_with(ApolloSpacecraft&) override { std::cout << "ExplodingAsteroid hit an ApolloSpacecraft\n"; }
};


#include <iostream>

int main()
{
    /****************Testing function overloading (static dispatch - at compile time)******************
    
    "asteroid.collide_with(space_ship);"  - undergoes static dispatch:
    asteroid is variable instance on the stack
    this "violates" one of the 3 conditions that enable dynamic dispatch: "asteroid"
    isn't a polymorphic ref/ptr (see 28_virtual_functions) - therefore static dispatch will occur.

    During the compilation and as a part of static dispatch, the compiler peforms name-mangling
    to functions - this helps to differ between them without any vtable mechanism, even if they
     overload each other (with the same name - but with different params. This enables
    function overloading, a.k.a compile-time polymorphism where a single function (symbol)
    can represent multiple types of functions based on their parameters, and the call to this
    functions is resolved at compile time, where the call is linked to the body based
    on the argument.
    different return type don't affect function overlading - and probably not name mangling,
    meaning that 2 functions with same name + params but different ret types is not enought
    for the compiler to perform function overload and differentiate between them).
    For example:
    collide_with(SpaceShip&) becomes something like ___collide_with__spaceship
    collide_with(ApolloSpaceCraft&) becomes something like ___collide_with__apollospacecraft.
    and becuase we perform static dispatch, spaceship_ref is sliced to SpaceShip and the first function is chosen.
*/

     //spaceships
    SpaceShip space_ship;
    ApolloSpacecraft apollo_spacecraft;

    //asteroid
    Asteroid asteroid;
    asteroid.collide_with(space_ship); // invokes Asteroid::collide_with(SpaceShip&) as expected
    asteroid.collide_with(apollo_spacecraft); // invokes Asteroid::collide_with(ApolloSpacecraft&) as expected

    //Exploding Asteroid
    ExplodingAsteroid exploding_asteroid;
    exploding_asteroid.collide_with(space_ship); // invokes ExplodingAsteroid::collide_with(SpaceShip&) as expected
    exploding_asteroid.collide_with(apollo_spacecraft); //invokes ExplodingAsteroid::collide_with(ApolloSpacecraft&) as expected

    std::cout << "\n";


    /***************Dynamic dispatch - works******************
    2. "asteroid_ref.collide_with(spaceship_ref);" - undergoes dynamic dispatch:
    dynamic dispatch is chosen here due
    to the fact that all 3 conditions for dynamic dispatch coexist:
    a. inheritance (ExplodingAsteroid inherits from Asteroid)
    b. virtual function override - ExplodingAsteroid overrides Asteroid's collide_with virtual function
    c. polymorphic pointer / reference - a reference of type Base (Asteroid) represents/refers a
    sub-class (ExplodingAsteroid).
    Note: for polymorphism to exist we don't necessarily need "b.", but without virtual functions,
    there isn't much need for polymorphism anyway(?)
    ************************************************************************************
    Note that dynamic dispatch only works when we invoke a function from an instance, based
    on the run-time type of that instance from which we invoke the function
    (asteroid_ref run-time type is ExplodingAsteroid,
    therefore we will invoke collide_with of ExplodingAsteroid as expected).
    As seen in the "problem" example below, dynamic dispatch DOES NOT work in a scenario where we 
    want to invoke a method based on the run-time type of an ARGUMENT!
    ************************************************************************************/

    Asteroid& asteroid_ref = exploding_asteroid;
    asteroid_ref.collide_with(space_ship);
    asteroid_ref.collide_with(apollo_spacecraft);

    std::cout << "\n";



    /***************The problem is here!!!!!!!!!!!!!!!!!!!!!!!!***************
    invoking a method of a class based on the run-time type of a parameter
    is not possible because static dispatch , a.k.a linking the call with its implementation at 
    compile time - only occurs at compilet time - while the run-time type of an object 
    can only be evaluated at run-time when it is directly used.
    therefore "spaceship_ref" is supplied to a function supporting Spaceship& (the underlying type
    of the ref known at compile-time) and not to a function  which supports the run-time type of spaceship_ref
    which is ApolloSpaceCraft because it only known at run-time (the run-time type can be queried via
    3 techniques as seen in 122_double_dispatch), after the dispatch already occured.
    Because of this compile-time polymoprhism, a.k.a function overloading based on the
    run-time type of an object fails (function overloading is resolved at compile-time,
    before we know the run-time type of spaceship_ref
    
    The solution is in 122_double_dispatch or 122a_crtp or 123_std_visit*/
    

    SpaceShip& spaceship_ref = apollo_spacecraft; //although spaceship_ref refers to an ApolloSpaceCraft object......
    asteroid.collide_with(spaceship_ref); // (1) Asteroid collides with SpaceShip and not with ApolloSpaceCraft (run-time type of spaceship_ref)!!!
    asteroid_ref.collide_with(spaceship_ref); //(2) ExplodingAsteroid collides with SpaceShip and not with ApolloSpaceCraft!!!
    /**************************************************************
    Bottom line: if ptr x of type A points to B (polymorphic), B inherits and overrides a virtual function from A,
    we can (dynamically dispatch) invoke any function through x  (x.func_name())
    BUT! when providing x as an arguemnt to functions, x will be interpreted according
    to its underlying type (A) and not by its run-time type B (it points to B).
    this is because functions are bind at compile time and the run time type of x (B)
    can only be known at runtime. This also happens with non-classes:

    struct A { virtual void foo() { }};
    struct B: public A { virtual void foo() override { }};
    void wr(A* a) {}
    void wr(B* b) {}

    int main()
    {
        A* a = new B;
        wr(a); ---------->>> although the run time type of "a" is B, wr
        will be chosen according the underlying type of a which is A, since
        only the static type of a is known at compile time, which is when 
        static polymprhism (function overloading) occurs
    }

    *****************************************************************/

}
