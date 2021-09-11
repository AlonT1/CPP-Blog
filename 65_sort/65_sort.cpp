
#include <iostream>
#include <vector>
#include <algorithm> //for std::sort
int main()
{
	std::cout << "Hello World!\n";
	std::vector<int> values = { 3,5,1,4,2 };
	//std::sort(values.begin(), values.end()); //with no provided predicate (comparison function to perform the sort)
	//the sort will be ascending 

	//std::sort(values.begin(), values.end(), [](int a, int b) {return a > b;}); //with provided predicate (function to perform the sort)
	//a>b means order by ensuring that the 1st number a is bigger than the 2nd number b (Descending order)
	//this function is similar to comparator in java

	
	std::sort(values.begin(), values.end(), [](int a, int b) //results in 2,3,4,5,1 (1 is pushed to the end)
		{
			if (a == 1) 
				return false;
			if (b == 1)
				return true;
			/*
			the section above means: if we take two ordered numbers and the 1st is not a 1
			and the second is 1, then take the second number (1) and push it to fhe front
			this applies to all pair of numbers, and causes 1 to be pushed to the end
			*/
			return a < b;// true when a < b -> a will be oredered before b
		});

	for (int value : values)
		std::cout << value << std::endl;
}
