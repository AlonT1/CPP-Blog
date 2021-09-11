/*
Array decaying means that, when an array is passed as a parameter to a function,
it's treated identically to ("decays to") a pointer to the memory address of the first element of the array
*/

#include <iostream>

void print(int *arr) // array decayed into a pointer. in effect we just passed a memory address to the first element of the array
{
	for (int i = 0; i < 4; i++)
		std::cout << arr[i] << std::endl; //or *(arr+i)
}

void print(int num) {}

struct X
{
	int x, y, z, w;
};

int main()
{
	//at declaration "arr" is an identifier for an object allocated on the stack
	//that stores 4 ints contiguously in memory. This is analogous to an instance "x" of 
	//a class that aggragates 4 integers. In other words, in concept, the sizeof(arr) and 
	//and the sizeof(x) is both 16 bytes (4 byte per int* 4 ints, dependes on the architecture)
	int arr[4] = { 0, 1, 2, 3 };

	//but during usage, that is to say, after declaration, c/c++ decays the object
	//identified by "arr" from an object that stores 4 ints, into a pointer to the address
	//of the first int in the array.
	//this is in complete contrast to the instance "x" which after declaration
	//doesn't decay and still packs 4 bytes in the object identified by "X"

	//for example:

	//prints the size of an int* (prints 8 bytes - because pointers store addresses - 
	//the address of the first int of the array - and theses addresses are 64 bit
	//long (bytes) on 64 bit machines)
	std::cout << sizeof(arr) << std::endl;
	std::cout << arr << std::endl; // prints the address of the first element of the array
	print(arr); // remember, arr is a pointer, therefore the accpeting parameter must also be apointer

	// the subscript operator (function) returns an lvalue ref (an alias) of the int in index 2.
	//print(arr[2]);


	/************Important Note: the decay is not permanent! we can still pass 
	"arr" to functions that accpet array as a parameter!
	example:

	int arr[2] {1,2};
	printElements(arr);

	// "int (&array1)[2]" - "array1" is a reference to an array of ints (read 30a).
	//note that it is mandatory to write how many elements are in the array
	void printElements(int (&array1)[2]) 
	{
	  int length{ static_cast<int>(std::size(array1)) }; // we can now do this since the array won't decay!
	}
*/

}
