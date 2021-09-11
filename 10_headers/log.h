//#pragma once
/*
The guarantee file - as long as the implementation exists somewhere in the project,
it is sufficient to include these forward declerations - the linker will use them 
 and will find the implementation in the appropriate translation unit
 
 headers should include:
 1. forward declerations:
 e.g: extern int x;   void func();

 2. inline functions / variables (consolidated into a single definition:
 e.g: inline constexpr int x = 5;    inline void func(){...}

 3. class / structs - forward declerations of class methods and declaration of 
 class variables. (their definition will lie in a .
 
 
 )

inline means:
1. It tells the compiler that the function code can be expanded where the function
is called, instead of effectively being called.
2. It tells the compiler that the function definition can be repeated (because of point 3).
3. the compiler may consolidate all inline definitions into a single definition in memory.
4. applies internal linkage.

 */


#ifndef _LOG_H // instead of pragma once
#define _LOG_H

void log(const char* msg); // decleration - defintions are in the cpp
void init_log();


#endif

/* compilation stages
1. preprocessing of # commands
2. compilation into assembly code (.s)
3. the assembler produces machine code (.obj)
4. the linker links (obj's)
5. we get an exe

*/