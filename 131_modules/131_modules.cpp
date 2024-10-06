#include <iostream>
// https://www.modernescpp.com/index.php/c-20-modules
// https://stackoverflow.com/questions/22693950/what-exactly-are-c-modules
// https://vector-of-bool.github.io/2019/03/10/modules-1.html
// 
// MOUDLES VS HEADERS!
//multiple cpp's can include headers such as iostream this increases compile time (remeber that #pragma once
//guards only protect agains import ).
// if we have math.cpp, math.h and array.cpp, array.h. and main.cpp.:
// math.h and array.h both include iostream. because math.cpp and and array.cpp both include math.h and array.h
//respectively, they also include iostream. main.cpp includes iostream, math.h and array.h.
// During compilation iostream will be compiled once for each of the following files
// math.cpp, array.cpp and main.cpp (although main.cpp includes iostream three times: once by itself
// and twice through math.h and array.h, because of #pragma once guard, the preprocessor that pastes
// iostream into main.cpp will not paste iostream more then once)
// So basically because the compiler compiles each translation unit atomically, iostream has been compiled
// three times!
// This is even more severe with headers that also contain the implementation via inline functions (for not
// violating ODR), and less severe when the headers contain only forward declarations (the compilation
// is then faster because then the linker does the "heavy lifting" by linking all forward declerations
// (i.e function calls) to a single implementation file. regardless if the header contains only forward
// declerations and/or implementations, the header must be recompiled together with the files that "include"
// them.
// 
// Modules on the other hand....
// 
//main.cpp 
// nstead of performing a copy and paste when a #include preprocessor directive is found, 
// they will read a binary file that contains a serialization of the abstract syntax tree that represents the code.


#include <iostream>

//since roman module doesn't explicitly use "export <string> module", we must explictily import it!
//this is different when compared to header files, and a benefit in general
import <string>;

// Once a module is imported, we can use all entities (functions, etc) that it explictly exports.
// what about definitions, such as std::string_view that are not explicitly exported?
// How can we use functions that use std::string_view considering the fact that the module
// doesn't export it and we dont import string_view here?
// By importing roman, the definitions of both unexported modules such as
// std::string and std::string_view  become REACHABLE
// thus allowing US to invoke "from_roman(std::string_view)", although we didn't import string_view to this file.
// we can even invoke member functions of the reachable class, for examplee to_roman(1234).c_data() (the members
// of a reachable class definition are called VISIBLE)

// BUT! the entity on its own is not visible, i.e we cannot instantiate std::string_view x{};.
// in other words, we CAN indirectly interact with the REACHABLE string_view and its VISIBLE members,
// through classes and function that make use of it, but we cannot directly interact with string_view,
// and create instances of it, because it isn't explicity exported for use. This is a compromise, otherwise
// a module would have to export all other modules that it uses just to work, and this undermines
// our abillity to choose exactly what to export. In this setup we instantiate 
// the "meat" of the module and interact with it. all other "secondary" modules that are not marked
// with "export" (string,vector, etc...) are there just to facillitate the functionality of the module,
// we cannot actually instantiate them.

// This is an advantage over header files, because now they are encapsulated: by importing a module
// we interact only with what the author wants us to interact with, and we are not allowed to 
// instantiate and use everything that the header file and its implementation have, here we interact
// and instantiate only with what is explicitly exported for use


import roman; // no need for angle brackets around our own modules. 
int main()
{
	std::cout << "1234 in Roman numerals is " << to_roman(1234) << std::endl;
	std::cout << "MMXX in Arabic numerals is " << from_roman("MMXX") << std::endl;
}