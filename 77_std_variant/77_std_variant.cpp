/*
variant (cpp 17) allows to initialize a variable that can take up the form of predifined types
variant is a type safe union, we explicitly define the types it houses, and types we get from it.
*/

#include <iostream>
#include <variant>

int main()
{
    std::variant<std::string, int> data; //data can be exclusively string or int 
    data = "Hello"; // now data is ONLY std::string
    std::cout << std::get<std::string>(data) << std::endl; // get the string
    data = 2; // now data is ONLY an int
    std::cout << std::get<int>(data) << std::endl; // get the int 

    //check the type of the data to do a "safe" get (ensuring that data is a string when try to get a string)
    data.index(); // which index (each type is an index - string is 0, int is 1)
    if (data.index() == 0) {} // if data is a string

    //better alternative
    auto str = std::get_if<std::string>(&data); // if not string - > returns null
    if (str) {}

    //variant vs uniion

    union unionab// the size of the union is the size of the biggest variable
    {
        std::string a;
        int b;
    };
    //variant stores all the possible data types as seperate variable members
    std::cout << sizeof(std::string) << std::endl; // 40 bytes
    std::cout << sizeof(unionab) << std::endl; // 40 bytes

    std::cout << sizeof(data); // size of data is 48 (40 for string, 4 for int, extra 4 for std::variant(padding?))

    //variant is essentialy a struct/class that stores the data types as members.
    //unions are more efficient and better. variant is much more type-safe without undefined behaviour
    //unions are good for low level performance!
}
