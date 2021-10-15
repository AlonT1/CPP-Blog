#include <iostream>
#include <string>
#include <variant> //for std::variant and std::visit
#include <vector>

/*Applies the visitor vis (Callable that can be called with any combination 
of types from variants) to the variants vars.
https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/#more-1692
http://www.vishalchovatiya.com/double-dispatch-in-cpp/
*/

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

struct Mercedez : Vehicle<Mercedez> {};
struct Lamborghini : Vehicle<Lamborghini> {};

struct Rock
{
    template <typename T>
    void collide_with(Vehicle<T>* vehicle)
    {
        RockVisitor rock_visitor{this};
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
    rock.collide_with(vehicle_ref);
}
