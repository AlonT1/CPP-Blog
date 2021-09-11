#include <iostream>

template<typename T>
void change(T n) {
    //if n is std::reference_wrapper<int>, 
    // it implicitly converts to int& here.
    n += 1;
}

//class members can be of reference type - the problem is that
//references cannot be rebinded, meaning that once m_str is initialized (binded) 
//with str, m_str reference cannot be rebinded to another variable.
//the consequences are the copy semantics and move semantics won't
//work with reference members because they cannot be rebinded.
//std::reference_wrapper is a class template that wraps a reference in a copyable,
//assignable object, which solves the problems above - AND! we don't need pointer
//syntax as customed with references.

class Entity
{
public:
    std::string& m_str; // reference is binded to a variable at construction (=initialization)
    Entity(std::string& str) : m_str{ str } {}
};

int main()
{
    int x = 10;

    int& xref = x;
    change(xref); //Passed by value, x is still 10
    std::cout << x << "\n"; //10

    //now we parameterize the template to take in an lvalue ref
    //now x is 11
    change<int&>(x);
    //equivalent to: "change(&x)" the compiler auto deduces the template type as int&
    std::cout << x << "\n"; //11

    //std::ref returns a reference_wrapper
    change(std::ref(x)); //Or use std::ref Passed by reference, x is 12 now
    //T is deduced to int&, the call will look like this, change<int&>(x)
    std::cout << x << "\n"; //12

    int& b = x;
    std::cout << &b;

    //using a reference isn't possible in STL containers, because reference
    //types are not eraseable types (as required by STL containers)
    //in other words these objects cannot be destroyed.
    //std::vector<int&> v; error!

    //references must be binded to variables at initialization: "int& x = y;"
    //once they are binded, the cannot be rebinded:
    int x1 = 10, y1 = 20;
    int& x1ref = x1, y1ref = y1;
    x1ref = y1ref; // although x1ref is assigned with the value that is represented
    //by y1ref, x1ref IS STILL A REFERENCE TO "x1" and not "y1"

   // std::reference_wrapper<int> nr; //Error! must initialize.
    std::string str1{ "Hello" };
    std::string str2{ "World" };
    //std::ref returns a std::reference_wrapper, a reference type that is copyable
    //and can be rebinded
    std::reference_wrapper<std::string> r1 = std::ref(str1); //OK
    std::reference_wrapper<std::string> r2 = std::ref(str2); //OK
    
    //Assignment rebinds the reference_wrapper  
    r2 = r1;  //r2 also refers to str1 now 
    //Implicit conversion to std::string&
    std::string cstr = r2; //cstr is "Hello"

    //Possible to create an array of reference_wrapper!
    std::reference_wrapper<std::string> arr[] = { str1, str2 };

}

//https://www.nextptr.com/tutorial/ta1441164581/stdref-and-stdreference_wrapper-common-use-cases
//https://oopscenities.net/2012/08/09/reference_wrapper/