#include <iostream>

/*brace initialization or uniform initialization (cpp 11+) is a feature in c++11 that allows
conistent way to initialize variables and objects.

uniform initialization can be used in two ways on objects
1. direct-list initialization - initialization of a named variable with a braced-init-list {}
2. copy-list initialization - initialization of a named variable with a braced-init-list after an = sign
compiler-wise they are both evaluated to the same expression as seen below.
*/


int main()
{
	/*******************uniform initialization on primitives************************

	Uniform initialization (performed via {}) warns against narrowing 
	(as opposed to direct initialization - performed via (), 
	which doesn't warn against narrowing). for example:

	1. char can be assigned to an int, but int cannot be assigned to a char,
	since char may not be capapble of holding the value of the int(overflow)
	int a = { 'a' }; allowed   char a = { 128 } not allowed overflow -> this because char a is
	implicit unsigned char meaning the msb is denotes + / -
	since a char is 8 bits, the 8th bit is for the sign, and the rest of the 7 bits allow
	for a number ranging from 0 - 127, with the 8th bit
	we gain - 127 to 127 number range. this is why char a = { 127 } is allowed!

	2. float(4 bytes) can be assigned to a double(8 bytes), but not the 
	opposite(causes overflow), same principle as in(1)

	3. floating point cannot be converted to an int - > int a = { 4.2f }; error!
	Entity e2(3.8, 4) is ok but Entity1 e2 { 3.8, 4 }; notifies narrowing of
	a float(3.8) to an int - nice!
	
	4. using empty uniform initialization, initializes the object with default/null values,
	for example "int x{}" - x is assigned with 0, as oppposed to "int x;" where
	x may be initialized with garbage values
	*/

	
	int x2 = 3.4; // causes narrowing - x2 becomes 3
	 // error -  this one notifies an error of narrowing!! (conversion of floats to int,warns against truncation)
	//int x3{ 3.4 };

	int x3{ 4 }; //direct list initialization
	int x4 = { 4 };// copy list initialization

	std::cout << x2 << std::endl;


	/* If the object is an array of unknown size, and the supplied brace-enclosed
	initializer list has n clauses, the size of the array is n.  */
	int arr[] = { 1,2,3 };

	/*******************uniform initialization on objects*************************/
	//uniform initialization can be used in two ways on objects
	// 1. direct-list initialization - either of a class via ctor or an aggregate
	// also called aggregate initialization 
	// aggregate - class/array with no private/protected/static members"
	struct Vector2 { int x; int y; };
	Vector2 vec2{ 1,2 }; // aggregate direct list initialization

	struct Vector3
	{
		int m_x, m_y, m_z;
		Vector3(int x, int y, int z) : m_x{ x }, m_y{ y }, m_z{ z }{}
	};
	Vector3 vec{ 1,2,3 }; //direct list initialization via ctor

	//2. copy list initialization of an aggregate
	Vector2 vector2 = { 4,5 };
	//copy list initialization
	Vector3 vector3 = { 1,2,3 };

	/*******************************************
	both:
	Vector3 vector3 {1,2,3};
	Vector3 vector3 = {1,2,3};

	are evaluated by the compiler to the same statement:
	Vector3 vector3 = Vector3{1,2,3};
	which can brace-initialize aggregates, or invoke the ctor of exists + narrowing protection, more modern.
 
	while  Vector3 vector3 (1,2,3) is evaulated to:
	Vector3 vec3 = Vector3(1, 2, 3);
	which simply invokes the ctor without the benefits above
 	
	********************************************/
	

	/**************Uniform initialization solves vexing parse*********************/
	struct B { B() {} };
	struct A { A(const B& b) {} };
	B b(); //parsed as a forward declaration to a function called b taking no parameters and returns B
	// can be read also as a creation object b of type B.
	A a(B()); // compiled as "A a(B(*)())" (cppinsights) which means that we are declaring a function "a" that returns A
	// and takes in a function pointer to a function that takes nothing and returns B

	//uniform initialization solves this:
	B bb{}; //creating an object bb of type B! no more parsing as a forward declaration
	A aa{ B() }; // creating an object aa of type A whose ctor takes an annonymous B() object ! 
	// no more parsing as a forward declaration

	//*pre cpp11 the way to overcome the vexing parse was with extra paranhesis:
	A cc((B())); // instead of "A cc(B())", the extra paranthesis imply that this is not a function declaration
	// but a construction of an object cc with a ctor that takes in annonymous B()

}
