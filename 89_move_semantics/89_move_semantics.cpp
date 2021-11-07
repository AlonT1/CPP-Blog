/*
in C++11 it was decided that the compiler will implicitly generate move constructor as member-wise moves
(moves members), unless you have explicitly defined a copy constructor or copy/move assignment or a destructor.

move ctors do 2 things:
1. reroute (steal) the resources from the argument to the newly created object
2. set the resources of the argument to null, ensuring that the object can 
be destructed without destroying the stolen resources
https://akrzemi1.wordpress.com/2011/08/30/move-constructor-qa/

move ctors and assginment operators takes in an rvalue reference - a reference
to an rvalue (xvalue or prvalue) - a temporary object who is "stealable" - 
by supplying an rvalue we convey that the object is indeed a temporary - it isn't
manipulated somewhere else by another actor, therefore it is safe to steal its content

When to use move ctors?
 when moving variable b into variable a
Obj a = std::move(b); // stealing b into a via move ctor;

Obj c;
c = std::move(a) // stealing a into c via move assignment operator
//(not move ctor because c already exists)

// what about returning from a function?
// no reason to do that! rvo is better, just return an lvalue

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
		m_size = std::strlen(string) + 1; // +1 for null character
		m_data = new char[m_size];
		memcpy(m_data, string, m_size);
	}

	String(const String& other) //copy ctor overwrite to perform deep copy
	//invoked when assigning b to a, where a is not yet-initialized, a&b are of same types.
	{
		printf("copied\n");
		m_size = other.m_size;
		m_data = new char[m_size];
		memcpy(m_data, other.m_data, m_size);
	}

	//the move ctor
	String(String&& other) noexcept // not throwing exceptions the move ctor.
	//see 103_exceptions move_if_noexcept - tl;dr because move ctor can fail
	//leaving two object damaged (the stolen and the object we creating),
	//we need to promise the compiler the ctor will not fail via "noexcept
	//(not throwing exceptions).

	{
		printf("moved\n");
		m_size = other.m_size;  //stealing the size from other
		m_data = other.m_data; //stealing m_data from other (pointing to the same location)


		/*
		"nullptr"ing members on object we took ownership from (move), prevents a 
		situation where the dtor performs a delete on those pointer members
		(impossible now because the ptrs are null) and destroys the resources pointed at, 
		while we already moved ownership to another object (using that object will invoke errors because 
		the pointers are now pointing to deleted resources).
		Thus it's important to nullptr members from the class we move from, 
		to ensure that its dtor won't be able to destroy the resources we have taken into a new owner.
		*/
		other.m_data = nullptr; //other.m_data buffer now points to nowhere to prevent dangling pointer
		other.m_size = 0; // reseting other's size, since m_data is now pointing to nullptr

		//Note that move ctors can also have initializer list!
	}

	~String()
	{
		std::cout << "destroyed" << std::endl;
		delete m_data;
	}

	void print_string()
	{
		std::cout << m_data << std::endl;
	}
};


//class Entity wraps around String class
class Entity
{
private:
	String m_Name;

public:

	/***************************************
	Important note: when constructing an object with a prvalue and
	two ctors exists when one is copy ctor and the other is a move ctor
	then the move ctor will be chosen (despite that both of them can accept an prvalue),
	because the move ctor is more efficient by rerouting  heap objects from b to a & putting b's objects to sleep (move),
	instead of deeply copying objects from b to a like a copy ctor, which is not intended to 
	modify the copied object like a move ctor, only to deeply copy its contents.
	prvalues by default are the ones that are about to die, thus if we are dealing with the kinds of values
	it safe to assume that we can steal of  their members without damanging any other components,
	or causing any side effects because prvalues are not used by other components 
	(to be used they would have a to had an ID (memory address) and thus an identifier
	and prvalues don't have an ID, they don't have an identifier through which other objects can use them.

	"Whereas the goal of the copy constructor and copy assignment is to make a 
	copy of one object to another, the goal of the move constructor
	and move assignment is to move ownership of the resources from one object to another"
	***********************************************************************/

	Entity(String&& name) : m_Name(std::move(name)) {} //std::move(name): 
	//equivalent to (String&&)name - this is the expression that actually invokes the move ctor of String class.
	//std::move does not move anything, it simply casts objects to a xvalues which makes them POTENTIALLY moveable-from
	//Note: this ctor accepts expressions that are references to rvalues and then we convert name back to
	//to an xvalue via std::move. why?
	//because name indeed accepts rvalues, BUT once we are in the scope of the constructor, 
	//the "name" expression is an lvalue for all intents and purposes! "String&& name" only
	//denotes that we are willing to accept rvalues + "name" will act as a reference to this rvalue
	//+ the expression is bound to function as a "String" object. "name" as a sole expression
	//in the context of the function is just an lvalue, therefore we are required to cast
	//it to an rvalue to perform the move construction.

	Entity(const String& name) : m_Name(name) {} //equivalent to "m_Name = name" 
	//inovkes the copy ctor of String, which was overwritten to deeply copies "String name" into "String m_Name"


};

String func()
{
	String a("test");
	return a;
}


int main()
{
	// a simpler example
	String str("shalom");
	String str1(std::move(str)); // move construction of not-yet created str1, via
	//the move ctor of string. by using std::move we convert "str" expression (expression because 
	//it evaluate to an object) from an lvalue (has ID, not stealable), to xvalue (having ID, stealable).
	//This is necessary since the move constructor accepts expressions that are references to the
	//non-primary category of rvalues (includes prvalues and xvalues),
	//therefore it cannot accept lvalues such as "str" (from the glvalue primary value category,
	//part of the , but it CAN accept xvalues which are from the rvalue family (we pass expressions to functions)
	
	//the move constructor reroutes the resources from str into str1,
	//meaning m_data of str1 takes the address of m_data of str and leaves data members
	//of str in a nullable state, with non dangling pointers and without memory leaks.
	//explained below in "HOW DOES THE MOVE OPERATION OPTIMIZES THE CODE"

	//****************************************************************
	//the compiler generates the following code for the statemenet above
	//"String str1 = String(std::move(str));"
	//****************************************************************

	String str2(
		String("ken")
	);


	Entity entity("hello"); // equivalent to Entity::Entity(String("hello")) COMPLEX IMPLICIT CONVERSION!
	/*
	entity has a ctor that takes in a  String instance. the String class has a ctor that takes in a const char*. since the argument is
	"hello" which is also a const char*, the compiler can actually implicitly convert the written
	statement into Entity::Entity(String("hello")).
	Conclusion: implict conversion works as longs as somewhere down the construction line
	the base class ctor accepts the argument! in this case even though entity ctor doesn't allow a const char*
	as a ctor parameter, due to the order of construction:
	->
	1. String("hello") is constructed
	2. The constructed object is passed to the ctor one of Entity's ctors
	3. because String("hello") is a prvalue, Entity class will seek one of the following  ctor's
		a. a ctor that takes in a reference to an rvalue (the move ctor) - the best fit, because
		the temporary String("hello") is a prvalue (a primary category) that belongs to the non-primary rvalue category
		and the ctor accepts rvalues, which is a perfect match for String("hello") object:
		this ctor will initialize m_name by stealing "name" of the temp object -
		this is done done by invoking the move ctor of String via std::move -.
		this ctor takes a ref to an rvalue, and "reroutes" the data from it into the new object. after the reroute
		is done the pointers and data of the temporary object are reset to null (done to ensure that when the temp
		object reaches the end of the construction scope, its dtor will not delete the data we stole)
	
		b. if the ctor in "a" doesn't exist, the ctor which takes in a CONST lvalue ref will be chosen - 
		which is the copy ctor. const lvalues refs are expended to accept prvalues such as String("hello").
		this because prvalues are const by default (have no memory thus unmodifiable),
		and by marking the lvalue ref as const, we agree not to modify the unmodifiable prvalue, therefore we can accept String("hello")
		this ctor will initialize m_Name by invoking a copy-initialization of  "name" into m_Name. (explained in 85_lr_values)
		"m_Name = name" invokes a copy ctor (both lhs and rhs are of same type, and m_Name is not yet created.
		this is not an assignment operation!). the copy ctor is overwritten to perform a deep copy of the temporary "name" into m_Name,
		since a shallow copy of the heap allocated data of "name" will dangling ponters, because "name" will be
		deleted at the end of the construction and m_Name pointers will remain pointing to deleted memory.
		(dangling pointers point to "deleted" memory via "delete ptr", therefore we set them to null, to make
		sure we don't dereference memory that was returned to the os (a.k.a deleted))

	4. after the initialization of m_Name, Since the temporary object constructed in 1 is no longer in use and it
	reached the end of its scoped, it gets deleted.
	5. the object is finally built
	*/

	/*HOW DOES THE MOVE OPERATION OPTIMIZES THE CODE:
	
	1. No longer need to "new" and "delete" objects and risk memory leaks and dangling pointers:
	(read the problems associated with new in 39_new)

	2. move semantics provide a performant alternative to copying objects:

	the problems with copy operations: 
	a. Problem 1 - dangers of a shallow copy:
	shallowingly copying an rvalue object with heap allocated members is dangerous, because
	the rvalue object (that we copy from) is deleted after the copy assignment, leaving the newly created
	lvalue object Entity object with an heap member that points to a deleted memory location (null) (shallow copy,
	only copies the pointer's value, and the data at that value(memory address) is deleted in the dtor of temp object)

	b. Problem 2 - so lets deep copy - but there is a cost:
	deep copy of an rvalue object with heap members is ok, but is expensive performance wise. after the temp rvalue object
	is deeply copied (into m_Name), the compiler deletes it, and since everything is completely copied before the deletion, the newly copied
	heap members are pointing to new locations with the copied data. DEEP COPY IS ONLY PROBLEMATIC WHEN DEALING WITH HEAP
	allocated class members, they are the real issue, because we must make sure that the class members in the newly created
	object point to a valid location with the same data that was in the deleted one, but copied into a new location which is expensive.

	The move semantic solution (ownership transfer):
	an alternative to a deep copy is to use the data of the temp rvalue and just reroute it
	to the newly created instance. new memory will not be allocated, rather the newly created instance m_Name
	copies the data and the pointers addresses of the temp rvalue into m_name, and assigns nullptrs or 
	zero's to the data of temp rvalue to leave it in well-defined, deterministc state, allowing for its
	self destruction and deletion (no dangling pointers pointing to nowhere or memory leaks).
	THIS SOLVES THE PROBLEM IN (1) WHERE AFTER SHALLOWLY COPYING THE DATA OF RVALUE AND DELETING IT, 
	THE newly created object would STORE POINTERS TO DEAD LOCATIONS (DANGLING) due to the deletion of the object
	we copied from. now the rvalue can be deleted safely, because Before its dtor ran, we "MOVED" its data and pointers
	into our object and reset all of its data and pointers to null/0 to ensure that 
	its deletion will not delete the data in those locations (affecting the data the we've stollen,
	leaving our object pointing to dead locations.). 

	SO IN THE END, WE MANAGED TO AVOID THE DEEP COPYING OF THE HEAP ALLOCATED OBJECTS (2)! AND INSTEAD, SAFELY
	COPYING ONLY THE VALUE OF THE POINTERS! (SHALLOW COPY) while allowing a safe destruction of the rvalue.

	This is the most "cost-effective" way of performing an initialization/assignment
	of a user-defined type inside of a class. There is another way, but problematic:
	m_Name could be an lvalue reference, and it could be initialized in the member initializer list with an existing lvalue String object
	(created outside of the class) passed through the ctor. This could work but will present an ownership issue, since now the
	data is managed outside of the class and inside it, e.g:

	There is a disadvantage to move semantics when comparing them with copy semantics:
	copy semantics promise not to damage the copied object by making them const, they simply
	perform a deep copy of all their fields - an exception thrown during a copy process, 
	won't damage the object we copy from.
	In contrast during move semantics, we set all members of the object we steal from to null state.
	An exception thrown at this stage may damage both the object we steal from and the object that
	is stealing, potentially losing resources.
	With copy semantics we are always sure that the object we copy from will retain the
	resources / won't be damaged even during exceptions.

	Notes:

	1. the compiler always preferes move semantics over copy semantics. move semantics are more performant
	(no deep copying, only stealing resources by copying address), although move semantics are less safe - 
	a an exception thrown during move opeation (in move ctor or move assignemnt) may cause
	the object we steal from to be set to null, and the object we try to build failed in construction
	(will cause a stright jump to catch block) - basically we will lose both objects.
	On the other hand move semantics are somewhat safer because they mitigate possible duplicates
	(an unecessary deep copy of strings for example).

	2. If we perform a move construction/assignment via an lvalue by std::move(the lvalue)
	We can’t assume it’s safe to alter the l-value because it might be in use by other components.
	Therfore we must be careful when dealing with move semantincs, and remember that although
	copy semantics are more expensive, they are safer because they promise not to damage the object
	we steal from (the object being copied is const for a reason both in copy ctor and assignment).
	
	3. However, if we construct an object or do an assignment where the argument is a prvalue,
	then we know that prvalue is just a temporary object of some kind. Instead of copying it
	(which can be expensive), we can simply transfer its resources (which is cheap) 
	to the object we’re constructing or assigning (done automatically if move semantics
	(move ctor / move assignment exist)). This is safe to do because the temporary will 
	be destroyed at the end of the expression anyway, so we know it will never be used again!

	4. in move-enabled classes, it is sometimes desirable to delete the copy constructor 
	and copy assignment functions to ensure copies aren’t made. 

	5.The C++ specification has a special rule that says automatic objects (lvalues
	such as "temp") returned  from a function by value can be "moved" even if they are lvalues.
	see 91_rvo for further details

	6.	assume we create an object in a function, how can we return it to the caller?
	we can return by value (performing a deep copy) or by "newing" up the object and returning
	the address of that memory location  (anything allocated with new is heap-allocated and
	uses a "dynamic storage specifier" - the object lives until delete).
	A more performant alternative is to perform a move operation passing the ownership
	(passing the resources) to a variable outside the functions scope. example:

		std::string func()
		{
			std::string x{ "shalom" }; //single "new" allocation, no extra copies to the caller
			return std::move(x);
		}

		int main()
		{
			std::string str = func();
		}

		Notes:

		1. This can actaully be bad for performance because it disables rvo which is more
		effective - see 91_rvo.

		2. returning lvalue and not lvalueref or rvalueref because x is local and
		would be destroyed when leaving the scope (we don't want a reference to a deleted object).

	*/


}
