/*
tl;dr tl;dr

1. Static-linking: the .lib contains all the definitions (all the data) of the library,
the .lib (library function definitions) is loaded (linked) with our code (.obj) at linking stage
to produce a single .exe that isn't dependant on other files.

2. Dynamic-linking:
the .lib contains pointers to the definitions that exist inside a .dll.
the .lib pointers are loaded (linked) to the .exe  with our code at linking stage , and at runtime
the .exe queries the .dll to load the function definitions.

***********************************************************************************
so in static linking the definitions are known at linking time, and in dynamic linking
the definitions are known (loaded) at run time.
***********************************************************************************



tl;dr
library - contains compiled code indended to be reused in development. libraries can come
in 2 forms:
    1 .lib which contains all the  machine code to use the library (definitions).
    used for creating a statically-linked .exe where the .lib is linked up (embedded) into the .exe 
    together with our .obj machine code (containing forward declarations to the functions of the library
    to satisfy the compiler) - library Definitions are loaded into the exe at linking time).
    the .lib and .obj's can be deleted after the .exe is produced.

    2. .lib and .dll combination where the .lib file contains pointers to the definitions that
    exist inside the .dll. Used to create a dynamically-linked .exe, where only the .lib
    is linked (embedded) into the .exe with our .obj machine code (which contains
    forward declarations to the functions of the library to satisfy the compiler). 
    This results in smaller .exe compared to statically-linked method where the .lib contains all definitions.
    The execution of one of the library functions requires querying the machine code of the .lib (embedded in our .exe)
    to know where is the definition of the function inside the .dll
    - library definitions our externally loaded (queried) at runtime by the .exe).
    the .obj and .lib can be deleted but the .dll must exist.





the long version.

.lib (Static-linked library) 
    library of compiled functions that are statically linked to our program
    during linking phase by the linker (explained below). lib files are created with the help of .obj files
    which are the result of compiling the cpp files that hold the code that is supposed to be reused:
    our_libarary.cpp -> the compiler produces our_library .s (assembly),
    the assembler produces "our_library.obj" (machine code) -> create "our_library.lib" via "our_library.obj" ->
    "our_library.lib" can be reused (.obj can be erased).

    to use a function from .lib, for example foo():
    1. we need to forward declare foo() in our compilation unit (.cpp) to satisfy the compiler
    (informing the compiler that the definition exists somewhere).
    2. after we compiled our cpp, to create the .exe the linker will link the forward declaration of foo()
    with foo()'s definition inside .lib and foo() will work (Assuming we informed the linker that the .lib exists)
    3. now we have a STATICALLY LINKED the .lib LIBRARY INTO OUR EXE- the .lib has been completely combined with our work during
    linking stage (providing definitions to forward declartions) to create a single binary (exe)
    that holds both our program's machine code and the .lib machine code.

    Notes:
        1. a change in the functions of the library (modified .lib) will require us to relink our code
        with the modified .lib and produce a new .exe.
        2. We can delete the .lib file (and .obj's), the .exe can function on its own because it contains all
        the machine code necessary to work by itself, it is self contained.
        3. each program that is statically-linked with a .lib has all of its code. e.g: a.exe & b.exe are linked with c.lib
        then a.exe and b.exe both contain the complete code of c.lib



.dll (dynamic-linked library) - library of compiled functions that are linked to our program.

    dll's are also created with the help of .obj's as explained above. dynamic linking also
    creates a .lib file, but as opposed to static linking where .lib contains full definitions, here
    .lib only contains symbols (pointers?) that point to the addresses of functions in the .dll.
    which houses all the machine code of the library, therefore the .lib in the context of dynamic-linked
    library is also called the import library provided by the maker of the dll.

    to use a function from .lib, for example foo():
    1. we need to forward declare foo() in our compilation unit (.cpp) to satisfy the compiler
    (informing the compiler that the definition exists somewhere).
    2. after we compiled our cpp, to create the .exe the linker will search the .lib
    for a pointer to the machine code of the foo() inside the .dll
    and foo() will work (Assuming we informed the linker that the .lib and .dll exists)
    3. now we have a DYNAMICALLY LINKED the .dll library WITH our exe- the .lib has been completely combined with our work during
    linking stage, providing the locations to the definitions inside the .dll. when the created .exe
    wants to execute foo() it query's the embedded the .lib for the location of foo()'s definition inside the
    external .dll and the function then executes.
    4. statically linked binaries are easier to distribute (single .exe)

    Notes:
        1. The size of the .lib is much smaller then the statically-linked .lib that contained all the machine code.
        here the .lib contains only pointers to the functions inside the .dll

        2. Because the linker links our code with the .lib, the outputted .exe is also smaller then the statically-linked
        .exe, because the .lib only contains pointers to functions inside the .dll instead of full machine code.

        3. After the linking is complete we can delete the .lib file (and .obj), because it was linked up (embedded) into
        the .exe together with our code into a single binary (same as in static-linking). the difference
        is that the actual library code is in the external .dll file, while .lib only
        has pointers to the .dlls. Deleting the .dll will cause an error if we would like to 
        execute a function from the library (.exe will look for foo()'s
        definition with the help of the pointers in the embedded .lib inside the .exe, but the
        definition will be gone because the .dll is deleted).

        4. if we want to make a change (update/bugfix) to the library, we only need to provide a new dll to the
        customer. The customer doesn't need to recompile or relink his .exe, because the already-embedded (linked)
        .lib already points to the correct foo() function inside the .dll. as long as the signatures
        are the same, the old .exe will know to find foo() inside the new .dll and execute the modified
        definition of foo(). This is in contrast to statically-linked libraries where every change in the library
        (expressed as a change to the .lib) requires the customer to relink his code with .lib
        (recompiling isn't necessary unless the customer's code is changed) and produce a new .exe.

        5. Many processes can use a single DLL simultaneously - this saves memory! in contrast
        a static link library has the library code linked into its exe image,  if a.exe & b.exe are linked with c.lib
        then a.exe and b.exe both contain the complete code of c.lib
    

        6. an executable communicates with a dll in one of 2 ways:

            a.implicit linking (most popular due to ease of creation) -
            operating system loads called functions from the DLL at the same time when it loads
            the .exe. The end effect is similar to statically linked libraries where the definitions
            of the library are known as soon as the program runs, as if they are embedded in .exe.
            therefore implicit linking is also called as static load or load-time dynamic linking

            b. explicit linking (less popular due to complexity of creation) - 
            where the operating system loads the DLL on demand at runtime.
            For example, application extensions that add new functionality to your 
            app without rebuilding or redeploying it (app gets updated at run time with new functionality
            from a new .dll).
            Explicit linking is sometimes referred to as dynamic load or run-time dynamic linking.
      




Static Linking (project 49a_static_math_library and 49b_math_library_client)
*make sure that in project properties, configurations are set to "all configurations" and 
platform is set to "all platforms.

    Creating the static library (project 49a_static_math_library):
        1. right click on the project - > properties -> general -> choose configuration type: static library .lib
        2. create a header file with declarations and a cpp file for implementation.
        3. build the project. this will output a .lib which contains all the definitions of the functions
        (taken from .cpp).

    Using the library (49b_math_library_client) - client needs only .lib and the originial headers from the library:

        1. Satisfy the compiler: first we must use include the header file(s) of the library - they include forward
        declarations to the library's functions, this satisfies the compiler, telling him that the definitions
        exist somewhere (in the .lib which the linker will link up to in the next step).
        right click on 49b_math_library_client -> properties -> c/c++ -> general- > additional include directores ->
        choose the path of the header from 49a_static_math_library.
        in the .cpp of the client (49b_math_library_client, which intends to use the library) perform an #include "MathLibrary.h" 
        (which is taken from the provided path we've just added)

        2. Satisfy the linker: add the path of the .lib (contains the definitions:  right click on 49b_math_library_client -> properties
        linker -> general -> additional library directories (definitions satisfy linker).

        3. Lastly, we need to inform VS about the actual name of the .lib (in 2. we only declared the path).
        add dependency to additional dependencies (the name of the lib file so that vstudio would know we're dependant on the library)
        right click on 49b_math_library_client -> properties
        linker -> input -> additional dependencies -> add the name of the .lib (49a_static_math_library.lib);
        Note: the seperation between step 2 and 3 allows us to have a list of library directories (step 2)
        and a seperate clean list of actual .lib names in additional dependencies (step 3).

        4. Use functions from the libray, simply by invoking them in code. build the .exe - it works
        by itself without any dependencies.

    Alternatives:
        1. we can discard step 2 and provide the name of .lib + its path in step 3 ("additional dependencies").
        2. IF THE LIBRARY ORIGINATES FROM A PROJECT WITHIN OUR SOLUTION: we can perform the steps above,
        but we can also utilize this fact by adding the library project as a reference to the client:
        right click on 49b_math_library_client -> Add reference -> choose the library project
        (49a_static_math_library). This has 2 effects:
            a. auto inference of .lib:
                once we satisfied the compiler in step 1 by including the header, and satisfied the linker 
                by declaring the path of additional library directories (step 2), step 3 isn't necessary -
                we don't need to explicitly tell VS the name of the .lib. VS infers which .lib to use
                via the reference we added, and the functions the client uses.
                The advantage is that if we modify the name of the .lib, the project will continue to work,
                because the "reference technique" can infer which .lib to use automatically, as opposed
                to explicitly naming the .lib as performed in step 3.
            b. adding the library as a reference casuses an automatic build of the library when we execute
            the build of our project - this ensures consistency of both of them - working always on the most
            updated code.


Dynamic Linking (project 49c_dynamic_math_library and 49d_math_library_client) via implicit linking

    Creating the dyanmic library (project 49a_static_math_library) - The method shown here is 
    dynamic linked library via implicit linking (when the .exe is loaded, the .dll is also loaded
    and linked with the .exe). (dynamic linked library via dynamic linking is more complex to set up and less popular):
    1. right click on the project - > properties -> general -> choose configuration type: dynamic library .lib
    2. create a header file with declarations and a cpp file for implementation.
    in the header file(s) we must include the following statements (demonstrated in 49c_dynamic_math_library):

    #ifdef MATHLIBRARY_EXPORTS __declspec(dllexport)
    #define MATHLIBRARY_API __declspec(dllexport)
    #else
    #define MATHLIBRARY_API __declspec(dllimport)
    #endif

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
        compatability with c programs that want to use the .dll

    2. the preprocessor code block explained:
    
        a. In the library project :
        should ensure that "MATHLIBRARY_EXPORTS" is defined (in the project properties, under C/C++ ->
        Preprocessor -> Preprocessor Definitions). if "MATHLIBRARY_EXPORTS" is defined the properties,
        the line: "#ifdef MATHLIBRARY_EXPORTS __declspec(dllexport)" will cause the preprocessor to replace 
        "MATHLIBRARY_EXPORTS" with "__declspec(dllexport)". functions (ONLY IN THE HEADER!) marked with __declspec(dllexport) will be 
        exported to the dll.

        b. the client doesn't need to define "MATHLIBRARY_EXPORTS" in the project properties,
        but he does need to define "MATHLIBRARY_API" in the project properties.
        This will cause the declerations in the header file that the client uses to change their
        signature at preprocessing, in other words, MATHLIBRARY_API will be equal to __declspec(dllimport).
        the linker will know that functions marked with __declspec(dllimport) need to be linked up with the .dll



Using the dynamic library (49b_math_library_client):
    the same steps as using the static library, with the following differences:
        a. we still need to include the header to satisfy the compiler (step 1). in
        step 2 the .lib we include in additional libraries to satisfy the compiler, doesn't include
        full definitions like in static linking but rather addresses to the dll.

        b. the .dll must be in the same folder of the .exe (we only need both of them to execute
        the program). we can set search locations for dll's but the location of 
        the .exe is automatically a search loation for the dll.
    



*/


https://docs.microsoft.com/en-us/cpp/build/dlls-in-visual-cpp?view=msvc-160
https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-160
_declspec(dllexport) void test()
{
}

int main()
{

} 