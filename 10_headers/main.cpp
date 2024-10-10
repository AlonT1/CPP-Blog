#include <iostream>
/*
iostream is included in this cpp and in log.cpp - but the iostream header 
includes #ifndef #define guard against include duplications!
for example avoiding:
void log(const char* msg); to appear in this file twice, due to 2 includes of log.h.
which will cause the linker to also link up the definitions in log.cpp, in summary
we will have 2 fully defined "log" functions here, if we don't use preprocessor
directives in the header file - either "pragma once" or "ifndef, define, endif"

// header.h
#pragma once
// Code placed here is included only once per translation unit.

or:

// headerfile.h
#ifndef HEADERFILE_H
#define HEADERFILE_H

// contents of headerfile.h (included only once per translation unit).

#endif // HEADERFILE_H

*/


/*
include <> searches only the compiler predefined paths and the paths specified in the "include additional headers"
in visual studio project settings.

*/

#include "log.h" // enables the usage of log.cpp. by inserting the declerations that exist in log.h we 
//tell the linker that somewhere in the project, the definitions of these functions exist.
//the linker will eventually find them in the log.cpp translation unit and link this translation unit up with it
//without it, the compiler won't know that these functions exist in log.cpp.
//this enables the reuse of log.cpp's function in other files.
//Why not just declare functions and define them inside the header file? 
//because it will cause a redifinition of data across the project - in cpp data types must be defined ONCE!
//each translation unit (cpp file) will contain decleration and definition of a function, and during linkage
//this can cause duplicates of data. therefore we declare once, define once, and include the header in other cpp's
//a promise to the linker that some cpp file holds the definition of the data in a single location with a single defintion.


/*
Form 1 - #include < xxx >
The preprocessor searches for the header file in the standard system directories
(e.g., directories where C++ standard library headers are stored) and
in "include additional headers" in visual studio project settings.
mainly used for external files not written by the programmer itself

Form 2 - #include "xxx"
mainly used for external files  written by the programmer itself, such as "log.h".
include " " first searches the current folder (where the header is)
and then reverts to search in the same way as include<>
*/

int main()
{
    log("hello world");
    init_log();
}
