#include <iostream>
#include "Player.h"

//declaration - introduce one or more names into a translation unit or redeclare names introduced by previous declarations.
void func(); // forward decleration (this thing exists but not yet give a defintion)
void func() { } // definition


class Entity; // empty class declaration
class Entity // definition - "make memory for it!"
{
};

int main()
{
    Entity e;// object decleration, initalization(implicit) and definition
    int x; //decelration("this thing exists and named x (identifier)") + definition("make memory for it" - x is allocated
    //in memory but it's value is uninitalized!!!
    //std::cout << x << std::endl; //invokes error, x is unitialized
    x = 4; // initialization! - giving the INITIAL value to a varaible. for pod's (plaid-old-data: floats, ints..),
    //initialization can be seperate from decleration+definition, but it can also be combined to a single statement:
    int y = 4; // decleration + definition + initialization. Again, this is not assignment (assignemnets are for not-yet-created
    //objects and only after they were initialized. So why do we use the assignment operator(=)? this is a syntactic sugar for:
    int z{ 3 }; //decleration+definition+list initialziation (has benefits over the statement above, see 25_constructor)
    x = 3; //copy-assignment! x was already declared+defined+initialized, now we assign it with a new value

    Player p(1, "ken"); //object initialization (decleration at the header file, definition at cpp)

    std::cout << x << std::endl;
}
