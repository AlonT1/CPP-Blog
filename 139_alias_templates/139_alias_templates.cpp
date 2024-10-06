#include <iostream>

// https://www.modernescpp.com/index.php/alias-templates-and-template-parameters
// alias templates (cpp 11) - provide a means to give a convenient name to a family of types

// Matrix is a template class of type T (template type), with specified rows and cols (non-template type)
template <typename T, int rows, int col>
class Matrix 
{
};

// Now we want to create a specialized case of Matrix:
// A Square Matrix (rows==cols) and a Vector (col==1).
// Solution: use alias templates:
template <typename T, int rows> using Square = Matrix<T, rows, rows>;
// explained:
// 1. lhs: "Square" is an alias for "template <typename T, int rows>".
// 2. the two "rows" in the rhs are collapsed to a single "row" in rhs. In other words,
// "Square" represents a Matrix where rows and cols will be constructed with the same value.

//*************************************************************************************
// When instantiating a Square, we will provide T and rows: "Square<int, 5> square;"
// the compiler will automatically stencil out a "Matrix<int, 5, 5>" class. think about
// like this: the specified types from the lhs (T=int, rows=5) are used to fill out the types (int, 5, 5)
// of the rhs and create the Matrix 
//*************************************************************************************

// same goes for Vector:
template <typename T, int rows> using Vector = Matrix<T, rows, 1>;

// Advantages: we keep the same functionality and code of Matrix, without rewriting a full separate class
// for Square Matrix and Vector - code reuse!

//usage:

Matrix<int, 5, 3> matrix; // no alias
Square<int, 5> square; //equvalent to Matrix<int,5,5>
Vector<int, 5> vector; //equvalent to Vector<int,5,1>

int main()
{
    std::cout << "Hello World!\n";
}

/*
Notes: template specialzation is not a viable solution, because for template types
we specialize a specific type (int, float, etc,...) and for non-template types <int t>
we specialize a concrete value (2 for example). for example:
template <typename T, int rows> Square<  col can only specialized as a concrte number >
template <typename T, int rows> Square< col = row >   <- not possible

The using directive is used to bring a name into the current scope that otherwise is not.


*/