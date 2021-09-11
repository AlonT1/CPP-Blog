#include <iostream>

static int x = 4;
int main()
{
	extern int x;
	std::cout << x;
}