#include <iostream>


int main()
{
	// ***************Given a Union:
	union Data // union is a keyword used to defined a user-defined type "Data"
	{
		int i;
		float f;
	};
	/* The union occupies the size of the biggest member (float) as potentially all members can utilize this space (one at a time).
	The member that is actually stored in the allocated memory is the last assigned member*/

	union Data data; // instantiating Data type. "union" keyword mandatory in C, not in CPP (see "struct")
	data.i = 10; // data union now stores 10 (int) in the allocated memory. data is now INTERPRETED as int.
	data.f = 22.5f; // data union now stores 22.5 in the allocated memory. data is now INTERPRETED as  float.
	// as data is now a float and both data.i and data.f occupy the same memory, data.i prints garbage value (1102315520)
	// (since 10 is interpreted as a float)
	std::cout << data.i << '\n'; 
	

	// ****************unions can be annonymous just like a struct and be stored in a struct
	struct MyUnion
	{
		union // annonymous union
		{
			float a; 
			int b;
		};
	};

	MyUnion u;
	u.a = 2.0f; //now the union struct is dominated by float! accessing u.a gives correct result!

	// ******************scoped non annonymous union
	struct MyUnion1
	{
		union NamedUnion // annonymous union
		{
			float a; 
			int b;
		};
	};

	MyUnion1::NamedUnion nameUnion; // scoped to MyUnion1, but not related to a MyUnion instance
	u.a = 2.0f; //now the union struct is dominated by float! accessing u.a gives correct result!

	// ************************with instance
	struct MyUnion2
	{
		union NamedUnion // annonymous union
		{
			float a; 
			int b;
		}nameUnion; // instance of NamedUnion
	};

	MyUnion2 myUnion2;
	myUnion2.namedUnion = 5;


	// *******************union with multiple structs
	struct Vec2 
	{
    		float x, y;
	};
	
	struct TheUnion
	{
		union // annonymous union
		{
			struct // annonymous struct
			{
				float x, y, z, w;
			};
			struct
			{
				Vec2 a, b; // "a" stores 2 floats and "b' stores 2 floats
			};
		};
	};

	TheUnion theUnion; // size of Vec2 (as it is the biggest member)
	// "vec2 a" (stores 2 floats) is aligned with "float x,y" so the following will effect xy (x=5, y=6)
	theUnion.a = {5.0f, 6.0f};
	// "vec2 b" (stores 2 floats) is aligned with "float z,w" so the following will effect zw (zw z=5, w=6)
	theUnion.b = {3.0f, 2.0f};
	

	/*---------------------------------------------------------
	in effect the union struct simulates a type of any value!!!
	similar to std::variant or std::any
	The union struct can be INTERPRETED either a float or an int on the fly, exclusively!!!
	u.a = 3.0f; -> now the union represents a float (u.b gives undefined result
	BECAUSE THE SINGLE MEMORY BLOCK STORES A FLOAT VALUE IN A FLOAT SIZED BLOCK)

	u.b = 2; -> now the union represents an int (u.b gives undefined result
	BECAUSE THE SINGLE MEMORY BLOCK STORES AN INT VALUE IN A FLOAT SIZED BLOCK)
	this works becuase the compiler knows to read up to 4 bytes (int) of memory out of the union's memory block
	(the size of the block is determined by the biggest union member)
	------------------------------------------------------------*/
}
