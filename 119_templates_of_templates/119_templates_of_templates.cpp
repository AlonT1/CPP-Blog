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

    //Should be read: CustomClass is a template type parameter that is a template itself.
    //CustomClass can represent any template class (in this case "Entity") that has
    //a single template type parameter (called "V" in this case, instantiated as "int" below).
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
    CustomClass<int> var3{ 4 }; //instantiating CustomClass ("Entity" in this case)
    //parameterized with an int and initialized with 4 via "default member initialization".
};

int main()
{
    //instantiating class Main and supplying the Entity type to serve as the 3rd template type,
    //when Entity is a template class itself.
    Main<int, double, Entity> x{1, 2, 5};//initializer list overrides default list initialization!
    //var3 of class instance x equals to 5 instead of the default value 4
    std::cout << x.var1 << ' ' << x.var2 << ' ' << x.var3.var;
}