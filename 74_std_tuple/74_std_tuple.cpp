#include <iostream>
#include <tuple>

/*
tuples are similar to structs. tuples should be used as one-time only
structure to transfer data from functions, or as a type in stl containers
tuples can receive multiple template parameters and be seen as an extension
of std::pair which can only operate with 2 type parameters
*/

struct Person { int age; std::string name; };

int main()
{
    //make_tuple is similar to "emplace" in vectors, we create the tuple in 
    //the stack frame of the tuple - no redundant copies are made - for example
    //we don't construct "shalom" std::string outside and then copy it to the tuple,
    //but rather construct it directly in the tuple.
    std::tuple<int, std::string> tup = std::make_tuple(1, "shalom");
    std::tuple<int, std::string> tup2 = std::make_tuple(2, "shalom");
    std::tuple<int, std::string> tup3; //int and string are "default" constructed

    std::string my_name = "shemesh";
    //tuples can store references to existing variables. STL Containers such
   //as vectors cannot
    std::tuple<std::string&> tup4{ my_name };
    std::get<0>(tup4) = "makore"; //my_name is also "makore" now
    std::cout << my_name << " " << std::get<0>(tup4) << '\n';

    

    //struct vs tuples - both can hold multiple data types:
    std::cout << std::get<0>(tup) << " " << std::get<1>(tup) << '\n'; //tuple get - awkward syntaxh
    Person p{ 1 , "shalom" }; //aggregate initialization
    std::cout << p.age << " " << p.name << '\n'; //

    //std::tie and std::tuple
    //std::tie - allows unpacking of std::tuples (and std::pair) into variables
    //in the same order of the tuple. std::ignore means that we skip the 1st
    //element of the tuple (no variable is going to receive this value during unpack).
    //std::ignore causes the 1st parameter of tuple "tup" (int) to be skipped
    //during the binding process.
    std::string name;
    std::tie(std::ignore, name) = tup; // invokes std::tie(std::ignore, name).operator=(tup)
    //where the bind occurs
    name = "haim"; //won't affect the value in the tuple itself
    std::cout << name << " " << std::get<1>(tup) << '\n'; //prints haim shemesh
}

/*
Yeah, std::tie() is a lot less useful since C++17, where structured bindings
are usually superior, but it still has uses, including assigning to existing
(not simultaneously newly declared) variables and concisely doing other things
like swapping multiple variables or other things that must assign to references.
*/