// 35_Multiple_Inheritance.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

class Mother
{
public:
    int mother_data = 10;
    virtual void mother_method() {}
};

class Father
{
public:
    int father_data = 9;
    virtual void father_method() {}
};

class Child: public Mother, public Father
{
public:
    int child_data = 8;
    virtual void child_method() {}
    void father_method() override {}
};

int main()
{
    Mother m;
    Father f;
    Child c;
    c.Father::father_data; //access father_data through the Father memory portion of c
    //child c contains in memory portions of Father, Mother and then Child (in order of inheritance)
    Father* child = &c;
    child->father_method();
    // child ptr (of type Father) points to the Father portion of c which doesn't
    //exist at the start of address of c (Mother portion exists there), but rather in an offset
    //from start address of c, calculated by the compiler!

    std::cout << "Hello World!\n";
}
