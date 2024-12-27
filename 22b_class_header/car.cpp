/*

see 10_headers first, and car.cpp and car.h here

Notes:
1. Member variable fields: only the header should define member variables! the cpp shouldn't! //int Car::m_speed = 4; error!
non-static variable members such as m_speed (private or public) can only be defined using "default member initialization"
inside the class (in the header) - they can not be defined outside the class in a cpp file like static members.
check 36_member_initializer_list to see the construction phase and what the compiler chooses to initialize
the variable with (the value at the inline definition or the value supplied to the ctor)

2. Methods: for methods, the keywords, static, override, const should appear both in .cpp and in .h EXCEPT virtual:
which needs to exist only in h (initial declarations).
C++ Standard n3337 § 7.1.2/5 says:
The virtual specifier shall be used only in the initial declaration of a non-static class member function;
*/


#include "car.h"
#include <iostream>

std::string Car::brand = "ken";
int Car::m_wheel_amount = 4;


Car::Car()
{
	m_speed = 0; // possible to initialize members in a ctor
}

//implementing class functions there is no "using", There is no way to avoid the "Car::" qualification!
// like done with "using std" and then we can execute cout<<x, no "using" here.
void Car::accelerate(float intensity)
{
	m_speed += ACCELERATION_FACTOR * intensity;
}

void Car::brake(float intensity)
{
	m_speed -= BREAKING_FACTOR * intensity;
}

float Car::get_speed()
{
	return m_speed;
}


//user access control doesn't apply for defining private/protected/public forward declarations 
// of classes via scope resolution operator (::) for the sake of defining them!
// However actually using  private member variables / functions via scope resolution is forbidden
// they can only be accessed through an object.
// With the exception of static vars/functions members of a class only then :: operator is relevant
// for usability because these symbols actually belong to the class, and not to a specific object
void Car::transform()
{

}
