/*
macros - using preprocessor of cpp to macrofy (automate) operations
the preprocessor is one of the first thing that happen during compilation - every 
precprocessor directive is evaluated and then the code is passed to the compiler for compilation
this is a text editing stage - we control the text that is fed to the compiler (advanced find and replace with parameters)
in contrast to templates which happen during compilation, preprocesser occurs pre compilation
*/

#include <iostream>
#define PRINT std::cout << "Hello World!\n";//preprocessor directive #. PRINT will be replaced by PRINT std::cout << "Hello World!\n"

//logging for debugging purposes
//in project preferences, Linker, preprocessor definitions, _DEBUG is for debug configuration and NDEBUG is for release configuration
//We can use them to ensure that the logging (prints)  only occurs in debug mode, and not in release mode - saving performance
#ifdef _DEBUG //alternative #if _DEBUG == 1 better because we can #define _DEBUG 0  (controling the value in this way)
//need to modify _debug to _debug = 1 in preferences (preprocessor definitions in linker
#define LOG(X) std::cout << X << std::endl // function like directive. if debug LOG exists
#else// else if release mode, log is empty statement // alternative - #elif defined(RELEASED) RELEASED in preprocessor directive
#define LOG(X)
#endif 


//deactivating code!
#if 0
//code
#endif

// "\" is for a new line
#define MAIN int main()\
{\
}

int main()
{
    PRINT; //the statement in the define is pasted here during preprocessing, the compiler won't see a difference.
    //we only change the text being brought to the compiler

    std::cout << "hello" << std::endl;//equals to
    LOG("hello");

}