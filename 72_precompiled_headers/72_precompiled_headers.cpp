/*
precompiled headers (.pch files) - every time we include header files in one of the cpp
, for exahmple #include<vector>, the contents of vector is copy-pasted into our file via the preprocessor
parsed, linked and compiled each time
we compile our project - this takes time! this problem is aggrevated when we use include
in multiple translation units. instead of recompiling the header file each time, we can precompile it - 
this quickens compilation times. This is recommended as long as the stuff in the precompiled
header doesn't frequently change.
Using one .pch means that we don't need to include many different libraries. The downside
is that it is not clear what exactly libraries we use (single #include "x.pch" instead
of many different explicit includes)

HOW TO (in visual studio):
1. create a cpp file that includes the header file
2. select that cpp file -> properties -> precompiled headers -> precompiled header -> create
(make sure that the precompiled header file field says pch.h
3. select the entire property for the entire project->
-> c/c++ -> precompiled header tab -> precompiled header (use) -> precompiled header file -> pch.h
since we select properties of the project, it will apply to all files (all files will use the precompiled header)
4.the .pch is generated in the files
*/
#include <iostream>
#include "pch.h" 
int main()
{
    std::cout << "Hello World!\n" ;
}
