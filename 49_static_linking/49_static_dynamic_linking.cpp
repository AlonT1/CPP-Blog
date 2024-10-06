/*
library - contains compiled code indended to be reused in development.
libraries come into two forms: static and dynamic libraries.

*statically link - linking stage at compile time.

********************************Static Library************************************
static library - we statically link (embed) function definitions (.lib) together
with our compiled code (.obj) to produce a standalone .exe

static library parts:
1 .lib - contains all function definitions of the library.
2. header file(s) - library.h, included into our code to use the library.
contains forward declarations to the functions in .lib.

Behind the scenes of the integration of a library with our code/program:
1. .obj - our compiled (machine) code, generated after compile stage.
.obj contains compiled library function calls from our code, for example:
library_func(), enabled by the "include" of the forward declarations in 
library.h such as "void library_func();". These forward declarations satisfy
the compiler at compilation stage, promising that the function definitions do
exist and prepared to be used at linking stage.
2. .exe - During linking stage, the linker links .obj and the .lib. 
This causes the library function calls we made in our code (compiled into .obj) 
to be linked up with their body, a.k.a definitions (that exists in .lib (library we use)). 
The linker then outputs a .exe.

Creation (example in project 49a_static_math_library):
1. right click on the project - > properties -> general -> choose configuration type: static library .lib
2. create a header file with declarations and a cpp file for implementation.
3. build the project. this will output a .lib which contains all the definitions of the functions
(taken from .cpp).

Usage - client needs only .lib and the original headers from the library (example in 49b_math_library_client):
1. Include the header to the project (contains forward declarations, satisfies the compiler):
right click on project -> properties -> c/c++ -> general- > additional include directores ->
choose the path of the header.
2. Add the .lib file to the project (contains definitions, satisfies the linker):
    a. include .lib:
    right click on the project -> properties -> linker -> general -> 
    additional library directories (definitions satisfy linker) -> add .lib path
    b. add the .lib as a dependancy:
    right click on the project -> properties -> linker -> input -> 
    additional dependencies -> add the name of the .lib.
3. Include the header into some cpp.
4. Invoke the functions!

Usage alternatives:
1. we can discard step 2 and provide the name of .lib + its path in step 3 ("additional dependencies").
2. IF THE LIBRARY ORIGINATES FROM A PROJECT WITHIN OUR SOLUTION: we can perform the steps above,
but we can also utilize this fact by adding the library project as a reference to the client:
right click on client project -> Add reference -> choose the library project. This has 2 effects:
a. auto inference of .lib:
we don't need to explicitly tell VS the name of the .lib. VS infers which .lib to use
via the reference we added, and the functions the client uses. if we modify the name of the 
.lib, the project will continue to work, because the "reference technique" can 
infer which .lib to use automatically, as opposed to explicitly naming the .lib as performed in step 3.
b. adding the library as a reference casuses an automatic build of the library when we execute
the build of our project - this ensures consistency of both of them - working always on the most
updated code.

Notes:
1. after the .exe is created, the .lib is not needed (the function definitions it contains are
fully embedded into our .exe).
2. a change in the functions of the library (modification to .lib) will require
us to relink our code (.obj) with the modified .lib and produce a new .exe.
3. each program that is statically-linked with a .lib has all of its code. e.g: 
a.exe & b.exe are linked with c.lib then a.exe and b.exe both contain the complete code of c.lib


********************************Dynamic Library************************************
Dynamic library - we statically link (embed) function pointers (.lib) with our 
compiled code (.obj) to produce an .exe which uses library functions (from .dll)
at ****RUN TIME***, by invoking a function call,and reaching to the function
body in the external .dll through the function pointer that points to it.

There are two types of dynamic libraries:
a. dynamic library via implicit linking: when the .exe is loaded (run-time),
the .dll is also loaded and linked up with the .exe
b. dynamic library via dynamic linking: functions from the .dll are linked
dynamically only when they are actually invoked at run-time.

Dynamic library parts:
1. .dll - contains all function definitions of the library.
2. .lib - contains pointers (symbols) to the definitions that exist inside a .dll.
3. header file - library.h containing forward declarations to the functions in .lib.

Integration with our code/program:
1. .obj - same as in the .obj in static library above
2. .exe - At linking stage: our library function calls (in the compiled. obj)
---are linked up with---> function pointers (.lib).
the .exe is produced, and at run time when the functions are invoked, the code
reaches to the function pointers, causing the program to look for the definition in the .dll

Creation (we create a dynamic library via implicit linking. dynamic library via dynamic linking
is more complex) (example in project 49c_dynamic_math_library):
1. right click on the project - > properties -> general -> choose configuration type: dynamic library .lib
2. create a header file with declarations and a cpp file for implementation.
in the header file, each function must be preceded with "extern" C
and the header must include the following statements:

#ifdef LIBRARYNAME_EXPORTS __declspec(dllexport) 
#define LIBRARYNAME_API __declspec(dllexport)
#else
#define LIBRARYNAME_API __declspec(dllimport)
#endif
**(LIBRARYNAME_EXPORTS macro is defined in project properties)

explanation:
1. the terms:
    a. __declspec(dllexport) and __declspec(dllimport):
        storage-class attributes, Microsoft-specific extensions to the C and C++ languages.
        used for exporting and importing functions, data, and objects to or from a DLL.
        __declspec(dllexport) and __declspec(dllimport) are THE CONVENTION and recommended to
        be used (Only in header files!). dll's may or may not function without these storage classes.

    b. extern "C": extern "C" should be added to functions (although it seems
    not mandatory, the library works without it). makes a function-name in C++ have
    C linkage (compiler does not mangle the name). The non-mangling of names ensures
    compatability with c programs that want to use the .dll. name mangling enables
    function overloading (see 9_functions_overloading), which isn't available in c.

2. the preprocessor statements explained:
    a. In the library project :
    should ensure that "LIBRARYNAME_EXPORTS" is defined (in the project properties, under C/C++ ->
    Preprocessor -> Preprocessor Definitions). if "MATHLIBRARY_EXPORTS" is defined the properties,
    the line: "#ifdef MATHLIBRARY_EXPORTS __declspec(dllexport)" will cause the preprocessor to replace
    "MATHLIBRARY_EXPORTS" with "__declspec(dllexport)".
    functions (ONLY IN THE HEADER!) marked with __declspec(dllexport) will be exported to the dll.

    b. the client doesn't need to define "MATHLIBRARY_EXPORTS" in the project properties,
    but he does need to define "MATHLIBRARY_API" in the project properties.
    This will cause the declerations in the header file that the client uses to change their
    signature at preprocessing, in other words, MATHLIBRARY_API will be equal to __declspec(dllimport).
    the linker will know that functions marked with __declspec(dllimport) need to be linked up with the .dll

3. build the project. this will output a .lib (containing function pointers) and .dll 
containing function definitions.


Usage ( example in 49d_math_library_client):
exactly as in the "usage" in static_library, in addition to ensuring that the
.dll exists in the same location of the .exe (which is automatically a search
location for the dll.) - the function pointers from the .lib that were linked
(embedded) with our .obj into a standalone .exe are pointing to the function
definitions inside the .dll


Notes:
1. After the .exe is produced, the library .lib can be discarded - it is fully
embedded into our .exe, together with our compiled code (.obj)
2. We cannot delete .dll (leave it beside the .exe). It contains the actual function definitions.
3. because the .lib contains only function pointers, it is much smaller than the .lib
of static libraries that contain full function definitions.
4. if we want to make a change (update/bugfix) to the library, we only need to provide a new dll to the
customer. The customer doesn't need to recompile or relink his .exe, because the already-embedded (linked)
.lib already points to the correct foo() function inside the .dll. as long as the signatures
are the same, the old .exe will know to find foo() inside the new .dll and execute the modified
definition of foo(). This is in contrast to statically-linked libraries where every change in the library
(expressed as a change to the .lib) requires the customer to relink his code with .lib
(recompiling isn't necessary unless the customer's code is changed) and produce a new .exe.
5. Many processes can use a single DLL simultaneously - this saves memory! in contrast
to a static library where the embedded fully-defined library code (.lib) 
is linked into its exe image causing each program (a.exe, b.exe....) to have a full copy of the library.
with dyamic linking the embededd .lib into our .exe only contains function pointers, therefore a.exe, b.exe...
use these pointers to retrieve function definitions from a single .dll, causing a reduction
in the size of a.exe, b.exe
6. Modularity - instead of a single big .exe that we can experience with static libraries,
with dynamic library we can have a relatively small .exe with many smaller .dlls


https://docs.microsoft.com/en-us/cpp/build/dlls-in-visual-cpp?view=msvc-160
https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-160
*/


int main()
{

} 