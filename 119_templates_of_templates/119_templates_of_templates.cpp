#include <iostream>

//https://blog.feabhas.com/2014/08/templates-of-templates/
//https://stackoverflow.com/questions/42125733/mixing-types-and-nontypes-in-variadic-template-parameters


template <class T>
class Entity
{
public:
    T var;
};

template <typename T, 
	typename U,

    /* should be read right to left:
    "template <typename V> typename CustomClass",
    1. "typename CustomClass" - CustomClass can represent any parameter type...
    2. "template <typename V>" - more specifically, CusotmClass can represent any template
    which can be parameterized with a single parameter type V*/
	template <typename V> typename CustomClass,

    // the trailing template types can have default types, unless
    //specified otherwise at instantiation
    typename W = uint32_t> 
class Main
{
public:
    T var1;
    U var2;
    W var4;
    CustomClass<int> var3{ 4 }; //CustomClass can represent any class with any template parameter
};

int main()
{
    //instantiating class Main and supplying the Entity type to serve as the 3rd template type,
    //when Entity is a template class itself which can be parameterized with a single type, just like CustomClass.
    Main<int, double, Entity> x{1, 2, 5};//initializer list overrides default list initialization!
    //var3 of class instance x equals to 5 instead of the default value 4
    
    std::cout << x.var1 << ' ' << x.var2 << ' ' << x.var3.var;
}