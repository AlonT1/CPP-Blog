/*
Notes:

**************************************************************************
1. when using a header and a cpp, the cpp should only include 
implementation of functions in the following format: return type, name and parameters.
keywords such , override, virtual, static, __declspec(dllimport) 
(49_static_dynamic_linking), should only be used in the header, where the functions
are declared
Why?
Header file == class interafce ==  function signatures and behaviors (whether they are virtual, static, etc.). 
It tells the compiler and anyone using the class what they can expect in terms of functionality.
By marking a function as virtual in the header file, you inform users of the class and the compiler that
this function is intended to be overridden in derived classes (interaction).
The cpp on the other hand contains implementation details.


C++ Standard n3337 § 7.1.2/5 says:
The virtual specifier shall be used only in the initial declaration of a non-static class member function;
**************************************************************************

2. cpp should not include class variable member definition //int Car::m_speed = 4; error!
non-static variable members such as m_speed (private or public) can only be defined using "default member initialization"
inside the class (in the header) - they can not be defined outside the class in a cpp file like static members.
check 36_member_initializer_list to see the construction phase and what the compiler chooses to initialize
the variable with (the value at the inline definition or the value supplied to the ctor)

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
//of classes via scope resolution operator (::)! this only applies for creating definitions!
//However using/reading  static private members / functions via scope resolution is forbidden
//(static because private members/functions belong to the class, only then :: operator is relevant)
void Car::transform()
{

}
