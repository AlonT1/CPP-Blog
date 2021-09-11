/*enum- a way of given a name to a value
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
    //A, C // standard enums are unscoped!!!! Error for A,C they're defined in Example! solution: diff names/ diff namespaces / strong enums
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
}; //definition of an enum

/*
The decleration above is equivalent in principle to the following syntax:
int  x;

"enum class Heights" is equivalent to "int" - in both we declare a type and both
reside in the decl-specifier-list of the declaration which contains the underlying type
*enum class Heights - we not only declare a type, we also DEFINE a user-defined type 
called Heights using the curly braces

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

    /*This method is similar to:
    1. #define SIZE 100
    only that that the enum hack occurs at compile time, the #define example occurs at preprocessing time.
    in addition, #define isn't suitable for debugging as compared to the anonymous enum

    and similar to 
    2. static const int SIZE = 5;  (only valid as a class member!)
    The standard states that a class member that is a  static const integer becomes
    a constant expression, just like the literal 5 or the SIZE.
    */
 
    /*
    enum {SIZE = 100} //anonymous enum - behaves like a literal, like a #define
    why not use #define? not good for debugging and not good when it is used only once
    in addition, this trick doens't allocates new memory, SIZE is a literal prrvalue
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
    std::cout << A << '\n';

    Color color1 = Color::Blue;
    std::cout << static_cast<std::underlying_type<Color>::type>(color1) << '\n';

    //strong enums
    Colors color2 = Colors::Blue; 
    //std::cout << color; // unliked an unscoped enumeration,
    //a scoped eumeration is not implicitly convertible to its integer value, we need to explicitly convert it to an integer using a cast
    std::cout << static_cast<std::underlying_type<Colors>::type>(color2) << '\n';

    //pass enum to a function
    func(Colors::Blue);
}