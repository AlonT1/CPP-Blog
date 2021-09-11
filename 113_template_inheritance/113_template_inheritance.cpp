#include <iostream>
//https://blog.feabhas.com/2014/08/variadic-templates/
//https://blog.feabhas.com/2014/06/template-inheritance/
//https://blog.feabhas.com/2014/07/template-member-functions/
//https://blog.feabhas.com/2014/06/template-classes/

/*******************most basic templated inheritance ********
this means that we inherit from template type T, or in another words we can inherit
from anything!
*/
class Base
{
public:
    int m_num;
    Base(int num) : m_num{ num } {}
};

template <typename V>
class Derived : public V //V - the class that we inherit from can be anything!
{
public:
    template <typename U> // we must use another template type for the ctor, V is occupied
    Derived(U num) : V{num} {} // delegating ctor
};

//can a template inherit from a template? probably not, because the templated 
//type that we inherit from must be a CONCRETE CLASS



int main()
{
    //we parameterize Derived as inherting from Base!
    //note that in Derived's ctor we use a template type - this type is automatically
    //deduced by the compiler according to the argument we provide - int in this case
    Derived<Base> derived(2);
}
