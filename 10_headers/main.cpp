#include <iostream>
/*


Header Inclusion (in .h/.c files):
1. #include <iostream>: Includes system headers from compiler-defined paths.
2. #include "log.h": Includes user-defined headers from the current or specified project paths
("include additional headers" in visual studio project settings.)

Header Guard Mechanisms (both equivalent), need to be written in the header files (not in .c):
1. #pragma once: Ensures the header is included only once per translation unit.
2. #ifndef HEADERFILE_H, #define HEADERFILE_H ...content... #endif
The gaurds helps to prevent the existence of multiple same forward declaration in a single translation unit (.c),
violating ODR(one definition rule) for functions (classes/structs are exempt from ODR).


The header enables the usage of log.cpp. by inserting the declerations that exist in log.h we 
tell the linker that somewhere in the project, in some .c, the definitions of these functions exist.
the linker will find them in the log.cpp translation unit and link the included forward declerations
here from "log.h" with the definition from "log.cpp". Think of the fowrard declerations as pointers to the defs.

Why not just declare functions and define them inside the header file? because even a single inlcude will cause
definition duplication (in the original .h, and in the included file), and the linker will encouter the duplicated
data, violating ODR.
*/
#include "log.h"

int main()
{
    log("hello world");
    init_log();
}

/*
What headers should contain:
Header guards, function forward declerations, structs+classes (functions are only fwd declerations), enums, templates.
Should player.c include player.h - not mandatory (the definition in .c will be linked up with fwd decl in .h),
but is recommended (https://stackoverflow.com/questions/1804486/should-i-use-include-in-headers):
header should have all the necessary "includes", and optimally player.c will need to include only player.h

for headers/cpp in the context of classes, see 22b_class_header
*/
