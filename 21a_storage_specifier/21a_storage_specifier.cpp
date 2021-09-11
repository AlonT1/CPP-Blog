/*
storage specifier controls:
1. storage duration - lifetime of the object (execution - termination / new - delete/ block start - block end)
2. the type of linkage (no,internal,external)
3.  storage in memory (stack, static)
* but not the scope - the visibility of the identifier in the project (block, file) (this is determined by the block {} they are inside
or whether they are in the global namespace)
Note: Storage duration vs scope: an identifier can have a storage duration from lifetime to execution, but only scoped to a block

The Storage specifiers:

1. automatic - default for all local variables (auto keyword is implicit).
Storage duration (lifetime): block start - block end - the storage is claimed automatically at the end of scope.
Linkage: no linkage (The name can be referred to only from the scope it is in). auto & register can't be declared globaly (file scope)
Scope: block visibility
Storage in memory: stack

2. static  - 
Storage duration: execution-termination (single decleration+initialization+definition
Linkage: internal linkage (name can be referred to from all scopes in the current translation unit.)
Scope: block / file  (depends on decleration)
Sotrage in memory: static storage
Notes:
1. global variables have default static storage duration (execution - termination), and external linkage by default ( "extern"  keyword allows referring to it- 
(global var "int t=5" in x.cpp can be referred by declaring "extern int t" in y.cpp)
2. declaring global variables as static, modifies its linkage to internal
3. functions declared at file scope are global with external linkage (unless declared as static - causes internal linkage)
4. Internal vs no linkage:

static int i; //internal linkage via static
void func() 
{
    int n = 5; //no linkage
    extern int i; // refers to the int i above, internally linked
}


3. dynamic  -
Storage duration: new - delete
Linkage: depends if declared static (internal linkage)/ at file scope and non-static (external linkage)
Scope: block / file  (depends on decleration)
storage in memory: heap storage
Notes:
1. global variables have default static storage duration, and external linkage by default (can be referred by "extern" -
(global var "int t" in x.cpp can be referred by declaring "extern int t" in y.cpp)
2. declaring global variables as static, modifies its linkage to internal
3. functions declared at file scope are global with external linkage (unless declared as static - causes internal linkage)

*const / constexpr vars have internal linkage by default, static keyword is ignored
*static functions are internally linked (without static - external link)
*/


#include <iostream>

static int y = 5; // static storage specifier  - scoped to this file, stored in static memory section, internal linkage via extern,
//static storage duration
extern int t; // referring to t in other.cpp
int x = 5; //global variable, static life time (execution-termination), on the stack, external linkage possible via extern, , global scope


int main()
{
    int x = 2; //automatic storage specifier - scoped to main and lifetime duration bound to main. stored in stack
    int* w = new int; //dynamic - allocated here, until delete. allocated in heap
    //t = 2;
    std::cout << t << std::endl; // prints 5, declared global int t = 5 and referred here by extern
}
