#include <iostream>
#include "log.h"


// static functions are functions that are only visible
//to other functions in the same file 
//(more precisely the same translation unit)
static int multiply(int a, int b)
{
	log("multiply");
	return a * b;
}

int main()
{
	std::cout << multiply(5, 8) << std::endl;
}