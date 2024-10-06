#include <iostream>

//structs and classes in cpp are USER-DEFINED TYPES
//struct are public by default, classes are private - other than that they are identical!!!!
//methods do not take any space in the class!
//in terms of memory they are identical to functions and variables outside of a class.
//classes, like structs, are a neat way of organizing information with encapsulation and minimize code duplication,
//making programming easier and logical. the methods inside classes are just encapsulated function in a namespace, 
//and varaibles are stored in memory like inside a single type like
//a struct, inline one after another according to their order of declerations.
//it is perfectly viable to program without classes, they just make life easier in terms of organization.
//class A defines a new disctint type "A".
class A {}; //requires ";" at the end just like "int x; " requires ";" at the end - both are types

class Player //"class" is the "class-key" - can be "struct" or a "class". "Player" is the class-head-name
{ // the following block is called the "member-specification"

/*
The group of public members of a class are often referred to as a public interface.
Because only public members can be accessed from outside of the class,
the public interface defines how programs using the class will interact with the class.

*/
public: //access specifer
	int x = 1, y = 1; //default member intiailization - instead of initializing them via ctor
	int z = x; // in cpp, as opposed to C# a field can be default initialized with another non-static member
	int speed = 1;
	// fp is a pointer to a member function of Player that takes in 2 ints and returns void, and is assigned with
	// the adress of the move() function (where its stored in memory).
	// Note that this "void(Player* fp)(int,int) = &Player::move;" won't work, we have to use the scope resolution
	// operator ::, because the latter declares a pointer to a Player object, while the former declares
	// a pointer to a member function and indeed we want a pointer to a function, not to a to some object
	void(Player::* fp)(int,int) = &Player::move; // fp  is a pointer, see 58_function pointers for how to invoke

	Player() {}
	void move(int xa, int ya) // a member function, aka method
	{
		x += xa * speed;
		y += ya * speed;
	}

};

/* aggregates - arrays or class/structs with no private/protected/static members.
for example:*/
struct Vector2 { int x; int y; };

int main()
{
	A a; // Single variable objects are also called scalar objects as opposed to array objects
	//A a; is equivalent to "class A a;"  in fact "a", is of type "class A"; "class A" is a type
	//in the exact same way that "int" is a type.

	std::cout << sizeof(a) << "\n";
	/*
	sizeof empty class is 1 byte why?
	1. The standard does not allow objects (and classes thereof) of size 0,
	since that would make it possible for two distinct objects to have the same memory address.
	2. instntiation must return a pointer to a distinct memory address

	For example a class that stores only an "int x = 5", will store 00000005 (4bytes) in the starting address
	of the class and the size of the class as a whole will be 4 bytes
	*/

	Player player; //equivalent to "class Player player;"
	/*
	The statement above is equivalent to Player player = Player() or more fully
	Player player = Player::Player() in result but not in performance.
	The latter first constructs a player object with default ctor (rhs),
	the ctor returns the object, and later the copy ctor initializes
	the rhs into the lhs. we can also perform move initialization by
	using std::move on the rhs, or by casting rhs to a reference to an rvalue
	so we get an overhead of using the copy-ctor. note: this is not assignment 
	operation but rather an initialization operation since rhs player is not-yet-created-object,
	therefore, in this scenario when lhs and rhs are of equal types and lhs 
	player is not yet-created, we invoke a copy initialization (in the latter example)
	*/
	Player player3 = Player();
	player.move(1, 1); // "." operator is called "member of object" oeprator, allows the direct access to members
	std::cout << player.x << std::endl;
	(player3.*(player3.fp))(2,3); // function call through function pointer accessed via pointer-to-member ".*" - see 57b:
	Player* player2 = new Player{};
	(player2->*(player2->fp))(2, 3); // access member through a pointer to an object on the heap
	Player* player4 = new Player();
	player4->x; // "->" operator is called member of pointer operator, access to membmer through pointer

	/*
	We can take the size of the class itself, not only the members.
	The size includes the sum of the sizes of all non - static data members,
	plus any padding added by the compiler to align data members correctly.
	Note that heap allocated variables will not be accounted for, for example an pointer
	to an array of 5 ints, will contribute only 8 bytes (the size of the pointer), and the
	size of the array itself will not be accounted for because of two reasons:
	1. the new memory block is not part of the object itself, unlike the stack allocated variables
	2. because cpp doesn't keep  track of the sizes of dynamically allocated data ("don't get what you don't pay for")
	for example in:
	class MyClass
	{
		int* ptr; // only 8 bytes
	public:
		MyClass() : ptr(new int[10]) {}  // constructor allocates memory on the heap
		~MyClass() { delete[] ptr; }  // destructor frees the memory
	};
	*/
	std::cout << sizeof(Player);
	/*
	Important Note: in Java Player e = Player(), means that e is a class reference to a Player object of type Entity.
	in other words e is just a reference/pointer to the location in memory where the data stored on the heap. in some sense it is
	similar to the new keyword which also allocates on the heap and returns a pointer address to that location.

	in CPP Entity e = Player() means that e is actually a variable that holds the data of Player directly (inline) on the stack
	it isn't a pointer to a location in memory. this is called value semantics. pointers/references are all about reference semantics.

	*/

	//becuase classes are types, they can be defined inside a function, just like
	//an int can be defined inside a function. (class X is no different then int X
	//in terms of them declaring inside a function).
	class X
	{
	public:
		int x;
	};

	X instance{ 4 };
	std::cout << instance.x;
	
}


/*
Note 1:
Player p; //p is a class object of type Player stored on the stack (instance variable).
p.name => "." Operator is called "member access thorugh object".
p stores the object "inline", across the stack frame.

Player *p = new Player; //p is a pointer of type Player which points to an object
of type Player that is allocated on the heap via "new expression (keyword" which does 3 things:
1. requests from the os to allocate memory at the size of Player (invokes operator (function) new()) and returns a void pointer
2. constructs Player object at the allocated memory
3. implicitly converts the returned void* into a Player*

Note 2:
a class cannot have an instance of itself as a field, whether the filed is static or not
- this would cause a ctor loop:
the construction would have started with the outer object, then constructing each field which
would cause the construction of the object field and back to square 1.
A class can hold a pointer/reference of to itself! pointers don't require construction
and they simply hold memory address

Note 3:
functions in the body of the class + friend of functions of that class, can
access the private members of each instance through "this" (pointer to the current
instance) or through any other instance of the class.


Note 4:
 a->b == (*a).b
"->" member access through pointer to an object (if we have a pointer to an object
we can access the object's members via "->") 
"." member access through object (if an object as allocated on the stack, we can access
it's members directly via ".") *a -> indirection.

Note 5:
unlike free functions, member functions can call each other regardless of their order!
for example, methodA can call methodB despite the fact methodB is declared and defined
AFTER methodA. this works because A function defined within a class definition 
is an inline function.
https://stackoverflow.com/a/9192159

*/