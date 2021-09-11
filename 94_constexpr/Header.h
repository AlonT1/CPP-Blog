#pragma once

/******EXTERN CONST INLINE CONSTEXPR DEMISTIFIED!*****/

//x is a global variable (by default - storage: static (execution-termination), linkage: external)
//when the header is included in other files, and the #include is in the global namesapce,
//x will be included in the global namespace. this will cause a linking error (multiple definitions of x due to external linkage)

//int x = 5;



//a const global has INTERNAL LINKAGE by default. including this header in multiple files
//will not violate ODR because q is internally linked in each translation unit.
//the problem is that we waste memory by defining z1 in multiple files!
const int q = 5;

//Solution (Not the best):
//non-const global int y has EXTERNAL LINKAGE by default. The extern keyword before a global varaible tells the linker
//to look for the definition in another translation unit.
//The extern must be applied in all files except the one where the variable is defined.
//USAGE: extern int y is only declared in this header file, and defined once in some "source.cpp" file (without including the header)
//other files who want to use y can include the header, or just include the statement "extern int y".
//all extern y's will refer to the same y defined in the other.cpp - done via the linker.
//This solves the problem with global int x which is defined in all flies including it, which violates ODR - one definition rule.
//here y is defined only in Source.cpp, all extern int y refer to it (in the linking stage)
extern int y;


//NOTE: extern on const-global variables applies external linkage to the variable.
//because it is const, it must be initialized with a value at decleration!
//USAGE: "extern const int z=42" can reside (or included) in a.cpp and b.cpp can refer to it by decleration-only "extern const int z"
//“this is a forward declaration for an external variable that is defined somewhere else” (it was defined where it was initialized due to cosnt)
//the problem with "extern const int z=42" inside headers is that headers are usually included in multiple cpp's
//and this violates ODR, because z needs to be defined inside the header (it is a const).
//NOTE: even though z is const, it doesn't promise that it will be a compile-time constant (see 94_constexpr.cpp)
//therefore external linkage (which occurs after compilation) can also work for z

//extern const int z = 42;


//BUT THERE'S A PROBLEM both in y and z: In order for variables to be usable in compile-time contexts (e.g: for array size) (optimizations!), 
//the compiler has to see the y' and z's definition in all translation units during compilation and not during linking
//(preprocessor->compiler(generates assembly .s)->assembler (genereates object files .o)->linker(genereates exe))
//in this solution, the compiler only sees the forward decleration of y and z in some tranlstion units (from the #include, thanks to the preprocessor)


//what about constexpr?????? - computation of variable, when possible, AT COMPILE-TIME!! (as opposed to to const which can
//also be a compile time and run-time const (see 94_constexpr.cpp)
extern constexpr int u = 4; //undefined by the standard (must be initialized as a const value) EVEN THOUGH MARKED AS EXTERN constexpr will remain with internal linkage!
//this will cause u to be uniquely defined in each translation unit without violating ODR - basically extern is ignored!
//so all those who include "u" will get a copy of "u"
//AS OPPOSED TO THE VARIABLE z, constexpr cannot utilize linkage since it's computed at compile-time variable
//the compiler needs to know the value of a constexpr variable at compile-time (before linkage) and forward-declerations
//doesn't provide them, like we see with the regular const "z"


//SO HOW CAN WE UTILIZE THE BENEFIT OF EXTERN: single definition - single memory usage, with multiple files referring to it, 
//while also using the optimizations of compile-time constants (previous solutions all happen at linkage)?
//SOLUTION - inline variables (c++17)!
//inline - "multiple definitions are permitted". The original intent of the inline keyword was to serve as an indicator to the optimizer
//that inline substitution of a function is preferred over function call (avoiding overhead of stack allocation, passing arguments, retrieving results).
//Therefore, the body of a function delcared inline is copied and replaces the function call, thus we essentially
//redifine the function each call
//downside - larger executable as the code for the function is repeated "inline" multiple times.

//this meaning was extended to variables - we can have multiple definitions of a variable (via #include)
//THE LINKER WILL CONSOLIDATE ALL INLINE DEFINTIONS OF A VARIABLE INTO A SINGLE DEFINTION (MEETING ODR)
//WHILE BEING CONSTEXPR (COMPILE-TIME CONST WITH OPTIMIZIATIONS!), although t has internal linkage because of constexpr!
//from cpp reference: inline variable has the same address in every translation unit.
//
inline constexpr int t = 5;



/*
tl;dr:
Note: the seperation between header and source is artificial because the contents of the header is included in the source.
extern can mean:
1. when dealing with non-const vars:
a. extern at decleration: "give this var external linkage". the declerations are called forward declerations
(satisfy the compiler) usually found in header files which are to be included in other files that will use the var. 
the linker will later link the decleration together with the defintion of the var
b. at definition: no extern should be used at definition. definition of extern var should be made once in some cpp file.
the defining cpp doesn't even need to include the header containing the decleration
Note: an extern decleration can be found in a.cpp and definition in b.cpp, linked by the linker (without headers)
equivalent to declaring extern var in c.h and defining it in d.cpp
c. extern at decleration + definition:
i. in a header: doesn't make sense because headers are usually included in other files
which will cause the var to defined multiple times (ODR Violation)
ii. in a cpp file: 

2. when dealing with const vars
b. extern at definition: 
"includes" - translation units that include the header
within a header file (assume variables are global):
1. int a = 5; // external linkage (by default) - "includes" cause ODR violation. extern keyword is ignored if used.
2. const int b = 5 //const requires initi. & applies internal linkage - "includes" - b defined per file (memory inefficient), ODR unviolated, compile-time const (optimized)
3. extern int c; // c defined+initialized in a.cpp (without header include), "includes" - refer to the defined c thanks to extern by linker
4. extern const int d = 4 //const requires init. extern - external linkage - "includes" - into a single a.cpp (to avoid ODR violation due to external linkage),
//other trans. units refer to it by extern (by forward decleration only - extern const int d;) during linkage therefore const isn't at compile-time
5. constexpr int e = 5; //same as var b - constexpr is more readable, saying - THIS IS A COMPILE-TIME CONST as opposed to "const" which can also be computed after compile-time
//at linkage (such as d) or runtime (assigning keyboard input into a const var - cin<<age; const int x = age)
6. extern constexpr int f = 5 // constexpr has always internal linkage, even when extern has used. (same as e or b)
7. inline constexpr int g = 4; //THE KING - inline allows multiple definitions of g (unviolated ODR in "includes") + compile-time constant optimizations!
//although constexpr has internal linkage, inline consolidates all definitions in "includes" into a single definition (share the same address)!

extern and inline together? doesn't make sense.. inline - allow multiple defintions (where all defintions are the same
to allow consolidation into one defintion) extern - tells the linker to look for the definition in another translation unit.
so we promise via inline to put the same definition everywhere and simultaneously tell the compiler via extern to look for the definition
in another translation uni?!!?


*/