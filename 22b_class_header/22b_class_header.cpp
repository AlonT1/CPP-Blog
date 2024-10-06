#include <iostream>
#include "car.h" // by providing the definitions, the linker can be sure that
// somewhere there is an implementation of the car class (car.cpp)

/*
Note - with namespaces this syntax is possible:
// header.h (1)
namespace MyNamespace { void myMethod() ; }

// source.cpp (2)
namespace MyNamespace { void myMethod() { /*definition/ } } //tl;dr this is not possible with class due to ODR

//or source.cpp (3)
 void MyNameSpace::myMethod() {}  

 with classes, this syntax is possible (style 1 and 3 but not 2!):

 // header.h (1a)
class Entity { void myMethod() ; }

//or source.cpp (3a)
 void Entity::myMethod() {}

 THE SOURCE FILE CANNOT LOOK LIKE THIS
 // source.cpp (2a)
class Entity { void myMethod() { /*definition/ } }

THIS WOULD BE A REDEFINITION OF A CLASS THAT WAS ALREADY
DEFINED IN THE HEADER (type redefinition).
since namespace are not a type, both (2) and (3) are possible.
https://stackoverflow.com/a/112451
*/
or source :
namespace MyNamespace
{
   void myMethod()
   {
      // etc.
   }
}

*/


int main()
{
    Car car;
    car.accelerate(20);
    std::cout << car.get_speed() << std::endl;
    std::cout << Car::brand << std::endl;
}
