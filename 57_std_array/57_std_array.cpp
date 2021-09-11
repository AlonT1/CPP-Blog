/*
std array is based on a static array that doesn't grow - size is set in decleration.
advantages: we know the size of the array arr.size()
std::array uses array which is stored on the stack, vector stores the underlying array on the heap
std::array offers out of bounds protection (bounds checking) (only for debug mode, in release it won't occur
- optimized. this can be seen in the source code of std::Array (if define _CONTAINER_DEBUG_LEVEL). 
doesn't store the size! the style is a template argument, it doesn't return 
the actual size of the array, it returns the the parameter itself.

added layer of debug protection, no performance cost, help to keep track of the size
*/

#include <iostream>
#include <array>

void print_array(const std::array<int, 5>& data)
{
    for (int x : data)
        std::cout << x << std::endl;
}

//explained in 53_templates
template<typename T>
void print_array2(const T& arr)
{
    for (int i = 0; i < arr.size(); i++)
        std::cout << arr[i] << std::endl;
}


int main()
{
    std::array<int, 5> data;
    data[0] = 2; //std::Array overrides [] 
    data[1] = 3;
    data[2] = 4;
    data[3] = 5;
    data[4] = 1;
    
    print_array2(data);
}
