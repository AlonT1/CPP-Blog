/*

cpp expressions: a sequence of operators and their operands, that specifies a computation. e.g (a+b)
Each cpp expression has two independent properties:

1.type: a property that both restricts the operations that are permitted and provides semantic meaning
to the otherwise generic sequence of bits (32 bits and can represent an int which can be summed, while
the same 32 bits can represent an Player class (user-defined type).


2. value categories:
each expression belongs to EXACTLY one of the tree primary value categories: prvalue ,xvalue, lvalue


        expression
        |       |
glvalue         rvalue
|      |      |        |
lvalue  xvalue          prvalue


lvalue: object with location (storage) in memory, with a name (have an indentity). e.g: x in  "int x = 4";  
prvalue (pure rvalue):  literal, a constant that on its own, temporaries that aren't stored in memory
for later inspection .e.g: literals(1, 'w') ,std::string("123"), and expressions who & (address-of)
cannot be applied to them (no identity). lvalues cannot be moved
xvalue (eXpiring value): 



note: rvalue references are lvalue themselves because they have an identifier and a memory address
(e.g int&& w = 4 - w is an rvalue reference that is an lvalue itself - w is modifiable with a memory address).
rvalue references can only be bound to rvalues (e.g: int&& w = 4;) and can't be bound to lvalues or lvalues references

lvalues can be assigned with lvalues and rvalues
lvalues references can be assigned only with lvalues
const lvalue ref can be assigned with either lvalues or rvalues (both would be umodifiable)
rvalue refernces can only be assigned with rvalues

int y = 4;
int& x = y;

int&& w = y // not allowed - y is an lvalue, cannot be binded to an rvalue ref
int&& w = x; // not allowed - x is a lvalue ref, cannot be binded to an rvalue ref

int&& w = (int&&)y //allowed
int&& w = (int&&)x //allowed - by casting to rvalue ref we tell the compiler that x (or y) is an rvalue ref
to some rvalue that they store(refering to, bindto) (rvalue ref can only store(refer to) rvalues),
and since w is also a rvalue ref, it can reference (store) the same rvalue x (or y) is refering to





lvalue - if expression e has an identity (identifier, address) that makes it accessible outside 
of the expression.

xvalue - if a function returns (implicitly / explicitly) an rvalue reference then the value is an xvalue

prvalue - it isn't an lvalue or an xvalue

rvalue (historic) - is a prvalue or an xvalue.
*/


#include <iostream>




int get_value() // returns an rvalue - a literal! 
{
    return 10;
}

int& get_value2() // returns an lvalue reference
{
    static int value = 10; //initialized only once when calling this function, lifetime -> endless
    return value;
}

void set_value(int value){}

void set_value2(int& value){}

void set_value_const(const int& value) {}

void print_name(std::string& name) {} //the parameter is a reference to an lvalue
//in other words, this function only accepts non-temporary objects with location
//in memory (lvalues). it cannot receive rvalues!

void print_name_const(const std::string& name) {} //can receive lvalues and rvalues

void print_name_const(const std::string& name) {}


void print_name_temp(std::string&& name) {} //takes in only temporary objects (rvalues)
//no lvalues are allowed!! std::string&& means a reference to an rvalue (rvalue ref)
//void print_name_temp(const std::string& name) {} //note: an rvalue can be also be passed to to the function
//above, but if it's overloaded by a function which can take an rvalue ref like this function, then
//this function will be chosen.


int main()
{
    int i = 10;  // lvalue - object with location (storage) in memory. 
    //rvalue - literal, a constant that on its own, cannot be inspected again later
    //lvalue can be assigned with an r value (example above) or with an lvalue:
    int a = i; // a and i are lvalues(have location in memory)
    //10 = i //error! rvalue 10 cannot be assigned with the lvalue l
    get_value2() = 5; //ok because get_value2() returns an lvalue reference,
    //we modify the the number itself, affecting the actual memory

    set_value(10); // set_value with an rvalue
    set_value(i); // set_value with an lvalue

    set_value2(i); //ok!
    //set_value2(10); // cant assign an rvalue to an lvalue reference! - it says in the name:
    // a reference to an LVALUE!! not an rvalue!
    //but there is an exception:

    const int& x = 10; //we can assign an rvalue to a CONST lvalue reference.
    //the compiler probably converts this to:
    int temp = 10;
    const int& w = temp;
    /*
    why the compiler allows to assign a prvalue such as 10(from the rvalue non-primary category)
    to the completely different glvalue non-primary category which const lvalue reference stems from? 
    if we think about it, an lvalue reference is a reference to an lvalue, an object in memory
    with an address and name, and prvalue 10 is just an unchangable literal, so why is this allowed?
    by adding the CONST qualifier we promise to fulfill the "read-only" contract of rvalues.
    we promise that the const lvalue reference will not change the rvalue and this coincides
    with the fact that the rvalue is an unchangeable literal. Also, by accpeting rvalues,
    const lvalue ref function parameters add additional flexibility by allowing to pass 
    rvalues to the function (which we are forbidden to change due to const), as well as lvalues.
    This is especially useful for copy constructors that can accept rvalues because of this
    */

    //therefore:
    //because the function's parameter is a const lvalue reference, the arguments can be also an rvalue
    set_value_const(10);
    set_value_const(i);

    //in the 3 following statements all lhs is lvalue, all rhs is rvalue
    std::string first_name = "Nihcolas"; 
    std::string last_name = "Cage";
    std::string full_name = first_name + last_name; // although first_name & last_name
    //are rvalues, first_name+last_name constructs a new std::string which is an rvalue
    // same as 5+6 = 11 -> 11 is an rvalue! essentialy a temporary object (temp string)

    print_name(first_name); //ok - lvalue reference to... lvalue
    //print_name(first_name + last_name); // error lvalue ref to.. rvalue!
    print_name_const(first_name + last_name); // ok - lvalue ref to a const rvalue

    //&& functions who take in only a temprary values
    print_name_temp("hello");//ok
    //print_name_temp(first_name); // error - rvalue ref can't take in an lvalue

    /*
    this is all really cool, because now we can detect when a function takes in a temporary object (rvalue)
    or an object with a location in memory (lvalue). useful for move semantics optimizations ??????????????
    when receiving a temporary value we can manipulate it however we want, without having fear that some
    other component uses it. manipulating lvalues is risky, because they can be used in other locations
    */
}