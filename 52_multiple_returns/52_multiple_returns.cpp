/*
in cpp you can't return two types. solutions: struct!
*/
#include <iostream>
#include <array>
#include <tuple> //for tuple

////strategy 1, using pointers /references
//disadvantage: the function flow isn't clear function is void, but pretends to return values
void func(int& outA, int& outB) // out denotes (output variable) the function outputs them after processing
{
    outA++;
    outB++;
}


//strategy 2, return heap int* array
//disadvantage: new allocation on the heap! (stack allocation isn't possible since we will return address
//to an array thats get deleted when it goes out of the scope of this function
//we also don't know the size of the array!
int* func2() // out denotes (output variable) the function outputs them after processing
{
    int a = 2;
    int b = 3;
    return new int[2]{ a,b };
}

//strategy 3 - return std::array
//disadvantage returns by value! copies the array!
//std::array - array is on the stack, std::vector - array on the heap -> std::array therefore faster(?)
std::array<int, 2>* func3()
{
    return new std::array<int, 2> {1,2};
}

//strategy 4 - tuple - a class which can contain x amount of variables
std::tuple<std::string, int, float> func4() // rvo optimized
{
    return std::tuple<std::string, int, float>{ "hello", 2 , 3.2};
}

//strategy 5 - pairs -  like tuple, but only for pairs. offers better syntax than tuple's std::get<0>(tuple)
std::pair<std::string, int> func5() //rvo optimized (single construction of pair at the stack frame of caller)
{
    return std::pair<std::string, int>{ "hello", 2};
}

//strategy 6 - struct - The BESTT???
struct Entity
{
    std::string str;
    int number;
};

Entity func6() // rvo optimized
{
    return Entity{ "hello", 2 };
}

//strategy 7 - destructuring (structured bindings)
Entity foo()
{
    return Entity{ "hello", 2 };
}


int main()
{
    //strategy 1, using pointers /references
    int x = 0, y = 0;
    func(x, y);
    
    //strategy 2 - return int* heap array
    int* arr = func2();
    std::cout << arr[1] << std::endl;

    //strategy 3 - return std::array
    std::array<int, 2>* w = func3();

    //strategy 4 - tuple - a class which can contain x amount of variables - doesn't care about the class
    std::tuple<std::string, int, float> tup = func4();
    std::cout << std::get<0>(tup) << std::endl; // get first
    std::cout << std::get<1>(tup) << std::endl; // get second
    std::cout << std::get<2>(tup) << std::endl; // get second
    //std::get is also used for accessing elements from std::array and std::pair

    // strategy 5 - pairs -  a type of tuple, but only for pairs (2 elements). 
    // offers better syntax than tuple's std::get<0>(tuple) for element retrieval (first and second).
    std::pair<std::string, int> pair = func5();
    std::string text = pair.first;
    int num = pair.second;
    std::cout << text << " " << num << std::endl;// better syntax! but what is first? unnamed variables...
    //although std::get syntax also works for pairs
    std::cout << std::get<0>(pair) <<  std::endl;// better syntax! but what is first? unnamed variables...

    //strategy 6 - struct
    Entity test3 = func6();
    std::cout << test3.number << std::endl; // clear names!

    //strategy 7 - structured bindings (read 75_structured_bindings for more info)
    auto [a, b] = foo(); //dereference the object in foo pointer (retrieves the struct)
    std::cout << a << std::endl; // a equals to str, b equals to number

}
