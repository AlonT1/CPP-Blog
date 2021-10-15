/*
user defined conversion function
Enables custom implicit conversion or explicit conversion from a class type to another type.
*/

#include <iostream>

class Entity
{
public:
    int m_x = 2;

    // customizing the conversion of this class when it is converted to an int.
    //adding explicit function specifier to the definition makes the
    //user-defined conversion function prohibit unintended implicit 
    //conversions from being applied. Such conversion functions are 
    //called explicit conversion operators.
    //prevents cases such as "int x = object;" - error! only explicit 
    //conversions can occur "int x = (int)object;"
    //return type isn't necessary because the object is effectively converted to an int
    //and this function "explains" what this conversion means

    //int serves both as the function name and the reutrn type
    explicit operator int() const //const - function promises not to modify member variables
    {
        return 8.5; //note the double here, explanied below
    }
    
    operator bool() const
    {
        return m_x > 1;
    }
};

int main()
{
    Entity x;
    // w = 8 not although the return is 8.5! 
    //because int conversion has been customized return 8.5 when
    //Entity is converted to an int
    int w2 = (int)x; //the same as "int y = static_cast<int>(x)";
    
    std::cout << w2;

    if (x) // this statemetn invokes conversion of x to bool, we customized this conversion
        //to return true if m_x > 1  (in operator bool() function).
    {
        std::cout << "shalom";
    }
}