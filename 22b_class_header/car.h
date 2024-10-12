#pragma once

/*
Reminder:
headers contains forward declerations - all cpp files that want to use Car 
class can include this header file. forward declerations satisfy the compile -
"the definitions exists somewhere", during linkage the forward declerations will
be linked with the single source file that contains the defintions - saves binary size - 
instead of multiple files including full-fledged definitions, multiple forward declarations
will point to a single defintion file.

The question:
Why is it possible to potentially include a fully defined class in a header (functions can
be defined in the header or in a separate cpp) in multiple cpp files?
User defined types (such as classes) are exempt from ODR (one defintion rule - 1
definition across all the program). In other words, a single cpp cannot have multiple 
redifnitions of the same class. (Although a single cpp file cannot have the same class redifined multiple times).

This is possbile because:

	a. class member functions are exempt from ODR, as oppposed to normal functions that can be defined
	only once and refer to this definition via forward declaration (normal functions are extern by default).
	This is becuase class member functions are inline by default:
		1. It tells the compiler that the function code can be expanded where the function
		is called, instead of effectively being called.
		2. It tells the compiler that the function definition can be repeated.
		3. the compiler may consolidate all inline definitions into a single definition in memory

	b. member variables can also appear redifined across cpp files, since the values they are defined
	with are only used during construction (the ctor checks if it was supplied
	with a value to initialize the member, otherwise it checks if there is an "inline" definition
	of the member variable at declaration.
	THIS DOES NOT APPLY TO STATIC MEMBER VARIABLES - THEY are treated more as a global variable,
	embedded in the exe, thus they need only one single definition across all program

Including fully defined header is problematic for 2 reasons:

	1. leads to a bloated binary because each cpp that will include the header,
	which includes full defintions for each function of the class. 

	2. each small change in the fully-defined header causes the need for recompiling
	all the files that make use of the class. 

instead, when using a header with forward declarations only and a single defined cpp file,  
we can simply recompile the cpp definition of the class (recompilation of a single translation unit),
and relink to create exe.

Advantages for using header files with forwad declarations only:

	1. less bloat and better performance - it is better to separate the class into a header file
	that includes forward declerations (satisfies the compiler - "the definition exists somewhere"
	,and a source file that includes the defintions (satisfies the linker - during linkage all forward decleations
	included in cpp's that want to use Car, will "point" to 
	the single defintion file, which will reduce overall binary file size).

	2. The above also allows for "single-pass compilation" - translation units are compiled in a
	single pass even when only forward declarations exist. the linker will later link them up
	with their definition. This faster then "multi-pass" compilers such as java and c# (they 
	are more convinient - no header files).
 	Technically, C# does not have a traditional linker. Instead, it compiles to an intermediate language (IL),
  	which is then JIT-compiled at runtime by the Common Language Runtime (CLR).

	3. Separate interface from implementation - 
	headers make it easier to understand the class because they don't include implementation details,
	they provide a short summary of the class.

	4. no need to recompile all translation units, only the translation unit that changes were made to,
	all the forward declarations are already linked to the changed definition.




*/
using namespace std;
#include <iostream>
// Car.h will have all the "major" includes (vector, string, etc.)
// the implementation file Car.cpp imports Car.h and therefore includes everything that Car.h included.

class Car
{
//note that non-static members such as m_speed (private or public) can only be defined using default member
//initialization inside the class at member declaration
//or via ctor - they can not be defined outside the class in a cpp file like static members.
//check 36_member_initializer_list to see the construction phase and what the compiler chooses to initialize
//the variable with (the value at the inline definition or the value supplied to the ctor)
private:
	int m_speed;
	const int ACCELERATION_FACTOR = 10;
	const int BREAKING_FACTOR = 30;
	void transform();
public:
	Car();
	static std::string brand;
	static int m_wheel_amount;
	void accelerate(float intensity);
	void brake(float intensity);
	float get_speed();
};
