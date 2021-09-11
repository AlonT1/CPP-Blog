#include <iostream>

int main()
{
	int x = 6;
	bool comparisonResult = x == 5;
	if (comparisonResult)
	{
		std::cout << comparisonResult << std::endl;
	}

	//cpp 17 has new version for if switch statement that support initializing statements:
	//if (init; condition) and switch (init; condition)
	if (int x = 4; x < 4) //int x=4 is the init,  x<4 is the condition
	{
		//code
	}
	//this is very useful because x is scoped and won't collide with other x's in the program

	//another example
	const std::string_view str = "hello";
	if (int pos = str.find("e"); pos != std::string::npos) //npos means not found (-1)
		std::cout << "char found at position " << pos;



}

