
#include <iostream>

int main()
{
	int i = 8;
	//comma operator lhs operand is evaluated, and then right operand is evaluated and its result is returned!
	int x = (i++, 6); //i++ evaluated, 6 evaluated and its values is returned into x
	std::cout << x << " " << i; // i = 9, x = 6 
	
}