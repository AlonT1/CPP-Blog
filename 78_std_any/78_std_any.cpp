/*
std::any - only for cpp17
***********************************************************************
like std::variant but without template arguemnts, because it can store ANY type
***********************************************************************
the fact that variant requires to list the parameter types explicitly makes it more type safe
than "any". variant is a type safe union, we explicitly tell what it can store - helpful for programmers

std::any - for small types it stores them as a union, but for large types it uses dynamic allocation
(not good for performance). any and variant for small types work exactly the same way. 
a member bigger then 32 bytes will cause any to heap allocate (variant will not)
*/
#include <iostream>
#include <any>


int main()
{
    std::any data; 
    data = 2; //now data is an int
    data = "Hello"; // now data is const char*
    data = std::string("hello"); // now data is std::string

    //get the data
    std::string w = std::any_cast<std::string>(data);

    std::string t = std::any_cast<std::string&>(data); //faster, return by reference
}
