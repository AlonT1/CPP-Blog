#include <iostream>
/*
iostream is included in this cpp and in log.cpp - but the iostream header includes #ifndef #define guard against include duplications!
*/


#include "log.h" // enables the usage of log.cpp. by inserting the declerations that exist in log.h we 
//tell the linker that somewhere in the project, the definitions of these functions exist. the linker will eventually find them in the log.cpp translation unit and link this translation unit up with it
//without it, the compiler won't know that these functions exist in log.cpp. this enables the reuse of log.cpp's function in other files.
//Why not just declare functions and define them inside the header file? because it will cause a redifinition of data across the project!
//each translation unit will contain decleration and definition of a function, and during linkage
//this can cause duplicates of data. therefore we declare once, define once, and include the header in other cpp's
//a promise to the linker that some cpp file holds the definition of the data in a single location with a single defintion.


/*
Form 1 - #include < xxx >
First, looks for the presence of header file in the current directory. If not found, then it searches in the preconfigured list of standard system directories.
mainly used for external files not written by the programmer itself

Form 2 - #include "xxx"
This looks for the presence of header file in the current directory from where directive is invoked.
mainly used for external files  written by the programmer itself
*/

int main()
{
    log("hello world");
    init_log();
}
