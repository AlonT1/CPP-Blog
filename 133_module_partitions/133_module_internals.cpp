// as seen in 131_modules, modules can be separated into two:
// 1. "module interface files" (.ixx) - like a classic header file (.h),
// contain only declarations, thus serve as a quick overview of the module.
// these files usually start with "export module module_name" and everything
// inside them that is marked with "export" is reachable by the implementing file.
// 2. "implementation files" (.cpp) - the actual code (body) of the declarations
// in the "module interace files". these file start with "module module_name".

// but what if want to have a module that consists both of "main" functionn that we are supposed
// to interact with (i.e reachable functions) AND functions that are helper (utility) functions.
// the following techniques are available:
// 1. A single module with "main" functions with marked with "export" and 
// the utility functions without "export". the problem is that if we have a very long module
// the implementation file (.cpp or the module itself that contains the implementation) becomes
// very long. so how can we separate the "utilities" from "main" functions?
// *****************************************************************************************
// 2. By using "module implementation partitioning" sometimes called internal partitions:
// for example: the exported "from_roman(std::string_view)" function uses the utility function from_roman(char).
// from_roman.cpp "module implementation file" only contains the implementation of the exported
// "from_roman(std::string_view)", therefore we keep the implementation file short.
// the utility function is implemented in "roman-internals", a "module partition implementation" file
// also called "module internal implementation" file because it contains the "internals" of the module,
// i.e the unexported utility/helper functions, that exist only to facilitate the functionality of the main functions.
// Snow now we have a single module, with its own implementation file for the "main" exported functions
// and another seperate implementation file for the utlity functions.
// In MSVC, the partition file "roman-internals.cpp" must be compiled with the "interface" flag:
// right click on "roman-interals.cpp"->properties->advanced->compile as-> c++ module internal partitions
// The files with .ixx extension by default are compiled as Module (/interface).
// *****************************************************************************************
// 3. we could also use two sub modules: one containing main functions and the other containing utility functions
// the one containing the "main" functions, then imports the module with utility functions.
// the downside is that instead of a single module file, we now have two module files.

// Module implementation partitions
#include <iostream>
import <string>;
import roman;

int main()
{
	std::cout << "1234 in Roman numerals is " << to_roman(1234) << std::endl;
	std::cout << "MMXX in Arabic numerals is " << from_roman("MMXX") << std::endl;
}