// This cpp file simulates C# delegates.
// C# delegates are function pointers with special feautres:
// multiple functions can subscribe and unsubscribe to the delegate (function pointer).
// a subscriber is simply some function!
// this allows the delegate to invoke multiple function (subscribers) that exist in different locations,
// with a single call with done to the delegate.
// Note: a delegate (and function pointers to functions) that accept X and returns Y can only be subscribed 
// (point at by functions  that accept X and return Y - same goes with function pointer in cpp).

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>


void method_a(const std::string& message) { std::cout << "methodA " + message + '\n'; }
void method_b(const std::string& message) { std::cout << "methodB " + message + '\n'; }

class Delegate // The delegate is essentially vector of subscribers (function pointers)
{
public:
    using ptr = std::function<void(const std::string&)>; // equivalent to "using ptr = void(*)(const std::string&)
    std::vector<ptr> subscribers; // vector that stores pointers to other functions

    // invokes all functions that subscribed to the delegate (== in the vec array)
    void operator()(const std::string& message)
    {
        for (ptr pointer : subscribers)
            pointer(message);
    }

    void operator+=(ptr func)
    {
        subscribers.push_back(func);
    }

    void operator-=(ptr func)
    {
        std::vector<ptr>::iterator it = std::find(subscribers.begin(), subscribers.end(), func);
        subscribers.erase(it);
    }
};

int main()
{
    Delegate delegate;
    delegate += &method_a;
    delegate += &method_b;
    delegate("shalom"); // calls both method_a and method_b with
    
}