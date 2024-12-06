#include <iostream>
#include "Array.h"

// NOTE: READ THIS FILE FIRST, not sure Array.h contributes anything here.

int main()
{
	Array<int> intArray(12); //Array template is parametrized by "int"
	
	for (int count{ 0 }; count < intArray.getLength(); ++count)
	{
		intArray[count] = count;
	}

	for (int count{ intArray.getLength() - 1 }; count >= 0; --count)
		std::cout << intArray[count] << '\n';

	return 0;
}

/*
The problem of seperating a templated class into a source.cpp and header.h:

tl;dr******************************************************************

Reminder: compliation flow given a non-template Array class (spearated into cpp (definition) 
and header (forward declarations):
1. Preprocessing: forward declerations of Array.h are included in main.cpp.
2. Compiler: cpps are compiled SEPARATELY to assembly (.s). The forward declarations from (1) 
satisfy the compiler - "definitions exist somewhere". 
each cpp is compiled as a seperate translation unit, unaware of other translation unity.
3. Assembler: the assembler compiler, produces machine code in obj from (2).
4. Linker: forward declarations are linked with compiled code (Definitions that satisfy the compiler 
(definitions satisfy the linker), and executable produced

Advantage: several classes can use Array by including Array.h and the linker will link these forward declerations to a single
source file (one defintion rule). Reduces compile time, link times and binary size.

*************************************
But with templates this gets complicated:
compiler only stencils out a template  when its instantiated:
Array<int> intArray(12);
Therefore in compilation phase (2), where all cpp's are compiled to ".s", 
the template code in Array.cpp WILL BE DISCARDED, since the compiler isn't aware of its instantiation in main.cpp
*************************************

Solutions:
1. All in header - easiest: 
place both the declaration and definition of all template functions directly in the header file.
This will cause the  template code to be included by the preprocessor in main.cpp and be available to be stenciled out.
Reminder: classes are exempt from ODR (the header can contain a class with definitions and can be included
into multiple files,even if its a template class code).
With non-templated fully defined header class, this inclusion would cause a bloated binary code (which is why
header(h) and cpp files are recommended). But with full defined templated header, as long as the class isn't
instantiated the code won't be included anyways.

2. Using inl files:
Array.h has a tempalte class with forward declarations for classes.
Then, an Array.inl containing the template function definitions is included in Array.h via "#include "Array.inl".
This is purely organizational, the result is jutst like in solution (1) where Array.h is included into main.cpp.
inl is a convention for inline file, the preprocessor dumps this code into the Array.h file.

3. Explicit Instantiation, given:
Header File (Array.h): Contains only declarations.
Source File (Array.cpp): Contains definitions for the template.
And a new additional file:
ArrayInstnace.cpp which includes both Array.h and Array.cpp (breaks cpp convention), and contains an 
explicit instantiation of the template for required types:
template class Array<int>;
template class Array<double>;

So now "ArrayInstance.cpp" is compileable to assembly (contains full template recipy), and we can simply include Array.h
to any cpp file we want, where its forward declaration will be linked by the linker to "ArrayInstance.cpp".
This resemebles most the classic header/cpp approach, solving code bloat in turn of complex setup.



Not sure about the following:
*************************************************************************************
declares a specialization of the template, with potentially different body.
template <> void foo<int>(int& t); 

causes an explicit instantiation of the template, but doesn't introduce a specialization. 
It just forces the instantiation of the template for a specific type.
template void foo<int>(int& t);
*************************************************************************************

*/
