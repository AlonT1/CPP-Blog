#include <iostream>
#include "Storage.h"

int main()
{
    //*******************regular, non-specialized, version of storage
    Storage<int, 10> nValue(5);
    nValue.print(); // the template non-specialized function print()


    //*******************calling the full-specialized version of print() function:
    //because full-specialized version of print() is specialized
    //for a "double" type and a 15 "non-type, in order to call this function
    //we must instantiate the template version of the Storage with the same explicit
    //values as the function was specialized with
    Storage<double, 15> dValue(3.0);
    dValue.print(); // template specialized function of a class print()

    //calling the non-specialized version of strage - the full-specialized version
    //above was specifically specialized for a double and 15 literal. Since 10
    //is not 15, this causes a mismatch, and therefore we call
    Storage<double, 10> dvalue(1.0);
    dvalue.print(); 

    //*******************calling template specialized "normal" non-class function
    test<double, 23>(2.0); // non specialized version of test()
    test<char, 5>('t'); // fully specialized version of test()
    //note that the explicit template parameters, must match the parameters that test()
    //was specialized with


    //*******************class template specialzation
    //we specialized the class with a bool type and 15 literal in Storage.h
    //instantiating Storage with these values, will create the specialized
    //version of the class with the members that exists within it (without any connection
    //to the data members of the non-specialized Storage class version
    Storage<bool, 15> bValue; 


}