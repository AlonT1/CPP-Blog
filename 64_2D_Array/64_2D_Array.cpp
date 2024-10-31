
/*
the important takeaways:
1. int*** x; can be better read as "int** *x" meaning x is a pointer to an int**,
thus it can be assigned with an int**
(in reality though all three "*" are a type modifier that belongs to the declarator "***x"

2. a pointer to X can be assigned with:
	1. address of X (single instance) (actually - the 1st byte of X)
	2. address of array of X (actually - the 1st byte of the array)
	3. a pointer of the same type (copy assignment of the value (memory address that another pointer "holds")
 	(because that pointer points to either (1) or (2))

	note: 1 and 2 are the same since in concept - in c/c++, the address of an array represents
	the byte of the 1st object in that array, therefore in both cases we point to the 1st byte of an object of
	type X - only in 2 the following objects in memory are also of type X.
	(stack allocated - contiguous, if heap - potentially fragmeneted).

	e.g: "int** x" (x points to a pointer to an int) can be assigned with:
	1. address of int* (single instance)
	2. address of array of int* (the address of the first element of the array)
	3. a pointer of the same type (int**)

	example in diagram (left represents bullet 1, right represents bullet 2)

	int** x = new int*(5)		int** x = new int*[3]

		set1    set2				set1    set2   set3(ints)
	*----->	* ----> 5     or         *----->* ----> .... 
											* ----> .... 
											* ----> .... 

		same goes for "int* x" (x points to an int) which can be assigned with:
		1. int
		2. array of ints
		3. a pointer of the same type (int*)


3. int***...* x:  x itself will always be a single pointer.
for example, in the following diagram representing "int** x",
x represents the first (leftmost) pointer and it will always be single.

set1    set2    set3(ints)
*----->	* ----> {1,2}
		* ----> {3,4}
		* ----> {5,6}

dereferncing x - "*x" will lead us to the object whose address stored in x -
the address of the first pointer in set2 -  thus retrieving the first
pointer of set 2.
(set2 can contain an array of pointers or a single pointer as explained above).

Bonus: dereferencing x twice "*(*(x+1))":
*(x+1): will retrieve the object whose address stored in x (which is a pointer to an int*)
- retrieves the 1st object in set 2, but since we increment by 1 we get the 2nd object in that set,
which is the 2nd pointer to an int - can store a single/array of ints).
b."*(*(x+1))":  now we have the 2nd pointer from set 2 - we dereference (outer parenthesis)
this pointer and retrieve element 3 (1st element of {3,4}).


4. types such as "int****....* x" DOESN'T NECESSARILY IMPLY MULTI DIMENSIONAL ARRAYS!
for example "int** x" isn't automatically means a 2D array - 
"int** x" simply means that x is a pointer to a pointer.
for example:
a. int** x can point to null.
b. int** x can to a 1D array of int* - then int** x will be equivalent to "int* arr [4]".
assuming that in both statements "int** arr;"  and "int* arr[4]" the int pointers in 1D array
point to some int, both of these ints can be accessed via arr[0...][0] where
[0...] is the inner asterik and [0] is the outer asterik in the following expression: *(*(arr+0)+0) 
the inner asterik dereferences the first pointer (from set1) that leads us to some n pointer in the 1D array (set 2),
the second asterik dereferences that pointer and retrieves the int from set (3) pointed by the pointer from set 2

set1    set2    set3(ints)
*----->	* ----> 1
		* ----> 2
		* ----> 3

This type of arrays are popular with files or nodes, for example: File** files - 
which isn't exactly a classic 2D array (a table/matrix), but rather can be thought of as a 1D array
of file pointers where each file pointer in that array is being "new"ed up with a file.
Of course one might think of these arrays as a 2D array with multiple rows and one column,
and they can even be dereferenced via [0...][0] syntax, but the prespective of "1D array
of pointers" instead of 2D array with multiple rows and one column" is important to have - 

Seeing File** files might imply a dense matrix of files ("proper" table) where each pointer
from the 1D array points to an array of files, or simply a 1D array of files where each File pointer
from the 1D array points to a file.


c. of course as shown in the previous example, int** x can be a "proper" 2D array,
with dense x and y dimensions.
set1    set2    set3(ints)
*----->	* ----> {1,2,4,5}
		* ----> {3,4,7,8,9}
		* ----> {5,6,5,3}


Notes:
1. int x{ 5 };
int* ptr { &x }; //works because &x returns a pointer to "x", which is the same type as "ptr"
int** ptr1 { &ptr } //&ptr returns a "pointer to a pointer" "ptr", the same type as "ptr1"

2. 	
// "carrots" is a pointer to an array of 4 doubles.
// in the example below, it points to the first element of the [3][4] array, 
// which is array {1,2,3,4} (indeed array of 4 doubles)
// the fact that the 2d array [3][4] has 3 elements (each element is array of 4 doubles),
// does not affect "carrots" pointer, which simply points to the first element of the array {1,2,3,4)},
**********************************************************************
// it doesn't need know about the other 3 elements, it only needs to be aware about the size
// of the immediate array it points to!!!! (because cpp is row-major order, thus the memory
// is traversed by column (1 then 2 then 3,.....)
**********************************************************************

double(*carrots)[4]{ new double[3][4] {{1,2,3,4}, 
									   {3,4,5,6}, 
									   {1,4,6,2}} };

for derefercing: "carrots[2][3]" or  (*(*carrots+2)+3) - dereferncing
the pointer (address) to the 2nd row, and from there we add 3 to the dereferenced pointer(address)
so we could dereference the 3rd element

//equivalent to above
double** carrots2 = new double* [3];
for (int i{ 0 }; i < 3; ++i) carrots2[i] = new double[4];


3. c/cpp is a row-major order, meaning that it stores  2d array contigously (on stack,
with heap, a.k.a free store, contgious storage isn't guaranteed), in the following
manner:

if we have the following 2d array:
a11	a12	a13
a21	a22	a23
a31	a32	a33

row-major order (c/c++) stores data in this order: a11, a12, a13, a21, a22, a23, a31, a32, a33
column-major order	(matlab) stored data in this order: a11, a21, a31, a12, a22, a32, a13, a23, a33


*/
#include <iostream>

int main()
{
	/*************************1D Array (stack allocated)*******************
	"arr_stack" is an identifier for a stack allocated array stored contiguously in mem.
	the array is stored directly inside "arr_stack" object (=variable).*/
	int arr_stack[4] = { 1,2,3,4 };

	/***************************1D Array (heap allocated)***************
	first, lets look on the lhs - "int* array":
	
	"array" is an identifier for an object of type pointer to an int.
	this means that "array" can be assigned with the following:
	1. address of an int (stack or heap allocated)
	2. address of an int array (stack or heap allocated)
	3. already existing pointer to an int (pointers are always allocated on the stack)

	AN IMPORTANT NOTE:
	1 and 2 are the same! an address of an int array is essentially the address
	of a single int that serves as the first int in an array


	Notes:
	1. *the decl-specifier-seq is "int"
	the decl-initializer-list (declerator) is *array. * is a type modifier for
	the underlying type int. together int* (pointer to an int forms a compound type).
	see (30a.

	2. The "new" expression performs the following steps in one-go:
		a. allocates (mallocates) a chunk of memory at the size of 50 ints using the "new" operator (function)
		that returns a void* pointer.
		b. if int was a user-defined type (struct, class...) the new expression would have
		caused the construction of 4 instances at that location, but since int is a pod
		this space is skipped
		c. the void pointer from step a is implicitly cast to int*, returning a pointer
		to the address of the 1st int in the array.
	*/
	int* ptr_array_heap = new int[4]; //pointer to an int - allocating 50 pointers to an int
	for (int i{ 0 }; i < 4; ++i) ptr_array_heap[i] = 0 ; //alternative" *(arr+i) = 0. must init the array
	
	int* ptr_array_stack = arr_stack; //the arr_stack is decayed to a pointer to an int* (16c_decay_

	
	/*************iteration via stack vs heap allocated array
	to iterate througn a array with a pointer we simply add +1 to the the pointer (stores the
	address of the int) to iterate the array. the compiler knows that the size of an int is 4 bytes
	(depending on the architecture), thus when we add +1 the compiler moves 4 bytes forward
	to reach the next int. a stack allocated array is contiguous, an heap allocated array can
	be defragmented across the memory - slower than stack arrays - but code-wise the compiler
	knows in both cases how to reach the next int in both cases with the same syntax
	*/
	for (int i{ 0 }; i < 4; ++i)
	{
		std::cout << *(ptr_array_stack + i); // or ptr_array_stack[i]
		std::cout << *(ptr_array_heap + i); // or ptr_array_heap[i]
	}
	

	/*************************************2D ARRAY*****************************
	stack allocated:*/
	int array2d[3][2]{ {1,1}, {2,2}, {3,3} }; //3 columns and 2 rows



	/*heap allocated:
	All the concepts that apply to 1D array apply here also, albeit a new concept
	is revealed with a 2D ARRAY:

	int is a pointer. now comes the interesting part. what does he points to?
	it points the address of a pointer to an int! in other words, int** can be assigned with:
	1. address of a pointer to an address of a  single int 
	2. address of array of pointers to an int
	*1 and 2 are the same - in both cases int** will point to the address of pointer to an int,
	2 simply states this "pointer to an int" is the address of the first element in an array.
	3. another int** (copy assignment)
	
	the main point is that a statement like the following: "int**** x" can be
	better understood by writing: "int*** *x" - meaning that x is a pointer to something.
	to what? to an int*** object, therefore it needs to be assigned with with an int***;
	*/

	//a2d is a pointer to something. to what? to an int*, better understood as "int* *x".
	//a2d now points to 50 int*
	int** a2d = new int* [50];
	//since int* is a pointer to an int, we iterate through each of the int*
	//and point them to an int array (allocating 1d arrays)
	for (int i = 0; i < 50; i++)
		*(a2d + i) = new int[20];

	//after declaring ("this thing exists") and defining ("memory allocation") the a2d pointer, we will initialize
	//("giving initial value") its values.
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 20; j++)
			a2d[i][j] = 0; // equivalent to  *(*(a2d+i)+j) :
	//*(a2d+i) //deref the first pointer which points to 50 int pointers and retrive the ith of these pointers
	//*(*(a2d+i)+j) //after we retrieved the ith out 50 pointers, deref each of them (retrieves a memory block of 20
	//ints) and for each of the jth ints, assign it to 0


	/*************************************3D ARRAY*****************************
	"int** * a3d" a3d is a pointer to (can be assigned with) a pointer to a pointer
	to an int*/
	int*** a3d = new int** [50]; 

	//allocation
	for (int i = 0; i < 50; i++)
	{
		*(a3d + i) = new int* [20]; //deref outermost pointer - a3d[i] will now point to a location which stores 20 pointers
		for (int j = 0; j < 20; j++) //iterate throuhg each of those 20 pointers, and point them to a memory block of 20 ints
			*(*(a3d + i) + j) = new int[20]; //equivalent to a3d[i][j]
		//*(a3d+i)- dereferences a3d (now we are at the ith location in memory - the ith pointer out of 50)
		//*(*(a3d+i)+j) - now we are acessing the ith out of 50 pointers, reaching the j pointer out of 20
		//each one of these 20 pointers now points to a memory block of 20 ints

		//*(*(*(a3d+i)+j)+k)  == a3d[i][j][k] - ith pointer out of 50 that points to the jth pointer out of 20
		//that points to the kth int


	}

	/*Delete - we have to delete the heap allocated arrays one by one - we can't delete the outermost pointer:
	this will lead to a memory leak, since we will lose the pointer to the pointers it points to,
	and therfore we will lose the abillity to delete those pointers because we cannot longer deref,
	the outermost pointer! leaving them and the data they
	point to as leaked in memory, in other words, we will lose the abillity to communicate with them, specifically delete them.
	the pointers a2d, a3d are pod stack allocated data, and will be removed. after this. using "delete"
	removes the contents of the pointer! as a pod "delete" doesn't remove the pointer itself, it will be
	deleted at at the end of the scope as stack based pod with auto storage specifier.
	without explicitly using "delete", even if we reached the end of the scope, only the outermost pointer will be 
	deleted which will cause a memory leak for the reasons stated above (losing communication with the pointers it points to). 
	deleting occurs from the most nested array up until the outer layer*/

	//deleting 2d array
	for (int i = 0; i < 50; i++)
	{
		delete[] a2d[i];
	}

	//deleting 3d array
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			delete[] a3d[i][j];
		}
		delete[] a3d[i];
	}

	/***************Simulating 2D array with 1 dimentional array***************************
	array of arrays results in memory fragmentation - instead of one contiguous memory buffer
	for 25 ints, heap allocated 2d of 5x5 allocates the same memory but each 5 ints are stored
	in completely different place (might be close together). thats an issue because when we
	iterate through them we have to jump to another location in memory - may result in a cache
	miss when fetching the nums from the ram (pointer over head). single dimension array may be faster!
	this is a memory access optimizations.

	instead of multidimensional arrays we can simply use 1 dimensional array - a lot faster!!!
	images can be represented as a 2d array but we can store it in a 1d array - much optimal and faster!
	*/

	//creating 2d
	int** arr2d = new int* [5];
	for (int i = 0; i < 5; i++)
		arr2d[i] = new int[5];

	//filling 2d
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			arr2d[i][j] = 3;


	//creating 1d 
	int* arr1d = new int[5 * 5];
	//filling 1d
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			arr1d[j + i * 5] = 3; //every time i increments we go forward 5 cells forward (dropping row down)
	//also called scaled-index-byte (SIB)

}

