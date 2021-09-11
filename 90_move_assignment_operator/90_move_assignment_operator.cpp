/*
std::move -> moving (taking the resources) of an exisiting object
into another exisiting object. Here we mainly discuss the move assignemnt operator.

in cpp classes have implicitly-declared move assignment operator if all the following are true:
no user declared: move assignment operator / copy constructor / move constuctor/  copy assignment/ destructors

move assignment does 4 things:
1. check that the object we want to steal from and the stealing object are different (!=)
2. reroute (steal) the resources from the argument to the newly created object
3. set the resources of the argument to null, ensuring that the object can
be destructed without destroying the stolen resources
4. in the end, we return *this to enable chaining of "=" operations (e.g: a=std::move(b)=std::move(c))
*/


#include <iostream>

class String
{
private:
	char* m_data;
	uint32_t m_size;

public:
	String() = default;

	String(const char* string)
	{
		printf("created\n");
		m_size = strlen(string) + 1;
		m_data = new char[m_size];
		memcpy(m_data, string, m_size);
	}

	String(const String& other) //copy ctor overwrite
	{
		printf("copied\n");
		m_size = other.m_size;
		m_data = new char[m_size];
		memcpy(m_data, other.m_data, m_size);
	}

	String(String&& other) noexcept // the move ctor
	{
		printf("moved\n");
		m_size = other.m_size;  //stealing the size from other
		m_data = other.m_data; //stealing m_data from other (pointing to the same location)

		other.m_data = nullptr; //other.m_data buffer now points to nowhere
		other.m_size = 0; // resseting other's size, since m_data is now pointing to nullptr
	}

	String& operator=(String&& other) noexcept //MOVE_ASSIGNEMNT OPERATOR overload. takes in an rvalue ref (temp object)
	//very similiar to assignment operator overloading which takes in a const lvalue reference
	//We are not constructing a new object, but rather move the "other" into "this".
	//The reason why "other" is not const is because 
	{

		//first we must prevent a scenario where we move a variable to itself,
		if (this == &other) //if lhs and rhs are the same, just return lhs
			return *this; // if they are the same, no moving necessary

		printf("moved\n");

		/*m_data = other.m_data; //may look correct but we create a memory leak because m_data of "this"
		needs to be deleted before we assign it the address of another location. This is done to prevent a memory
		leak, meaning that we prevent a situation where no pointers will point to the location of where
		m_data is pointing now. we are about to assign m_data with a new location (the location of other.m_data)
		if don't delete (return to the os) the current block of memory that m_data occupies now, assigning
		it a new location, will levae theold location leaking with no pointers pointing to it, and without
		the abillity to delete it*/
		delete[] m_data;
		m_data = other.m_data; // now that "this" m_data is clear, we can safely move "other" m_data into "this"
		m_size = other.m_size;  //stealing the size from other

		//leaving the "other" in a nullable state, ready for safe deletion. Otherwise, when "other" will be
		//deleted, the deletion of the memory locations will damage the new object we are creating with these ctor,
		//it will find out that the members it stolen were deleted during the destruction of the object we've
		//stolen from.
		other.m_data = nullptr; //other.m_data buffer now points to nowhere
		other.m_size = 0; // resseting other's size, since m_data is now pointing to nullptr

		/*
		Important note: a=b is equivalent to a.operator=(b)  in this case we actually don't need to return
		anything. but when dealing with chaining statements as a=b=c which is equivalent to a.operator=(b.operator=(c))
		we have to return a value -> b.operator=(c) returns a ref to a Vector2 which later evaluated
		in a.operator=(the returned value)

		in addition we return *this since we want to return the data itself and not a memory address
		and return an lvalue reference to the newly created object to avoid creating new data  (lvalue can be assigned with an lvalue).
		the relationship between the *this and the return value is like an assignment (String& in an lvalue reference to *this).

		Another reason for returning *this is because the signature of the overloaded copy-assignment function signature (convention)
		which requires to return an lvalue ref to the created object, for reasons stated above
		*/
	}


	~String()
	{
		std::cout << "destroyed" << std::endl;
		delete[] m_data;
	}

	void print_string()
	{
		if(m_data) std::cout << m_data << std::endl;
	}
};


//class Entity wraps around String class
class Entity
{
public:
	Entity(String&& name) : m_Name((String&&)name) {}
	Entity(const String& name) : m_Name(name) {}

private:
	String m_Name;
};

int main()
{
	Entity entity("hello"); // equivalent to Entity::Entity(String("hello)) COMPLEX IMPLICIT CONVERSION! details in 89_move_semantics

	String string = "hello";
	String dest = string; //invokes a copy ctor. but what if we just want to move string into dest?
	//in other words, steal the resources of string?->

	/*********************************************
	constructors (regular, move, copy ctors) are used to create a new object
	either directly with a ctor, or by directly constructing another
	object into the memory location of our object (via rvo, see 25_constructor)

	assignments are used to assign values eithery by copy or by moving an exisiting
	object into an already created object. if the object is not-yet-created, it means
	it will be constructed. If it is alreday created, it means it will be assgined
	via move/copy assignment operators

	******************************/

	
	/*
	Because dest1 is not-yet-created, assignment to dest1 cannot happen.
	Since lhs & rhs are of the same class type, the compiler constructs the lhs 
	through move ctor by using the rhs as the an argument for the move constructor.
	This can only occur when both sides are of the same type.
	*/
	String dest1 = (String&&)string;  // uses the move ctor - move initialization
	
	
	//std::move is equivalent to in its effect to casting to an rvalue reference
	String dest2 = std::move(string); //uses the move ctor - move initialization! 
	/*
	std::move doesn't do a move. It actually casts the lvalue reference to an rvalue reference.
	Note: the statement above does not use the assignment operator(=)! since the assignemnt operation
	can only be used on already created object! in this case the "=" is just a syntactic sugar,
	we actually perform a MOVE INITIALIZATION of dest2 not ASSIGNMENT.
	In addition, the assignemnt is just an overloaded operator so:
	String dest2.operator=(...) doesn't really make sense..
	more info in 25_constructor.
	*/

	//using the move assignment operator:
	String apple = "apple"; //lhs & rhs different types, lhs is not-yet-created ->rhs is constructed
	//directly in the memory of apple through copy construction (instead of constructing String via "apple"
	//and then copy constructing lhs with this object - this second copy ctor is elided via rvo)
	String dest3;


	dest3 = std::move(apple);  //rhs and lhs already created - using the move assignment operator!
	//equivalent to dest3.operator=(std::move(apple))
	dest3.print_string(); //should print "apple"
	apple.print_string(); //should be empty since dest3 has taken over "apple" resoureces, apple's m_data
	//which stores the string is now pointing to null

}
