#include <iostream>
/*
The basic difference between type conversion and type casting, i.e.
type conversion is made “automatically” by compiler whereas, 
type casting is to be “explicitly done” by the programmer.
*/

int main()
{
	int x = 5;
	//floats are double unless suffixed with f which turns them into a float.
	float y = x;  // y = 5.0000... type conversion (done automatically by the compiler)

	int s = (short)x; // explicit type cast to short

}