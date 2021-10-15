#include <iostream>
//https://blog.feabhas.com/2014/08/variadic-templates/
//https://blog.feabhas.com/2014/06/template-inheritance/
//https://blog.feabhas.com/2014/07/template-member-functions/
//https://blog.feabhas.com/2014/06/template-classes/

class Base
{
public:
    int m_num;
    Base(int num) : m_num{ num } {}
};



/********* subclass is a template class inehriting from a concreete class********
this means that we inherit from "template type T", or in another words we can inherit
from class T!*/


template <typename V>
class Derived : public V //V - the class that we inherit from can be anything!
{
public:
    Derived(int num) : V{num} {} //delegating the construction to the ctor of V
};

//can a template inherit from a template? probably not, because the templated 
//type that we inherit from must be a CONCRETE CLASS



/********* subclasses is a concrete class, inheriting from a templated class*******
For this to work the inheriting subclasses must parameterize the template type with some type.
Practical purpose? see 122 double_dispatch*/
template <typename T>
struct Vehicle {};

struct Mercedez : Vehicle<Mercedez> {};
struct Lamborghini : Vehicle<Lamborghini> {};



int main()
{
    //we parameterize Derived as inherting from Base!
    //note that in Derived's ctor we use a template type - this type is automatically
    //deduced by the compiler according to the argument we provide - int in this case
    Derived<Base> derived(2);
}
