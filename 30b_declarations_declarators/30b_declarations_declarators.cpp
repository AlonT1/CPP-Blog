#include <iostream>

/*

https://stackoverflow.com/questions/13808932/what-are-declarations-and-declarators-and-how-are-their-types-interpreted-by-the
https://en.cppreference.com/w/cpp/language/declarations


Declaration: the introduction of names (identifiers) into a cpp program.

cpp declarations (also called simple declarations):
a statement that introduces (declares), creates(defines) and optionally initializes
an entity. Declarations come in one of the following forms:

1. decl-specifier-seq(opt) init-declarator-list(opt) ;
2. attribute-specifier-seq decl-specifier-seq(opt) init-declarator-list ;

combining the two we have 3 elements comprising a declaration:

1. (rarely used) attribute-specifier-seq: attributes are suggestions to the compiler, used
to warn the developer about something, surpress warnings, or ask the compiler to optimize.
for example:
    a. [[noreturn]] void my_exit_function(); 
    informs the compiler this function will not return.
    [[noreturn]] bool my_exit_function(); in this case returning a bool will be undefined

    b. [[deprecated]] void do_something(); entity is not inteded to be used, a warning
    will be displayed at compile time when code attempts to use the entity.


2. decl-specifier-seq (declaration specifiers): 
a sequence of one or more declaration specifiers separated by white space.
sepcifier: to include a specification, a detail.


all the following are some examples of delcaration specifiers (decl-specifier-seq):
    a. type specifiers: void, char, short, int, long, float, double, signed,
    unsigned, struct-or-union-specifier, enum-specifier, typedef-name, class specifier.
    *only one type specifier is allowed.
    **when declaring an object "Entity x;" - class/struct/union type Entity is also called "elaborated type specifier"
    b. storage-class specifiers: auto, register, static, extern
    c. explicit (only within the decl-specifier-seq part of the declaration of a constructor or conversion function 
    d. decltype sepcifier
    e. auto is also a declaration specifier
    f. cv qualifier - specifies constness or volatility of the object being declared or the type being named.    
    *if a cv qualifier with the the type specifier (a.k.a the declaration specifier)
    the base type is said to be cv-unqualified.


3.  init-declarator-list: 
    A declaration may include one or more declarator each separated by commas with optional initialization per declarator.

    A declarator (also called init-declarator (init for initialization), is the part of a declaration that specifies
    the name (identifier or declarator-id) to introduce into the program.

    The declarator itself is usually comprised of:
    1. single identifier 
    2. optionally a type modifier (creates a compound type - pointer built upon an int - pointer to an int)
    such as * (pointer-to) or reference & or array[].
    3. cv qualifier - can appear in init-declarator-list inside a declarator  (e.g: const pointer x)
    (cv qualifier can also appear in the decl-specifier-seq examples below).

    function parameters for example may not have declarators at all (annonymous), only type-specifiers: "void func(int)",
    or even parameters with declarators that have only modifiers such as * or & and no identifiers: "void func(int*)".

    These type modifiers are always a part of the declarator, unlike "int" which is a part of  decl-specifier-seq 
    (declaration spefcifiers). These modifiers are also called compound types: a type created from another underlying type -
    for example int *y; - identifier y is *pointer and an integer*.
    https://stackoverflow.com/questions/52250480/what-are-compound-types,
    https://stackoverflow.com/questions/48757854/why-is-a-pointer-considered-a-compound-type-in-c

    Despite modifiers such as * or & which belong to declarators, cpp presents the following confusion:
    for example: void func(int&){};  int& here means that we have a declaration of a parameter without 
    an identifer, where the declarator only contains "&" modifier and the type specifier contains an "int".
    this can be interpreted as an "(unnamed is a) reference to an int", a compound type where & is part
    of the declarator (init-declerator-list) that doesn't have an identifier,
    and "int" is part of the type-specifier (init-specifier-list).



examples (declarations analyzed):

a. "int x=5, *y;": (this all statement (ends with ;) is a declaration - it is comprised of 
the following two parts:
    1. decl-sepcifier-seq: int is a type specifier.
    2. init-declarator-list:
        i. declarator 1: declarator contains - 
        x (=identifier) which is declared + defined (initialized) via copy assignemnt to 5. x is of type int.
        ii. declarator 2: *y (=identifier) which is declared + defined + uninitialized. y is a pointer to int.
        * (pointer) is a type modifier that creates a compound type (pointer type built open integer type).
    
    *the base type of both x and y is an int (can be only 1 type specifier at declaration (such as int)),
    but identifiers can be compounded with pointers and ref types

b. int const unsigned* const array[50];
    1. declaration specifier (decl-specifier-seq): "unsigned const int" (base type specifier)
    note that cv-qualifier const belongs to the type specifier, therefore it belongs to the decl-specifier-seq part
    of declaration.
    2. init-declarator-list:
        i. declarator: "* const array [50]". (array is an array of 50 const pointers)
        this declarator  an identifier ("array"), and two modifiers /compound types (pointer and array)
        const here pretains to the constant pointers that lie inside that array

c.  volatile char (*fp)(float const), &r = c;
    1. declaration specifier (decl-specifier-seq): volatile char (base type)
    2. init-declarator-list:
        ii. declarator 1: (*fp)(float const): fp is a pointer to a function that accepts a const float
        and returns a volatile char. fp is the identifier, * is a modifier, and const pretains 
        to the float parameter.
        ii. declarator 2: &r is a reference to a volatile char.
        the declarator includes a modifier/compound type & and the identifier r

d.  int a=1, f();
    1. decl-specifier-seq: int (type specifier)
    2. init-declarator-list
        i. declarator "a = 1": contains identifier a and copy assignment with 1
        ii. declarator "f()": declares but doesn't defin a function taking no arguments and returning an int

*/

int a = 1, f();

int main()
{
    int x, * y;
    std::cout << "Hello World!\n";
    int const unsigned* const array[50] = {0};
}