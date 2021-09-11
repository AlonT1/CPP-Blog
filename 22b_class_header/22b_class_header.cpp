#include <iostream>
#include "car.h" // by providing the definitions, the linker can be sure that
// somewhere there is an implementation of the car class (car.cpp)

int main()
{
    Car car;
    car.accelerate(20);
    std::cout << car.get_speed() << std::endl;
    std::cout << Car::brand << std::endl;
}
