
#include "MathLibrary.h"

namespace MathLibrary
{
    //functions are static members of a class, thus accessible by scope operator
    //through the name of the class.
    double Arithmetic::Add(double a, double b)
    {
        return a + b;
    }

    double Arithmetic::Subtract(double a, double b)
    {
        return a - b;
    }

    double Arithmetic::Multiply(double a, double b)
    {
        return a * b;
    }

    double Arithmetic::Divide(double a, double b)
    {
        return a / b;
    }
}