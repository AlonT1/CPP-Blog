#include <iostream>
#include <string>
#include <variant> //for std::variant and std::visit
#include <vector>


/****************CRTP (Curiously Recurring Template Pattern)****************
coined by Jim Coplien in 1995 after observing this recurring pattern.

1. The naive solution (as seen in 122_double_dispatch), violating DRY and repeating ourself:
instead of class A exclusively holding foo(), we copy and paste foo() directly into B & C.
Then static dispatch will deduce "this" as B or C since foo() is defined inside B & C.
The problem with this tactic is that we violate DRY. As seen in 122_double_dispatch,
we duplicate "rock_visitor.vehicle_visiting_rock(*this)" in Lamborghini and Vehicle

*******************************************************************************
2. The better solution: CRTP - a pattern which consists of 3 steps:

1. creating a class A which is a template, and class B inherits
from A and parametrizes the template parameter as B (itself).

2. creating a ptr/ref of type A which points/refers to an object of type B, and
parametrize the template parameter with B e.g:
A<B> ref = &B.
the parameterization can help us know at compile time the run-time type of "ref"
which is B and not A

3. Then class A can utilize this information by casting "this" (which is always A)
to T which represents the run-time type of the reference - helping class A in implementing
functions that utilize "this" keyword in once place (in class A), instead
of duplicating the same functions to class B,C,D... just because we can then
utlize "this" within them to know if we're inside B, C or D.
Now we can keep one function in A and use "this" combined with the parameter type 
and know who "this" actually refers to.
********************************************************************************

in our case:
Vehicle is a template and class Mercedez inherits from Vehicle and parametrizes
the template parameter to Mercedez (itself). Same goes for Lamborghini.

This enables us to parameterize the polymorphic ref/ptr with the run-time type
it points/refer to, thus making it available to know at compile-time:

(1) Vehicle<Lamborghini>* vehicle_ref = new Lamborghini();
vs
(2) Vehicle* vehicle_ref = new Lamborghini();

with (1) we can know that vehicle_ref points to (its run-tim type is) Lamborghini
thanks to the fact that we parametrize it with <Lamborghini>,
with (2) we can only know the run-time type of vechile_ref at run time using
3 techniques shown in 122_double dispatch (dynamic_cast, typeid  and double dispatch)

Now we can use functions declared in A and utilize "this" combined with the parameter
type T which contains the run-time type of the ptr/ref - 
we can cast "this" to T! 
for exampel in Vehicle class, in func visit, we cast "this" (which is always a Vechile) to T 
(Lamborghini - the run time type of the pointer). This saves us duplicating
the visit function to Mercedez and Lamborghini for the mere need to know who "this"
is (when using "this" within a function in Lamborghini, "this" will 
always refer to Lamborghini). Now we can leave visit() in Vehicle and cast 
"this" to T where T is the run-time type of the ptr/ref, in other words we can 
declare T is as either Lamborghini or Mercedez.*/


struct Rock;
struct Mercedez;
struct Lamborghini;

struct RockVisitor
{
    Rock* rock;
    void invoke_collision(Mercedez* merecedez);
    void invoke_collision(Lamborghini* lamborghini);
};

template <typename T>
struct Vehicle
{
    void visit(RockVisitor rock_visitor) { rock_visitor.invoke_collision(static_cast<T*>(this)); }
};
struct Mercedez : /*the following is the CRTP*/ Vehicle<Mercedez> {};
struct Lamborghini : Vehicle<Lamborghini> {};

struct Rock
{
    template <typename T>
    void collide_with(Vehicle<T>* vehicle) // T is Lamborghini
    {
        RockVisitor rock_visitor{ this };
        //vehicle is of type Vehicle<Lamborghini> and when calling visit of 
        //Vehicle we utilize the fact that T represents Lamborghini!
        //thus we cast this to Lamborghini. Essentially T preserves the identity (run-time type)
        //of Vehicle but this is done at compile-time! 
        vehicle->visit(rock_visitor); 
    }

    void collide_with(Mercedez* mercedez) { std::cout << "Mercedez Collsion"; }
    void collide_with(Lamborghini* lamborghini) { std::cout << "Lamborghini Collsion"; }
};

void RockVisitor::invoke_collision(Mercedez* merecedez) { rock->collide_with(merecedez); }
void RockVisitor::invoke_collision(Lamborghini* lamborghini) { rock->collide_with(lamborghini); }

int main()
{
    Rock rock;
    Vehicle<Lamborghini>* vehicle_ref = new Lamborghini();
    rock.collide_with<Lamborghini>(vehicle_ref);
}
