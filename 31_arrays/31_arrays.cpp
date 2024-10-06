#include <iostream>
#include <array>
/* represent a collection of variables. [] - index
store data in contiguously in memory - in a row
*/

class Array // improves the dynamic array by offering a way to retrieve its size (int* a = new int[5], sizeof(a) retrieves sizeof int pointer...)
{
public:
	int* example;
	int size;
	static const int examSize = 4;
	int exam[examSize];
	Array(int size)
	{
		this->size = size;
		example = new int[size];
		for (int i = 0; i < size; i++)
			example[i] = i;
		for (int i = 0; i < examSize; i++)
			exam[i] = i; // == *(exam+i) = i
	}
};

class Entity
{
public:
	int x = 2;
	int y = 3;
};


// 1D arrays decay to pointers! equivalent to "void func1(int arr[])"
void func1(int* arr)
{
	std::cout << *(arr + 1); // == arr[1]
}


// 2D stack arrays (e.g: arr1[3][4]) decay to a pointer to array, not pointer to pointer
void func2(double(*arr)[4]) // == "void func2(double arr[][4])" // column size mandatory - see below
{
	std::cout << arr[2][1];
}




int main()
{
	/*
	arr[i] ==  *( ( arr ) + (i) )
	In cpp, array = collection of elements of the SAME data type (at declaration) stored in contguous memory locations
	When allocated on the stack (either in main, or inside a function), The size of an array is fixed
	at the time of declaration and cannot be changed during runtime. Its allocation is done
	at compile-time (static)  on the stack (for static, compile-time vars), Note that we can actually
	take the sizeof this array and get 12 via std::cout << sizeof(arr1); due to 4 bytes (int) * 3 = 12
	because the compiler knows the size at compile-time.
	**********************************************************************************
	Think of int[3] as a new/distinct type, just like double, float, or a new class. Like stated below 
	it is a new type which is also a compount type, formed from pod and array type
	**********************************************************************************
	for example: */
	int arr1[3]{ 1,2,3 }; // equivalent to: int arr2[3] = {1,2,3} (= is assignment, {} is explicit init.) 
	// arr1 is a 1D array, 1x3 matrix, 1 row, and 3 columns (concept more evident below in 2D matrices)!
	std::cout << sizeof(arr1); // prints 12
	int size1 = 3;
	// won't work, size1 must be const, arr1 is only compile time, and size1 can be changed during runtime
	// for example by user input
	// int arr1[size1]{ 1,2,3 };
	// Recall that array lookup is done by adding multiples of sizeof(array[0]) to 
	// the address of the first element (see 19_structs for padding)

	/*On the other hand, arr2 is a pointer to a dynamically allocated array, specifically it points to
	"new int[5]" which returns a pointer to address of the 1st byte of the 1st integer
	among 5 contguous elements of ints.
	it iss not fixed at the time of declaration, and is allocated at run time on the heap.
	taking the sizoeof arr2 returns 8 bytes, because it stores a 64 bit address. But why?

	32-bit windows can use up to 4GB ram when a 32-bit processor and 32-bit os exists.
	because with 32 bits we have either 32 digits where each can be 0 or 1 (2 options), e.g:
	0000000000000000000000000000000000000000000000000000000000101001 // represents 42
	32 bits = 4 bytes = (1 byte = 8 bits)
	therefore we say that there are 2^32 = 4,294,967,296 unique locations, aka 4 billion
	combinations of 32 digits, or in other words 4.3 billion bytes, since each 32 bit
	mem address points to a byte.

	decimal system, base 10 -> 10 digits 0-9, e.g: 123 = 1*10^2 + 2*10^1 + 3*10^0
	base-2 system (binary) = 2 digits 0-1, e.g: 101 = 1*2^2 + 0*2^1 + 1*2^0 = 5 (in decimal)

	In the metric system (standard of SI international system of units), which is a decimal-based system: kilo = 1000
	remember that computers work in base 2, not decimal 1 kilboyte = 1024 bytes due to 2^10 =1024,
	not 1000 (10^3) (more accurately 1 kilobyte = 1000 bytes, and 1 KiB (kibibyte) = 1024 bytes).
	The term kibi = kilo + bytes ( introduced by International Electrotechnical Commission (IEC) to
	avoid confusion with metric kilos). So instead of 10^0=1, 10^1=10, 10*2=100, we get 2^1=2, 2^2=4, 2^3=8...
	But the story here isn't just an arbitrary decision, it is based on the fact that:

	When computers were first designed, it made practical sense to organize memory in powers of 2.
	Binary multiples of bytes align well with the addressing mechanisms used in computer architecture.
	Since 2^10 is the closest power of 2 to 1000 (aka KILO), it became the standard size for a kilobyte.

	1 byte = 8 bits - > 2^8 (8 digits each can be 2 options 0/1) = 255 possible values
	1 kilobyte = 10^3 = 1000 bytes, more accurately 2^10 = 1024 bytes
	1 megabyte = 10^6 = 1,000,000 bytes, more accurately 2^20 = 1,048,576 bytes
	1 gigabyte = 10^9 = 1,000,000,000 bytes, more accurately 2^30 = 1,073,741,824 bytes
	4 gigabyte = 4 * 10^9 = 4,000,000,000 bytes, more accurately 4 * 2^30 = 4,294,967,296 bytes
	hence we say that 32-bit os can point to ~4.3 billion bytes (unique addresses) = 4gb

	But when querying the integer (for example printing it), the compiler reads the
	4 bytes consequtively (thanks to to declared datatype). if it was a char, only the first byte
	would have been read.

	On a 64 bit machine we have have 2^64 = 18,446,744,073,709,551,616 unqiue mem locations,
	or up to 17,179,869,184 GB (16 exabytes) each points to some byte.
	each byte has a 64 bit memory address, and a 0-255 (2^8=256) value represented by the byte itself

	so this is why pointers are the size of 8 bytes (8bit * 8 = 64 bit), since they store a
	memory address to each byte, and when we dereference them we get the value at that address, which
	is in practice read by consequitvely reading the number of bytes (determined by type)
	at that location.

	So unless we stored beforehand the size of the array, the compiler has no way to know its
	size, because the ptr only points to the 1st byte of of the 1st int. The compiler doesn't
	know how many consequtive ints are there.

	*****************************************************************************
	automatically keeping track of the size of each dynamic allocation would impose additional overhead, 
	which goes against the C++ philosophy of “you don’t pay for what you don’t use”.
	So if you want to keep track of the size, save it to a variable, or make a custom class
	of array which allocates a pointer and has a size parameter.
	*****************************************************************************

	*/
	int* arr2 = new int[5];
	std::cout << sizeof(arr2); // prints 8

	/*
	unlike the static arr1 whose size cannot be changed at run-time, arr2 can change.
	To be exact, we don't expand the 5 element array into 10 by freeing up the consequtive
	5 elements in memory, but rather we move to array to the start of a whole new memory address.
	*/
	int* oldArr = new int[5]{ 1,2,3,4,5 };  // Allocate memory for the old array
	int newSize = 10;  // varying size is possible in runtime arrays! unlike the static arr1
	int* newArr = new int[newSize] {0};  // Allocate memory for the new, larger array
	// Copy elements from the old array to the new array
	for (int i = 0; i < 5; i++) { newArr[i] = oldArr[i]; }
	delete[] oldArr; // must deallocate old array!
	/*
	both arr1 and arr2 are compound data types: are types that are
	defined in terms of other types. Some examples of compound data types in C++ include:
	Arrays, Pointer types (e.g., Pointer to object, Pointer to function),
	Reference types (e.g., L-value references, R-value references)
	Enumerated types (e.g., Unscoped enumerations, Scoped enumerations)
	Class types (e.g., Structs, Classes, Unions).
	These compound data types allow for the creation of more complex data structures
	arrays allow grouping of elements, ptr/refs enable indirect mem access, classes/structs,
	enums provide fixed set of named values.*/

	/*
	Note that Both stack-allocated arrays and heap-allocated arrays are stored
	contiguously in memory(!!!). But they do differ in:
	1. where the memory is allocated - stack or heap
	2. who manages the memroy - arr1:the os (stack), arr2:programmer(heap)
	3. the lifetime - arr1:end of scope, arr2(software termination) - the pointer
	itself is on the stack (os-removed), by the memory allocation itself will survive the scope,
	we must deallocate using "delete", o.w mem leak.
	4. how it is accessesd:

	and the knowledge we have
	about them at compile-time.
	*/


	delete[] arr2;


	// both are equiavelnt, one uses = and the other {} ("explicitly initialization",
	// while the other is assignment, which can happen at every stage);
	int a[3] = { 1,2,3 };
	int b[3]{ 1,2,3 };

	//  2D Array on stack
	double carrots2[3][4]
	{
		{1,2,3,4},
		{3,4,5,6},
		{1,4,6,2}
	};

	/*
	just like a 1D array, we can take the size of this array, i.e double[3][4] is a distinct type

	1. [3][4] is RC (row, columns), a matrix (see intro_to_cg_2.docx - matrices demystified)
	Where we have 3 rows: {1,2,3,4} is row1, {3,4,5,6} is row2, {1,4,6,2} is row3.
	{1,3,1} is column1, {2,4,4} is column2, {3,5,6} is column3, and {4,6,2} is column4.

	2. in C, Arrays are stored in a row-major order in terms of memory layout - doesn't effect the programmer
	Elements of each row are stored consecutively, one after another. e.g:
	  0   1   2   3   4   5   6   7   8   9   10  11  -> 12 elements
	| 1 | 2 | 3 | 4 | 3 | 4 | 5 | 6 | 1 | 4 | 6 | 2 |

	3. Are cpp matrices column/row major in terms of math notation? purely depends on implementation:
	we can implenment vector-matrix multiplication as a dot between vector and columns (then the matrix and
	vector would be considered row-major), or as  dot between rows (of the matrix) and the vector
	(then the matrix and the vector would be considered as column-major).

	4. the above can be written as "double carrots2[][4]" and in-fact the number of columns is essential
	when passing the 2D array to a function. Moreover we can even write:
	*/
	double arr1[][4] = { 1,2,3,4,3,4,5,6,1,4,6,2 }; // compiler will deduce a [3][4] array
	
	/*
	Why the column size is more important than the row size?
	with the form above, it is clear that the only way we can derive 4 columns is by:
	double carrots2[3][4]
	{
		{1,2,3,4},
		{3,4,5,6},
		{1,4,6,2}
	};
	from the number of columns the compiler can deduce the number of rows

	Only specifying 3 rows "double arr1[3][] = { 1,2,3,4,3,4,5,6,1,4,6,2 }";  can be interpreted as jagged:
	double carrots2[3][5]
	{
		{1,2,3,4,3},
		{4,5,6,1,4},
		{6,2}
	};

	*************************************************************************************
	We can think of a 1D array elements "int arr1[3]{ 1,2,3 };" as elements in single row
	where each element forms a column
	*************************************************************************************
	*/

	/****************************array decay****************************
	Ptr “decay” = process where an array name is implicitly converted into a pointer to the 1st byte of its first element. 
	aka “array-to-pointer conversion”.  The term “decay” signifies a loss of :
		1. Type Information: An array in C or C++ has a specific type that includes the type of its elements and its size. 
		For example, int arr[10]; is of type int[10]. When arr decays to a pointer, it becomes int*, 
		and the information about it being an array of 10 integers is lost.
		2. Dimension Information: For multi-dimensional arrays, decay results in loss of information
		about dimensions other than the first. For example, if you have a two-dimensional array int arr[10][20];
		it decays to a pointer of type int (*)[20], a pointer to an array of 20 integers
		. The information about the first dimension (10) is lost.
	The concept of decay is deeply rooted in the history and design of the C language. It was inherited from B and BCPL,
	the predecessors of C, where arrays were implemented as physical pointers pointing to independently allocated
	blocks of memory containing the actual array elements. This design choice was primarily for efficiency reasons
	as copying arrays that are passed as arguments would be expensive and unnecessary most of the time.
	Unlike arrays, structs/classes do not decay to pointers, which allows them to be passed by value.
	However, this leads to inefficiencies when working with large structs.

	scenarios of decay: arrD will decay when printing/assignment/passing to functions/expressions (arrD+5)
	although the following print the same memory address - they are not the same type!
	*/

	int arrD[3]{ 1,2,3 };
	int* ptr1 = arrD; // decay to "int*"
	int (*ptr2)[3] = &arrD; // ptr to "int*[4], parenthesis are must
	/* the difference between the types:
	ptr1 and ptr2 both point to the 1st byte of the 1st element of arrD.
	but pointer arithmetics work differently: ++ptr1 will take us the 1st byte of the 2nd elemnt
	(aka the 2nd element of the array), while ++ptr2 will take us to the 1st byte of the next 4 element
	of the array (remember that the compiler knows "where" to increment the pointer due to the type),
	so in the case of
	int arr[][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
	int (*ptr2)[4] = arr;
	++ptr2;
	std::cout << (*ptr2)[0] << std::endl;  // prints 5

	when decay doesn't occur: sizeof(arrD), &arrD (returns ptr to int[4]), typeid(&arr5).name() (prints PA3_i
	aka point to array of 3 element_integer), unlike typeid(arr5).name() (prints A4_i)
	*/
	 

	/*********************2D array on heap - only pointers**************************
	carrots is a pointer to array of 4 doubles.
	arr4 points to (stores the address of) the 1st byte of an int pointer. So it happens that
	there are 2 more consecutive int pointers (3 int pointers (rows!) in total)*/
	int** arr4 = new int* [3]; // == malloc(3 * sizeof(int*)) allocate and instantiate 3 int pointers
	// each 3 pointers now point to (store the address of) the 1st byte of a unique integer. 
	// So it happens that after each unique integer, there are 3 more consecutive integers.
	for (int i = 0; i < 3; ++i)
		*(arr4 + i) = new int[4];

	// populating the array
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			*(*(arr4 + i) + j) = i + j;

	// printing the array
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			std::cout << *(*(arr4 + i) + j);

	// delete the 3 inner pointers first
	for (int i = 0; i < 3; ++i)
		delete[] arr4[i]; // == free(arr4[i])
	delete[] arr4; // delete the oute rpointer == free(arr4)



	/***********************2D Array - alternative syntax*********************************/
	/*
	arr5ptr is a pointer to an array of 4 doubles, in this case it points to arr4.
	But &arr5 returns a pointer to the 1st byte of an integer amongst 4 consecutive integers.
	And we also know that arr decays(equals to) &arr.
	thus when we say that arr5ptr is a pointer to an array, we might as well say that
	arr5ptr is is a pointer to a pointer (because stack arrays  decay to pointers!).
	More explicitly we can say that arr5ptr is a pointer to 4 pointers (=arrays decayed to pointer).
	*/
	double arr5[4] = { 1,2,3,4 };
	double(*arr5ptr)[4] = &arr5;
	std::cout << arr5ptr[0][0];

	/*carrots is a pointer to a distnct type double[4] (a new type, just like a custom struct)
	when incrementing ++arr5ptr, the compiler, because of the type, will increment the pointer
	to the next set of 4 doubles, aka to the 1st byte of the 1st element in {3,4,5,6}.
	carrots just like any other pointer can point to the 1st byte of an element, and there maybe
	infinite amount of contiguous elements stored after the element we point to. the 1st element
	carrtos point to is {1,2,3,4}, this is also the type of the of pointer double[4].
	the next element stored contiguously is {3,4,5,6} and so forth.
	on the rhs, we allocate 3 such elements - each element is a row. within each row, each
	element represents a column of a single element.
	*/
	double(*carrots)[4] = new double[3][4]  // 3 can be implicit, compiler will deduce num of elements
	{
		{1,2,3,4},
		{3,4,5,6},
		{1,4,6,2}
	};
	// same as : (above = initlaization, below is with brace-initialization,)
	// double(*carrots)[4] {new double[3][4] {{1,2,3,4}, {3,4,5,6}, {1,4,6,2}}};


	// equivalent to above (a lot more popular way) - 1st we initialize the outer set of ptrs
	// and then move "inwards" to the inner set of pointers
	double** carrots3 = new double* [3];
	for (int i{ 0 }; i < 3; ++i)
		carrots3[i] = new double[4];
	std::cout << carrots[2][2];


	/*
	stack vs heap
	The stack is generally faster than the heap for memory allocation and deallocation,
	as it involves simple pointer manipulation and automatic management by the compiler.
	On the other hand, heap allocation provides more flexibility and is suitable for
	handling larger data structures.*/



	//arrays are a data type  (we can take their size), they store data contiguously.
	//they decay into pointers when passed to functions, losing the "array-ness".
	//arrays aggregate data, while pointers just point to a location in memory (taking their size yields the size of a ptr.
	//(8 bytes in 64 bit systems because in 64 bit systems memory addresses are 64 bit wide).
	//"example" yields the address of the first element of the array which is also considered as the address of the array.
	//example[0] yields the value stored at that address. &example and example yields the same result (address).

	int example[5]; //array of 5 ints allocation for 5 integers - stack allocated and "static", meaning its size cannot be changed!
	example[0] = 2; //access  and set
	example[4] = 4;
	//std::cout << example[0];
	//std::cout << example[-1]; // memory access violation - error doesn't belong to me -
	//in release mode its possible - possible to override a memory that exists elsewhere!, in debug- not. 
	//std::cout << example; // this is a pointer type! the adress of the array

	int size = 5;
	//Error! size can be manipulated and changed during run-time, meaning that 5 isn't necessarily
	//"size"'s "final" value. This collides with the fact that stack-allocated arrays are constructed
	//during compile-time, therefore the compiler raises an error, telling us that the value that "size"
	//holds during compile-time maybe modified during run-time, thus it cannot be trusted to allocate
	//the size of the array during compile-time. Therefore "size" must be const or constexpr to solve
	//this issue.
	//int array3[size]; 

	// possible to write <=4 but <= is more expensive than < (two actions vs one),
	//more readible since our array size is 5
	for (int i = 0; i < 5; i++)
		example[i] = i;

	int* ptr = example; //example is an integer pointer
	*(ptr + 2) = 16; // equals to example[2]  +2 means to elements forward in memory

	//e.g: value of ptr (=of array example) - points to this address- 0x008FFC50 - elements in the row are 00000001(hex- represents 32 bits or 4 bytes, each hex,e.g 01 is 4 bits * 8 hexes ) (1st element) 00000002 (hex) ...
	//pointer arithemtics: the number of bytes added depened on the type! int pointer going to add 2*4 sizeof(ptr) is 4!
	//(two elements up * 4 bytes becuase 4 is thesize of each integer) leads us to the second elemnt in memory (8 bytes forward (4bytes(num1), 4bytes(num2), 4(bytes)(num3)<- the target!)
	//*(ptr+x)  == sizeof(ptr)*x <- this is the math
	std::cout << *(int*)((char*)ptr + 8); //char is 1 byte large * 8  = going 8 bytes forward (2nd element) will lead us to the same location
	//+ cast back to int* so we can tell back the compiler that there is an int there so we can dereference it
	//*(int*)((char*)ptr + 8) == *(ptr+2) // due to pointer arithmetic
	//Note: typcasting creates 
	//order of evaluation -
	//1.((char*)ptr + 8)  the outer parenthesis win! now going in
	//2. (char*) typecast  and + operator have the same precedence - evaluated together
	//3. the result is then typecasted via (int*) and then dereferenced via * operator  (typecast and dereference have the same precedence)

	int* another = new int[5]; // created on the heap ***no way to work out the size of the array!
	//... there is a way to know because since delete commands works somehow - it is compiler dependant
	//this type of arrray is also "static" meaning its size cannot be changed
	delete[] another; //array allocated (new int[5]) requires delete[]
	//same above rules apply


	int x = 2;
	int* x_ptr = &x;

	Entity e_stack; // <- this is a variable which holds the class's data! all stack allocated
	Entity* e_heap = new Entity; // <- this is a variable of type pointer which it's data is the memory adress of where the class data is located on the heap


	/*why allocate on the heap and not the stack? lifetime - stack is bound to scope, heap will always be around until deleted
	when ever possible - better to create on the stack to avoid indirection overhead. dereferncing a pointer costs performance since we need to get the adress (value of pointer e_heap)
	and than jump to the memory location of that address, instead of a variable that its values are directly the data itself (e_stack)

	*/

	int a[5]; //we can find the size since it is allocated on the stack - the stack pointer contains the offset
	int count = sizeof(a) / sizeof(int); //20 (5*4)  count -num of elemnts , size - size in bytes
	sizeof(another); // retrieves size of integer pointer!


	int sizeArr = 4;
	//int arr[sizeArr]; error! when we allocate array on the stack it must be in a size of a compile time known constant - not a variable!

	const int sizeArr1 = 4; //correct! it is a const
	int arr[sizeArr1];

	// arrays in cpp11 library - offers bounds checking size of the array.
	std::array<int, 5> test2;
}
