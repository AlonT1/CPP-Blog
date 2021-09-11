/*
singleton design pattern - single instance of a class that we have around. 
objects are meant to be instantiated multiple times. a single of instance of an object
with a specific functionality eludes to a singleton. e.g: random number class, utility class,
renderer. can behave like namespaces in cpp, essentialy singleton in cpp is organized
data in a single location

We could potentially write everything inside a namespace, or mark all functions of
a class as static and later access symbols as Class::func(), but then we will lose
the OOP features such as class members, encapsulation, inheritance, and virtual functions.
*/

#include <iostream>

class Singleton
{
public:
    int x = 2;
    static Singleton& Get() // static function -> can access static class members
    {
        //static Singleton s_instance;
        /*alternative to the s_instance in private, defined outside of the class
        this would not be destroyed when leaving the Get() function because it resides in the static section
        of memory and not on the stack! and will die when the program dies. calling Get(), will not redifine
        s_instance, it is only defined once when during the first call fo the function.
        Note that s_instance was already defined (line 38) even before the class was instantiated!
        and is initialized and defined only once!
        */
        return s_instance;
    }
    void function() {}
    
    Singleton(const Singleton& other) = delete; // removes copy ctor
private:
    static Singleton s_instance; //This is the singleton! only a decleration. static - shared across all instances of this class
    Singleton() { } // private ctor - class cannot be instantiatied outside this class
};

Singleton Singleton::s_instance; //static class members are defined(=initialized)
//outside of class (see 22_class_static). s_instance is of type Singleton. 
//s_instance is initialized on the stack as a Singleton object only once!!!

int main()
{
    //Singleton instance = Singleton::Get(); - bad because it invokes the copy ctor!
    Singleton& instance = Singleton::Get(); // ok! - returning a reference  
    auto& instance1 = Singleton::Get(); // ok

    instance.function();//singleton -> do something!
    instance.x; // the singleton static instance can access non-static members


}