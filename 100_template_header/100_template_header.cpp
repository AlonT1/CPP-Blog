#include <iostream>
#include "Array.h"

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

non-template version of Array class:

1. Preprocessing: forward declerations of Array.h are included in main.cpp via the preprocessor
2. Compiler (to assembly): Conversion to assembly (.s) of main.cpp and Array.cpp (contains defintions of Array).
at this moment main.cpp only contains the forward declarations of Array.h.
(forward declarations satisfy the compiler - "definitions exist somewhere"). 
each cpp is compiled as a seperate translation unit, unaware of other translation unity.
3. Assembler: the assembler compiler uses the assembly of phase 2, produces machine code in obj. files per cpp file.
4. Linker: the forward declarations of Array.h included in main.cpp, are linked  with array.cpp  -> the 
(definitions satisfy the linker)
(many classes can use Array by including Array.h and the linker will link these forward declerations to a single
source file that contains their defintion (one defintion rule), also reduces compile time, link times and binary size.
(no need for full-fledged definitions in multiple files, the forward declerations just point to a single source file).


template version of Array class:

1. Preprocessing: forawrd declerations of Array.h are included in main.cpp
2. Compiler: (each cpp file is compiled as an atomic unit)
	a. main.cpp - if an array-template type is instantiated, the template class definition (which includes only forward declerations)
	is copied into main.cpp with the requested type (Array<int> arr;   -> int version of Array class is stenciled out,
	a class that contains forward declerations of type int). the binary will include this forward declarations in main.obj.

	b. array.cpp - ***********************BECAUSE FILES ARE COMPILED ATOMICALLY, 
	getLength() IN array.cpp WON'T BE STENCILED OUT because array.cpp (and the compiler) have no indication that an 
	Int version of class Array was instantiated in main.cpp via "Array<int> intArray(12)".
	THEREFORE we're LEAVING THE FORWARD DECLARATION of getLength() INCLUDED by the "Array.h" header IN MAIN.CPP WITHOUT DEFINTION, that should have came from array.cpp***********************************************.
	where is the definiton?
	definitions for uninstantiated tempaltes such as getLength() in array.cpp are only stenciled at compile-time,
	and array.cpp has no indication that a stencil was instantiated, therefore the definition of the class
	won't be even included in the binary! same goes for unused templates, they are only included in the binary
	if they are stenciled out, otherwise their code won't be appear in the program, as opposed to non-templated definitions.
	(compilation is done per-file, atomically, they don't "communicate" with each other during this phase).
	This is in contrary to a non-templated function / class definitions that would have been compiled into a binary, even if not used.

3. Assembly: via assembly of phase 2, produces machine code in obj. files
4. Linker: links obj of main.cpp with array.cpp -> because array.obj is empty and doesn't contain
any defintions for the forward declerations of Array<int> class inside main.cpp, the linking stages fails!



solutions:

1. (easiest) put all template class code including defintions in the header file (definitions can be
inside or outside the class). this works because classes are exempt from ODR (one defintion rule), the header can contain
a class WITH defintions inside of it. the header can then be included into multiple files.
that want to use the class. with this technique there is a difference between
template and non-template classes:

	a. non-template classes: When dealing with non-template classes, the issue with this technique is that it will lead to a bloated
	binary because each cpp that includes the header will now hold full defintions for each
	function of the class. Instead it is better to separate the class into a header file
	that includes forward declerations, and a source file that includes the defintions.
	during linkage all forward decleations included in cpp's that want to use Car, will "point" to
	the single defintion file, which will reduce overall binary file size. see 22b_class_header for more info.

	b. template classes: template classes are more forgiving when using this technique -
	despite the potential bloat that an "include" can cause, unless the template-class is instantiated in the cpp,
	the class won't be included in the binary of the cpp (the compiler won't generate a definition of it with the requested type).
	This stands in contrast to a non-template class which will be included in the binary whether the class is instantiated or not.
	SO THE UPISDE IS: easy implementation. Downside: Bloating through many local copies of the template - can increase
	compile time, link times and binary size. IN ADDITION (linkers may optimizie this duplications?? really not sure about this one)

2. inl (inline) file (more structured, but doesn't solve bloating)

	very similar to the solution above:
	we have an header file, that like above will include both forward declarations AND defintions, but!
	we separate the definitions into a file called .inl, and then include .inl to the header file.
	then, like above we include the header file to other to any .cpp that wants to use Array class.
	this is a more structured solution.
	
	inl files have no significance to the compiler, they are just a way of structuring the code for readability, JUST AS .h, .hpp etc.
	have no meaning to the compiler. In other words, the extensions of the files we INCLUDE don't have a meaning, they are
	dumped in the cpp's by the preprocessor (in terms of compilation, cpp extension does matter - the compiler looks for a main function there,
	in terms of including files, the extension does not matter they are just copied).
	so if header files usually contain forward declerations for classes that want to use the forward declared functions/classes (the linker
	will link those declarations with a defintion during linkage), inl files usually contain extra code that was separated, and supposed to
	be "inlined". The extension of the file could also be an .h, it doesn't matter, the extension just informs us about the
	purpose of the file.

afterwards that header file, could be included in cpp files that want to use the template class.
Upside: code is more organized. downside: additional file ".inl" file and we didn't solve the potential bloat problem.


3. three-file-approach (most efficient, but requires 3 files):
	The problem with approach 1 and 2 is that each cpp file that wants to use the template class, must
	be provided with a full definition of the template class - this causes each cpp file to have the same
	stenciled out template..
	in this approach we pre-instantiate (pre-stencil) a template in a cpp file, and then each cpp file
	that wants use the template (i.e contains statements such as: Array<int> array;)
	will use forward declarations from a header file to link up with the a cpp file 
	that contains a full-fledged instantiation of the templated class.(template.cpp)

	requires:
	a. forward declarations - inside an header file (Array.h)
	b. defintion file - a cpp file (Array.cpp)
	c. templates.cpp file - includes Array.h, Array.cpp (not recommended, breaking best practice by including a cpp)
	and then we ****explicilty instantiate the Array template in the template.cpp file with the following statement*****:
	***********************************************************************************************
	"template class Array<int>;" the compiler will stencil out an Array<int> (class Array {...int types..}).
	***********************************************************************************************

	Remember that the problem here is having a definition of a function (belonging to a templated class) inside
	a y.cpp while the template is stenciled out in x.cpp (with forwrard declarations), and because x isn't aware of y
	during compilation,  the compiler sees no reason in creating the definitions in y.cpp because it didn't know that the template
	was instantiated in x.cpp, and then the linker won't be able to link the forward declarations included in x.cpp,
	with the non-created definition in y.cpp.

	because in our solution, templates.cpp includes forward decleration + defintion + explicit instantiation of Array of type int,
	all inside a single cpp file, the compiler will succesfully generate an int Array stencil.

	d.********and then we can include Array.h (forward declarations) in other cpp files who wants to use the stenciled out
	version defined inside templates.cpp. the forward declarations will satisfy the compiler, and the linker
	will link those forward declarations with stenciled definitions in templates.cppp********** 
	Instead of providing each cpp file that wants to instantiate our template class a complete fully defined version of the template
	(like in method 1 (all in header) or method 2 ( header with inl) - both of them do not contain explicit instantiation of the class,
	we rely on the client to instantiate the template),
	here we pre-instantiate the template successfully in templates.cpp (because it contains all forward declarations+ definitions
	+explicit instantiation and then each cpp file that wants to use the templated class:
	i.e: a .cpp file can instantiate the tempalte via: "Array<int> intArray(12);"
	templates.cpp will be provided with forward declarations (Array.h) that will be linked up with the already-pre-instantiated
	and defined template that resides in templates.cpp file.
	Note: the file that includes an "Array.h" must obviously instantiate the class, otherwise the forward
	declerations included by "Array.h" won't even be included in the binary to be linked up with the defintions inside templates.cpp
	(uninstantiated templated-class are not included in binary).

Upside: solving the bloat problem - defintions are in a single place (templates.cpp), and forward declarations
will be linked up with them. Downside: complex setup that requires 3 files + templates.cpp includes an "include"
of a cpp file (breaking a practice - including a cpp file).
note: if many files need different versions of the template, for example
Array<int> intArray(12); 
Array<double> intArray(12);

we can pre-instantiate all variations inside templates cpp
template Array<int> intArray(12);
template Array<double> intArray(12);

and the same story goes, all cpp files will use Array.h to satisfy the compiler
with forward declarations, and the linker will link them up with templates.cpp
that contains pre-instantiated versions of the template class.


******************************************************************



additional read - same that is written above but in other words:

with non-templated classes, they are usually separated into an header file and a source file.
1. the header file contains the class with only forward declerations inside of it. The header can
be included into other cpp files that want to use the class. the forward declerations
inside the class satisfy the compiler by guaranteeing him that somewhere, in some cpp file the definitions of
these forward declerations exist.
2. the source file contains the definitions of the forward declerations inside the class. According
to the ODR rule (One Definition Rule), only one defintion of the class may exist. the definitions
satisfy the linker - after the compiler saw that the forward declerations exist and succesfully
created .obj files, the linker will now link  all the forward declerations with the single source file
that contains the definitions. all forwrad declerations now "point" at their defintion inside source.cpp -
this reduces binary size (definition appears only once in source.cpp, although many other files want to use our class, they only
possess forward declerations of it, instead of full-fledged defintions).

with templated class things are different. A template is not a class or a function - it is a stencil.
Assuming the header file contains forward declerations of the templated class, and the header is included
into main.cpp that instantiates a template type, during compilation, a template class definition will be
created (e.g: Array<int> arr;  -> compiler will create complete int version of class Array - T will be replaced by int).
THE PROBLEM IS THAT THESE PROCESS OF VERSIONING AN INT DEFINITION OF CLASS ARRAY ONLY HAPPENS IN THE FILE
THAT INSTANTIATES THE TEMPLATED-VERSION. IT WILL NOT HAPPEN IN Array.cpp that holds the definition for the forward declerations.
In other words, compilation starts with main.cpp, int version of Array class is created -> assembly code created.
now compilation moves to Array.cpp (definition file) -> the compiler sees definitions such as:
template <typename T>
int Array<T>::get_length() const
{
	return m_length;
}
but the compiler has no idea what happened in main.cpp (compilation is done per file basis, atomically),
it doesn't know that main.cpp instantiated an Array class of int version, and there is no indication
for that in Array.cpp. Therefore, as templates are not created if they are not instantiated, these definitions
won't be even compiled and included in the binary (as opposed to non-template definitions which are compiled even if they
are not called).

when reaching the linking stage, the linker will try to search the definitions for the int
forward declerations of class Array that exists in main.obj, but it will fail because they
don't exist, Array.obj is empty!

This doesn't happen in non-templated version of Array because the definitions of Array.cpp would
have been compiled into an Array.obj which would have been linked up succesffuly with the forward declerations
in main.obj


*************************************************************************************
declares a specialization of the template, with potentially different body.
template <> void foo<int>(int& t); 

causes an explicit instantiation of the template, but doesn't introduce a specialization. 
It just forces the instantiation of the template for a specific type.
template void foo<int>(int& t);
*************************************************************************************

*/