#include <iostream>
/*lifetime - how much time an entity remains in memory before it gets deleted
scope - where we can access that variable
static - lifetime of entire program - even before main!!!
static inside a scope - only when accessed
static initializes only once!!!
static local - lifetime of entire program, scope limited to inside a scope (class, function, if..)
int i=0; equals to static i=0 inside function in terms of lifetime EXCEPT it is in a global scope;

*******************************************************************
EVEN BEFORE ANY CTOR is called, static members of a class are defined when entering the program.
static functions cannot modify non-static members and non-static functions cannot modify static members.
(ctors can't r/w static members). outside the class we access them by ClassName:: operator (because
static functions/variables belong to the class and not to a specific instance.
*******************************************************************
*/

class Singleton
{
private:
    static Singleton* s_instance;
    Singleton() {}; //private constructor
    int test = 2;
public:
    static Singleton* get_instance()
    {
        if (s_instance == nullptr)
            s_instance = new Singleton();
        return s_instance;
    }

    //alternative to upper function using local staticccc
    static Singleton& get()
    {
        static Singleton instance; //this will be created locally only once but a reference will be returned!
        // although the function dies, the static variable lives
        return instance;
    }

    int& get_test() // returning this to a variable int& x = get_test() will create a reference to a private variable!
        //changing x will change class member test, breaking encapuslation!
    {
        return test;
    }

    void hello()
    {
        std::cout << "shalom" << std::endl;
    }
};

Singleton* Singleton::s_instance = nullptr; //static variable member class must be declared outside -> cpp design...


void function()
{
    static int i = 0;
    i++;
    std::cout << i << std::endl; 
    // because i is static, it won't be reinitialized, calling function() multiple times won't reset i to 0
}

int main()
{
    std::cout << "Hello World!\n";
    Singleton* singleton = Singleton::get_instance();
    singleton->hello(); 
}
