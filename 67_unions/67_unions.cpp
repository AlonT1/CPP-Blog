/*
Unions - only occupy memory of 1 member of a time. if we have a struct with 4 floats
its size would be 4*4 = 16 bytes. a union can only have 1 member. so if we declare a union
with 4 floats a,b,c,d, the union's size will be 4. and when we try to change
the value of a,b,c or d we will change the value of all since they share the same
memory block. the size of the union is the size of the largest member.
in c, unions only work for pod's.

unions are used in type punning. view (66_type_punning). espsecially when we like two different names
the same variable. e.g: math vec3(X,Y,Z) can also be used for a color (R,G,B).
used annonymously without names
*/
#include <iostream>

struct Vec2
{
	float t, m;
};

struct Vec4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			Vec2 a, b;
			// a represents t,m which aligns with x,y (in the struct above)
			// b represents another instance of t,m that aligns with z,w (in the struct above)
		};
	};
};

std::ostream& operator<< (std::ostream& stream, Vec2 vec2)
{
	return stream << vec2.t << " " << vec2.m;
}

int main()
{
	std::cout << "Hello World!\n";

	// a & b occupy the same memory block, its size is the size of the biggest type,
	//which is float in this uniion (float is bigger then int)
	struct Union
	{
		union // the union
		{
			float a; 
			int b;
		};
	};

	Union u;
	u.a = 2.0f; //now the union struct is dominated by float! accessing u.a gives correct result!
	std::cout << u.a << " " << u.b << std::endl; //result: 2 and 1073741824 
	//since 1073741824 is a byte representation as a float, in other words,
	//we treate this "float memory block" as if it was an int.
	//both a and b share the same memory block

	u.b = 3; //now the union struct is dominated by int! accessing u.b gives correct result!
	std::cout << u.a << " " << u.b << std::endl; //result: 4.2039e and 2

	/*---------------------------------------------------------
	in effect the union struct simulates a type of any value!!!
	similar to std::variant or std::any
	The union struct can be either a float or an int, exclusively!!!
	and can "change" time on-the-fly.
	u.a = 3.0f; -> now the union represents a float (u.b gives undefined result
	BECAUSE THE SINGLE MEMORY BLOCK STORES A FLOAT VALUE IN A FLOAT SIZED BLOCK)

	u.b = 2; -> now the union represents an int (u.b gives undefined result
	BECAUSE THE SINGLE MEMORY BLOCK STORES AN INT VALUE IN A FLOAT SIZED BLOCK)
	this works becuase the compiler knows to read up to 4 bytes (int) of memory out of the union's memory block
	(the size of the block is determined by the biggest union member)
	------------------------------------------------------------*/

	Vec4 v4 = { 1.0f, 2.0f, 3.0f, 4.0f };
	std::cout << v4.a << " " << v4.b << std::endl;

}