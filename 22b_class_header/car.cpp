/*
Notes:

**************************************************************************
1. when using a header and a cpp, the implementation of a function in a .cpp should only include return type,
name and parameters. keywords such , override, virtual, static, __declspec(dllimport) 
(49_static_dynamic_linking), should only be used in the header, and discarded in .cpp

C++ Standard n3337 § 7.1.2/5 says:
The virtual specifier shall be used only in the initial declaration of a non-static class member function;
**************************************************************************

2. //int Car::m_speed = 4; error!
	non-static members such as m_speed (private or public) can only be defined using "default member initialization"
	inside the class or via ctor - they can not be defined outside the class in a cpp file like static members.
	check 36_member_initializer_list to see the construction phase and what the compiler chooses to initialize
	the variable with (the value at the inline definition or the value supplied to the ctor)

*/


#include "car.h"
#include <iostream>

std::string Car::brand = "ken";
int Car::m_wheel_amount = 4;


Car::Car()
{
	m_speed = 0;
}

//with namespaces we can utilize "using" to avoid repeating the  namespace name
//when implementing functions from the .h file in .cpp file, or wrapping the whole
//implementation inside a namespace with the same name of the namespace from the header file. But! When 
//implementing class functions there is no "using", There is no way to avoid the "Car::" qualification!
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
//of classes via scope resolution operator (::)! this only applies for creating definitions!
//However using/reading  static private members / functions via scope resolution is forbidden
//(static because private members/functions belong to the class, only then :: operator is relevant)
void Car::transform()
{

}