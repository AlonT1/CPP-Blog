#include <iostream>


int x = 5;
int y = 10;

int main()
{
    if (x > 4)
        y = 2;
    else
        y = 3;

    //the above is equivalent to:
    y = x > 4 ? 2 : 3;  //if (x>4) y=2 else y=3


    //ternary operator is not only a syntactic sugar!
    std::string t; //constructing empty immutable (read-only) string!
    if (x > 4)
        t = "test1"; //a new memory is allocated for this new string (strings are immutable) + assignment operator
    else
        t = "test2";
 
    // in this example we construct and assign a string only once!
    //as opposed to the previous example of construction + memory allocation for new string + assignemnt
    std::string z = x > 4 ? "test1" : "test2";


    //nested ternary
    if (x > 4 && x < 10)
    {
        if (x > 8)
            y = 3;
        else
            y = 4;
    }
    else
        y = 5;

    //equivalent to above
    y = (x > 4 && x < 10) ? (x > 8 ? 3 : 4) : 5;

    
}
