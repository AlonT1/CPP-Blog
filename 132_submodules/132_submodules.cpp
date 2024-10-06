// 131_modules a single roman.ixx module file. importing this module gives us acess to from_roman()
// and to_roman() functions.
// cpp also allows submoduling- instead of 1 roman.ixx module we can create 3 submodules:
// 1. roman_to.ixx module that houses and implements roman_to() function
// 2. roman_from.ixx module that houses and implements roman_from() function
// 3. roman.ixx module that imports and exports both roman_to.ixx and roman_from.ixx modules
//
// This makes the program more modular: instead of importing roman.ixx which gives us access
// to roman_from() and roman_to(), we can import only roman_to.ixx and gain access only to roman_to()
// function. same for roman_from module.
// Of course we can gain access to both function by importing roman.ixx because it imports roman_from.ixx
// and roman_to.ixx

// Important Note: "roman.to" module that we import below DOES NOT mean that we import a "to" module
// that is nested inside a "roman" module. cpp does not support module nesting, but
// it DOES allow using "." in module names (the only identifier in cpp that allows "." in its name).
// in other words, roman, roman.to and roman.from are completely separated and atomic modules.
// importing "roman.to" module has nothing to do with "roman" module,
// the name "roman.to" simply implies that the "roman" module is comprised of (imports) a another module
// called "to" represented by "roman.to.ixx".
// Thus it is said that cpp "simulates" submodules by using a "." in a modules name. Submodules, i.e
// nested modules do not exist, in practice all modules are equal.
#include <iostream>
import roman.to;


int main()
{
    std::cout << "Hello World!\n";
}