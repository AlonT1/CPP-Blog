/*
tl;dr

1.
unscoped enums - unscoped because if we declare another enum and use A in it, we'll get an error
enum Example{ A,B=3,C };  //A=0 B=3 C=4
Example a = Example::A;
"std::cout << a;" or "std::cout<<A" //no namespace is needed (unscoped)
enum Test{A} //A is already used in Example enum!

2. enum class Example{ A,B,C } // scoped enums - solve the problem above
strong enums are not implicitly converted to enum likew unscoped enums,
they require a static_cast<int>(Example::A);
3. enum class A{...} : long //enums can have an underlying types
4. annonymous enums "enum {size=100}" size becomes a literal just like literal 100, #define size 100,
or const static int size = 100;
5. to loop over enum values, store the enum values in an array"
enum Direction {  Up, Down, left };
const Direction dirs[] = { Up, Down, left };
for (Direction d : dirs)  std::cout << d;

6. enum Week {Mon, Tue};
both in C and CPP this will work:
std::cout << Mon; //cpp
printf("%d", Mon); //C
meaning that we can use one of the enumerators directly (in cpp strong enums solve this)
if we still want to decalre an enum:
Week day = Week::Mon; // Cpp
(in C, as with struct, we use the full type "enum Week" - CPP allows
to omit enum keyword when instantiating an enum, same with struct)
enum Week day;  day = Mon // C


enum- a way of given a name to a value
enumeration or just enum enable grouping together a set of const integral values as a single type.
enums are synonymous to constant literals - both are prvalues, both don't take up space,
both don't have performance overhead, and both don't have an identity (address in memory)*/

#include <iostream>


//not grouped, and integers
int a = 0;
int b = 1;
int c = 2;

//enums are alternative to the above:
/****standard (unscoped) enums****/

enum Example //enum name (optional - can be anonymous - 
{
    //the following are the enumerator list:
    A, B, C //first  zero by default,   A=5, B, C ==>  A=6 B=6 C=7 //increments by 
    //Standard enums don't have a namespace ==> "Example example = A", and not "Example example = Example::A"
};

enum Example2
{
    //A, C // standard enums are unscoped!!!! Error for A,C they're defined in Example!
    //solution: diff names/ diff namespaces / strong enums
    D 
};
///************************/


/**Strong (scoped) enums**/  //scoped and have a namsepace e.g: Colors::Red
enum class Colors
{
    Red, Green, Blue
};

enum class Ken
{
    Red 
};

// enums by default are of integral type. It is now possible to change that by
//specifying an underlying type:

enum class Color : long
{
    Red, Green, Blue //0, 1, 2
};


// regular/ class enums cam be written like this:
enum class Heights // enum names
{
    Small, Medium, Large //the enumerators - representing 0, 1, 2
}/*Altitudes; // Creates an enum variable or...*/;
Heights Altitudes; // alternative to create an enum variable (not called an object)

/*
The decleration above is equivalent in principle to the following syntax:
int  x;

"enum class Heights" - Heights is a user-defined type, unlike "int" (built-in type) - both types
resides in the decl-specifier-list part of the declaration which contains the underlying type
see 30b_declarations_declarators
"Altitudes" - is equivalent to "x" - giving an identifier to to the heights enum
both reside in the init-declarator-list


with structs a similiar syntax means:
struct _bar_
{
    int a;
} bar; //defining bar object of type _bar_

bar is a declaration of a variable of type struct _bar_.
the same as struct _bar_ bar;

in enums "Altitudes" has no effect
*/


enum
{
    one, two
}Level;


//cpp enum hack (anonymous enum):
class Player
{
private:
    /*emulates a constant literal: because enums are equivalent to constant literals
    as explained in the top of the file, using an anonymous enum can emulate
    a constant integer: so instead of using 100 as a literal we can just use SIZE,
    both have no identity (memory address), identical overhead, don't take up space,
    and both are considered prvalue expressions
    this is more comfortable then a non-anonymous enum where we need to type:
    enum_name::enumerator;*/

    enum { SIZE = 100 };
    int weapons = SIZE;

    /*
        method is similar to:
    1. #define SIZE 100
    only that that the enum hack occurs at compile time, the #define example occurs at preprocessing time.
    in addition, #define isn't suitable for debugging as compared to the anonymous enum

    2. static const int SIZE = 5;  (only valid as a class member!)
    The standard states that a class member that is a  static const/constexpr integer becomes
    a constant expression, just like the literal 5 or the SIZE.	https://stackoverflow.com/questions/50033574/why-initialization-of-static-member-variable-is-not-allowed-inside-class-but-ini?rq=1
    */

};

//pass enum to a function
void func(Colors color)
{

}

int main()
{
    //weak enums (unscoped)
    Example x = Example::A; //although standarad enums are unscoped they do have a namepsace
     // unscoped - we don't need the namespace to print A, implicitly converts to int
    std::cout << A << '\n';

    Color color1 = Color::Blue;
    //unlike unscoped enums, scoped enums are not implicitly convertible to ints - we need a cast:
    std::cout << static_cast<int>(Color::Blue) << '\n';

    // a comment in https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
    //tells the statement above won't work if if the enum is explicitly given an underlying type
    //so the comment suggests the following cast
    std::cout << static_cast<std::underlying_type<Color>::type>(color1) << '\n'; // prints enums
    
    //But! Color enum above DOES have an underlying type and the "simple" static_cast still works - 
    //a discrepancy!

    //strong enums
    Colors color2 = Colors::Blue; 
    //std::cout << color; // unliked an unscoped enumeration,
    //a scoped eumeration is not implicitly convertible to its integer value, we need to explicitly convert it to an integer using a cast
    std::cout << static_cast<std::underlying_type<Colors>::type>(color2) << '\n';

    //pass enum to a function
    func(Colors::Blue);

    enum Direction
    {
        Up, Down, left, Last //last is a placeholder, we loop UNTIL we encouter it
    };

    //in C:
    enum Week {Mon, Tue}; // enum keyword mandatoy in C, unless the enum is typedefed as "typedef enum {...} Week"
    Week week = Week::Mon; //in cpp, enum isn't mandatory
    enum Week day;
    day = W


}
