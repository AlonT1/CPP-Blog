#include <iostream>
#include <type_traits>

/*
type traits (since cpp11) -
template struct that can do 2 things:
1. Query: ask a question about a certain type - some type traits struct hold a
member that answers this question
2. Transformation: some type traits perform a transformation on a type
*/


//https://stackoverflow.com/questions/43434491/difference-between-if-constexpr-vs-if
//https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp
template<typename T>



void algorithm_signed (int i )


int main()
{

    // examples for querying:
    //"value" member answers if "float" "is_floating_point" (prints 1)
    std::cout << std::is_floating_point<float>::value;
    
    /*
    how does works?
    AT COMPILE-TIME (templates are generated only at compile-time - read 53_templates), the compiler
    roughly generates the following code, according to the parameterized type:

    //if the parameterized type is "class" the following struct is generated:
    struct is_floating_point_class {
        static const bool value = false;
    };

    //if the parameterized type is "float" the following struct is generated:
    struct is_floating_point_float {
        static const bool value = true;
    };

    //if the parameterized type is "int" the following struct is generated:
    struct is_floating_point_int {
        static const bool value = false;
    };
    
    because the member of each genereated struct is static, it is accessible by :: (scope resolution)
    */

    //the type trait std::remove_reference is used in 114_perfect_forwarding
}