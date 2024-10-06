/*
based on: http://unixwiz.net/techtips/reading-cdecl.html
http://users.ece.utexas.edu/~ryerraballi/CPrimer/CDeclPrimer.html

syntax of c decleration is of the form:
storage-class	type	qualifier	declarator	=	initializer;

storage-class: typedef, extern, static, auto, register
type: void, char, short, int, long, float, double, signed, unsigned, struct, union, typedef type
qualifier: const, volatile
declerator: *, (), [] 

cpp declerators:
1. * pointer to...
2. [] array of...
3. () function returning... Note: if (int) then read: function with int parameter returning...
Notes: 1. grouping parenthesis surround the variable name
		2.  the return type is always without parenthesis.
		3. spaces don't matter

the rules of reading declerations - this is how the compiler evaluates declerations: 
***********************************************************
1. start with variable name (identifier) and end with type name, e.g:  p is ..... int
2. go right when you can, go left when you must
***********************************************************/

long** foo[7];

/*
1. (rule 1) start with the identifier: name is a.... returned type, e.g: foo is a......... long
from now on we only use rule 2
2. going right, because we must:    foo is "array of"..... long
3. going left, because we must:		foo is array of 7 "pointer to"...long
4. going left, because we must:		foo is array of 7 pointer to "pointer to" long.
in other words, foo is an array which contains 7 pointer to pointers to long


******remember that parenthesis have the highest priority
int &arr[4] - arr is an array of 4 reference to an int (impossible to store references in a container)
int (&arr)[4] - arr is a reference to an array of 4 ints (possible!)
*/

int* p[5]; // p is an array of 5 pointers to an int (array p stores 5 pointers to an int)
class Entity {};
Entity const  * const ptr; // ptr is a const pointer to a const Entity
void(*foo)(int); // foo is a pointer to a function with int argument returning void
//**********NOTE: void is the return type which is never allowed to be in a parenthesis!
void* boo();  //boo 2 is a function void arguments returning an int pointer

int nums[2]{ 1,2 };
int (&arr)[2]{ nums }; // arr is a reference to an array of 2 ints (called nums)
//int& arr[2]; // arr is a array of 2 lvalue ints - ERRROOORRRRR! array of references are not allowed

void (Foo::* func_ptr)(int) = &Foo::func; // see 58_function_pointers
//void (Foo::* the_pointer)(int) means: func_ptr is a pointer to a member of a class Foo that is 
// a function that accepts int and returns a void

/********abstract declarators
c standard allows abstract declarators, types not associated with a name, used for casting,
or arguments, e.g: "sizeof(int*)" where int is an abstract declarator.

so where the name of the varaible goes in a type such as char*[] ? 
the rules :
1. find the innermous grouping parenthesis
name should be in:
1. right of the rightmost *
2. left of the leftmost []
3. left of all the lefmost ()

examples:
for the abstract decleration: char*[]  -> (unamed is an array of char pointers) ->  (with identifier: char* x[])
for the abstract decleration: void(*)() -> ( (unnamed) pointer to a function returning void)  -> with identifier: void(*p)()
	void(*(int, int, int))(int, int, int) -> (unamed) is a function (int, int,int) returning a pointer to a function
	(int,int,int) that returns a void -> with identifier: void(*p(int, int, int))(int, int, int) 
*/


/*
Note: the rhs is NOT A DECLARATION , but it is an expression
that is evaluated into a single value
 int offset_A=*(int*)(*(int*)pcv+4)
 therefore the rules above doesn't Exactly apply for reading expressions, for example
 in declarations * means "pointer to" but in expressions it means dereferncing
 But in general we can use the rules for navigation:

 1. start with variable name (if exists) - pcv in this case
2. go right when you can, go left when you must

 pcv is an address that is cast to a pointer to an int (the compiler will treat that location as
 if it stores an int), we add 4 (the compiler deduces 4 as 16 bits (4*4) because we add
 4 to a pointer to an int) and then this address is dereferenced, casted to an int
 and then dereferenced again

*/