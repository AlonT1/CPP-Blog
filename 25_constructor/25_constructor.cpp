#include <iostream>

//all rules apply to stuct (except struct are public by default, class are private)

//Entity1 is an aggregate object
//contains implicitly-declared constructor (the declaration is implicitly done by the compiler
//if the user doesn't provide a DEFINED constructor). At construction of Entity1, this implicit default constructor will become
//an IMPLICITLY-defined constructor which means that this line will be added to Entity1 "inline Entity() noexcept = default;"
class Entity1 
{
public:
	int x; int y;
	//no ctor -> the same as writing Entity1(){}

};

class Entity2 //same as Entity 1, default constructor is explicit
{
public:
	int x; int y;
	Entity2() //has an explicit, accessible default constructor - this renders 
	//the "implicitly-declared default ctor" as a "deleted implictly-declared ctor"
	{
		std::cout << "Entity2_Hello" << std::endl;
		x = y = 0;
	}
	Entity2(int t)
	{
		std::cout << t;
	}
	Entity2(const Entity2& other) // copy ctor
	{
		x = other.x;
		y = other.y;
	}
	Entity2(Entity2&& other) // move ctor
	{
		x = other.x;
		y = other.y;
		other.x = 0;
		other.y = 0;
	}

	Entity2& operator= (const Entity2& other) // move operator when lhs is already defined
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
	}

	Entity2& operator= (Entity2&& other) // copy operator when lhs is already defined
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		other.x = 0;
		other.y = 0;
	}

	void makore()
	{
		std::cout << "ken";
	}
};

//instead of ctor we can use init function init(){x=0 y=0}
class Entity3
{
public:
	int x = 5; // default member init. (aka non-staic member init.) during the construction,
	//the ctor wins (x=5 is disregarded completely), if the ctor doesn't initialize it, then the in-place wins
	int y;
	Entity3() //overriding implicitly declared default ctor - Calling Entity3 e; will no refer to this ctor
	{
		x = 0;
		y = 0;
	}
	Entity3(int w, int z) //ctor overload
	{
		x = w;
		y = z;
	}
};

class Entity4
{
public:
	int x, y;
	Entity4() = default; // "explictly defaulted constructor definition" - explictly instructs the compiler
	//to generate a definition for a default constructor - equivalent to to "implictly declared default constructor"
	//only explicitly requested by the programmer.
	Entity4(int x, int y) : x(x), y(y) {}
	Entity4(int x) : x(x), y(4) {}
	Entity4(Entity4& other) { x = other.x; y = other.y; }
	Entity4(Entity4* ptr) {}
	/*
	---------------------------------------------------------------------------------------
	both ctors above are Converting Constructors - a ctor which is not declared with the
	specifier explicit which prohibit implicit conversion.

	Converting Constructor feature - specifies an implicit conversion from the types of its arguments
	to the type of its class. for example: Entity4 e4 = 5 (copy initialization); is implicitly converted to Entity4::Entity4(5)
	---------------------------------------------------------------------------------------
	*/
};

class Log
{
private:
	//Log() {} // can't access default constructor - Log l Inaccesible error
public:
	Log() = delete; // deletes default implicitly-declared constructor (defines it as deleted)
	//causes an error - the default constructor cannot be referenced
	//Note: if the ctor was inaccessible (in private), then the fault ctor would also become deleted
};


int main()
{

	/*
	Assembly Equivalence between the following construction techniques (all exactly identical):
	e = an expression (combination of literals that evaluates to a single value).

	"Entity a{e};"  <==>  "Entity a = {e};"  <==>  "Entity a = Entity{e}"; // {} means direct list initialization - more modern
	"Entity a(e);"  <==>  "Entity a = (e);"  <==>   "Entity a = 2;"  <==> "Entity a = Entity(e)"; // () means direct initialization
	*/

	//Entity e1(); // forward declaration of  a function named e1 which returns Entity type!!
	//this is not a construction!!!


	Entity1* e13 = new Entity1; //heap allocation with implicit ctor (indeterminate (garbage) values. returns address of entity1 to e13 ptr.
	Entity1* e14 = new Entity1(); //new Entity1() zero-initializes pod values, even if no explicit ctor exist!

	/*


	In addition: when no ctors exist and class members are pod's x,y  ->:
	Entity e{} - >  default initalization {0,0}  - x and y initialized with 0
	Entity e{3,4} ->  x initialized with 3, y initialized with 4
	Entity e{3} -> x initialized with 3, y initialized with 0
 	When there is a ctor {} will prefer to initialize with a ctor
  
	This is allowed because Entity1 is considered an "aggregate" (aggregates pod's) - 
	an array or class with no user-declared ctors, no private
	or protected non-static data members , no base classes and no virtual functions. In other words, a simple struct or class
	that contains ONLY POD'S (plain-old-data: int, float, char...). also valid for std collections, e.g: std::vector<int> a = {1,2,3}
	*/

	Entity e{}; // vs Entity e; :
	// the default ctor is a ctor with no parameters.
	//as opposed  to "Entity e" , the uniform initializer "{}" assigns default values to pod's. 
	//if  an explicit default ctor exists then uniform initalization won't initialize pod's to defaults (this happens only when 
	//default ctor is implicit or marked with default)

	/*******************list initialization with explicit ctor***********/
	Entity4 e_test{ 3, 4 }; // equivalent to "Entity4 e = Entity4{1,2};"


	/*******************Direct-list initialization with explicit ctor (identical to above)************/
	Entity1 e2 = { 3.8 }; //equivalent to  "Entity1 e2 {3.8};" and "Entity1 e2 = Entity1 {3.8};" (assembly-identical)
	/*
	the above statement is a list initialization, but is it a DIRECT list initialization? according to the cppreference.com
	direct initialization means initializing an object from explicit set of ctor arguments. in this case 3.8 assigned to x is explicit
	but the 0 value assigned to y is implicit. Despite this, I believe that the definition still holds, since we directly
	initialize the object, not through a copy.

	IMPORTANT NOTE: for OBJECTS The above statement is not an assignment (=) but rather a list *initialization*!
	this is because in cpp we cannot use the assignment operator (=) with NOT YET CREATED OBJECTS (e2 is not yet created),
	in other words the following is not possible for newly created object: Entity e2.operator=(3.8);
	therefore when dealing with not-yet-created-objects, '=' is just a syntactic sugar for list initialization:
	Entity e2 {3.8};

	INITIALIZATION means giving an object an initial value during creation
	ASSIGNMENT means assigning a new value to an exisiting object

	in pod's int x=5 is equal to int x(5), equal to int x{5}:
	int x{5} direct list initialization is the safest for INITIALIZING new values. it has added benefits over direct initilization - int x(5).
	int x=5 is a syntactic sugar for x{5} or x(5), using the assignment operator (=) is less fit for when we want to initialize the
	the variable for the first time, rather its more appropriate we want to reassign a value to the already exisiting object.
	*/


	/***************relationship between a user-defined object and
	the parameter that accepts it (in funcs such as ctors)*******************

	1. "a" not-yet-created + "b" is a prvalue, for example:
	Entity a = 10 (assuming Entity supports a ctor with int) or Entity a = Entity(5),
	both 10 and Entity(5) are prvalues:

	regular construction of "b" occurs in the memory of "a" if copy elision is enabled,
	otherwise "b" is constructed and then is copied constructed into "a"

	Notes: 
	b. if a ctor doesn't exist an implicitly-declared ctor will be invoked
	c. if rvo is disabled and copy ctor doesn't exist, an implicitly-declared copy ctor
	will be invoked, for copying constructed rhs (b) into (a)
	d. 	Entity4 e8 = (2, 3); //same as above, but since copy-initialization through implicit type conversion
	//can only occur with one element, only 3 is chosen, thus invoking the Entity4::Entity4(int) ctor with 3,
	//we can be more specific by "Entity4 e8 = Entity4 (2,3);

	*/

	Entity2 m = { 2 };
	/*
	stages:
	the compiler recognizes that Entity2 has a ctor that supports int argument, this starts the following procedure:
	1. rhs is implicitly type converted to an Entity2, equivalent to "Entity2(2)" (regular converting constructor), in the same way
	that when "float x = 5" rhs is actually implicitly type converted to float(5).
	2. after the temporary rhs object is constructed, the copy ctor is invoked, therefore the lhs is "copy-initialized",
	constructed via a copy initialization. (one constructor for rhs, and a copy ctor for copying rhs to lhs)
	Notes:
	1.copy ctor is invoked only  when the flag "-fno-elide-constructors" is enabled, otherwise the compiler
	optimizes performance by constructing (with the regular (non-copy, non-move) constructor
	directly in the memory location of e5 (stack frame of main) (process called copy elision) 
	2. although a overwritten copy ctor takes in a const lvalue refernce, because of the "const" keyword,
	the constructor also accepts rvalues, explained in 85_lr_values
	*/

	/*
	2. "a" not-yet-created + "b" is created (an lvalue) + both are the same type, for example:
	Entity b;  Entity a = b;   b is an lvalue
	copy constructor is invoked by the compiler, b is fed into the copy ctor and a is constructed
	by copying it.
	Note: if a copy ctor doesn't exist, an implicitly-declared copy ctor will be used.
	*/
	Entity2 b(10);
	Entity2 a = b;


	/*
	3. "a" not-yet-created + "b" is created and casted to an rvalue ref (manually or via std::move)+
	both and a,b are of the same type:
	Entity a;  Entity a = (Entity&&)b   or Entity a = std::move(b)
	the move constructor is invoked by the compiler, constructing a by moving members of b into a
	Note: if a move ctor doesn't exist, a copy ctor will be chosen, if a copy ctor doens't exist
	an implicit copy ctor will be invoked (to construct "a" by copying "b")
	*/
	Entity2 c = std::move(b);


	/*
	4. "a is created" + "b" is not yet created (prvalue), for example:
	Entity a;	a = Entity(5); or a = 10;
	rhs is first constructed and then:
	1. if move assignment and copy assignment
	exists, the move assignment will be preferred (assigning lhs with rhs),
	that's because the rhs is a prvalue and move assignment operator accetps rvalues
	(prvalue is from the rvalue non-primary category)
	2. otherwise, if a move ctor doesn't exist the copy assignment will be prefrred
	(const lvalue refs can accept prvalues because prvalues are not modifiable, and 
	by declaring the ref as const we are promising that we'll not modify the unmodifiable
	prvalue, further details in 85_prvalues).
	3. If neither a copy or move assignment exists - error! can't use implicitly
	declared copy/move assginments -THEY ARE ESSENTIALY DELETED FUNCTIONS IF NOT DECLARED!!!!!!!
	*/
	b = Entity2(10);

	/*
	5. "a is created" + "b" is created (lvalue), for example:
	Entity a;	Entity b;	a = b;
	copy assignment operator is invoked only if it is declared, otherwise it 
	is considered as deleted. copy assignment parameter is a reference to an lvalue,
	and because the lhs is an lvalue, the copy assignemnt is a perfect match.

	*/
	b = c;


	/*
	6. "a" is created + "b" is created and cast to an rvalue ref (manually or via std::move)+
	both and a,b are of the same type:
	Entity a;  Entity a = (Entity&&)b   or Entity a = std::move(b)
	the move assignment is invoked by the compiler
	Note: if a move assignment doesn't exist, a copy assignment will be chosen, if a copy assignment doens't exist -
	error - a copy assignemnt wasn't declared (equivalent to deleted)
	*/
	b = std::move(c);



	/*******************dynamic allocations - with pointers - requires "new" and "delete"*/
	//allocated on the heap
	Entity2* e_ptr = new Entity2(4); //see 39a_new_operator_expression

	/*******************dynamic allocations - without pointers*/
	/*
	whats happening:
	1. rhs object is constructed on the heap
	2. rhs is implicitly converted to Entity object:  "Entity4(new Entity4(5))" this invokes
	the ctor Entity4(Entity4 *ptr) which takes in the inner argument "new Entity4(5))
	*ptr is the controlling pointer of the dynamically allocated space, it must be eventually
	"deleted" to avoid memory leak
	3. a copy ctor is invoked which copy-initializes the temp rhs rvalue into lhs
	*/
	Entity4 entity4 = new Entity4(5);



	/*******************Direct Initialization with implicit ctor*/
	//called "direct" because we use the ctor of the class to construct an object directly,
	//as opposed to copy/move initialization where we use another object to create our object.
	Entity1 e1;

	/*-
	the compiler initializes e by defining the implicitly declared default ctor, which now becomes
	an implictly defined ctor that constructs the object with with indeterminate (garbage) values.
	in java it is equivalent to Entity1() ctor.  In cpp Entity e1() means a decleration of function!
	therefore the statement "Entity e1()" doesn't construct an object
	if Entity1() explicit ctor doesn't exist, an implicit ctor will run which doesn't initializes vars - but we can access them...
	not overwriting default ctor is ok  if Entity doesn't have variables, or static ones,
	or inplace intitalized (public: int x=2; int y=2;)
	creating any kind of constructor other than Entity1(), renders the default constructor as dead -  the statement: "Entity1 e1"
	will yield error, if a ctor such as Entity1(int) exists, since now we have a proper ctor we can call.
	without a proper ctor, members are constructed in the order of their decleration within the class.
	 pod (plain-old-data) (int, float,ptr's..) do not have an explicit constructor - cpp does nothing unless
	 we ask for (cpp philosohopy) - to improve performance
	*/
	
	/*******************aggregate list initialization with explicit ctor**********
	explained in Entity1 class.
	{} is called direct-list init.: direct because w init. an object via explicit ctor arguments. list = {} 
	() is called simply direct initialization
	because e is an aggregate object(no user - provided ctors, all public with no inheritance) this
	is called an aggreate list initialization.*/
	Entity1 e{ 3, 4 };


}

/*
class UserName {

private:
	std::string mName;

public:
	//compiler raises an ambiguity error - after converting "jonn" to an std::string
	//the std::string can be accepted by both constructors
	UserName(std::string str): mName(str){ }
	UserName(const std::string& str): mName(str){ }
};

int main()
{
	UserName u1{ "John" }; // whether we want to use a copy /move ctor
	//the compiler recognizes that it first must construct "john" as an std::string
	//and only then copy/move it into m_name.
	//we'll get heap allocation for "john" as an std::string and the copy ctor
	//will cause another new heap allocation for copying the "john" std::string
	//into m_name  - can be mitigated to 0 heap allocations via std::string_view


}

*/
