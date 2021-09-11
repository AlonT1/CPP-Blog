/*
initializer list is more of syntactic sugar (cleaner), but it does have an effect
in functionality in classes.*/

#include <iostream>



/*********functional difference of initializer list**********/
class Stub
{
public:
	Stub() //default ctor overwrite
	{
		std::cout << "Created entity" << std::endl;
	}
	Stub(int x)
	{
		std::cout << "Created Entity with" << x << "!" << std::endl;
	}
};

//bad example of ctor - Driver2 is correct
class Driver
{
public:
	Stub m;
	//int x
	Driver()
	{
		m = Stub(5);
	}
	/*
	THE ABOVE CONSTRUCTOR IS PROBLEMATIC
	in non-pod(plain old data - ints, floats, ptrs...) such as classes, the above ctor is equivalent to:

	Driver() : m() 	//implicitly, m is Defined and initialized with Stub object constructed with default ctor (Stub()). doesn't happen with ints - e.g x()
				//in other words, objects are implicitly constructed  with default ctor and assigned (to the reference type variable -  m)
	{
		m = Stub(5); // m is assigned with instantiated object constructed with  Stub(int x) ctor
	}

	Stub is instantiated twice and there are two assignments!!!  instead of one instantiation and assignment.
	The first (implicit) instantiation and assignment is redundant thanks to the explicit line in the body m=Stub(5)
	Therefore, list initialization is the best:
	*/
};


//correct optimized ctor
class Driver2
{
public:
	Stub m;
	Driver2() : m{ 5 } {} //m{5} equivalent to m(5) only with m{5} {} notifies against narrowing of ints ((3.8) into 3)
	//here Stub object is instiated once with Stub(5) and assigned to m right away -> 1 instantiation 1 assignment
	//WE OVERWRITTEN THE IMPLICIT INITIALIZATION LIST Driver2(): m()
};
/**********************************************************/


/*
Order of definition+initialization (mib - member initializer list):
Important note: the following describes the construction of an object with pod's and user-defined types:
1. EVEN BEFORE ANY CTOR is called, static members of a class are defined when entering the program.
static functions cannot modify non-static members and non-static functions cannot modify static members.
(ctors can't r/w static members). outside the class we access them by ClassName:: operator (because
static functions/variables belong to the class and not to a specific instance.
2. the compiler reaches the chosen ctor (specifically the start of the ctor's body, but 
one statement later jumps to the mib)
3. at this stage all pod's are defined and initialized with garbage values!
note: static members must be explicitly initialized inline with a value - they belong to the class
and initialized even before entering main
class members are still undefined at this point (although debugger shows that the members of these
classes are given garbage values).
4. the compiler enters the mib  - process of initialization:

	a. 1. if the current class inehrits from a Base class then:
			a. if a delegating ctor is included in mib (e.g Base(3,4)), then the delegating ctor will execute
			first no matter if we initialize other members before (e.g mib is: m_x{x}, Base{3,4} - Base{3,4}
			will execute first)
			b. if there is no delegating ctor, then the default ctor (Base(){}) will execute. (Note: vs debugger
			will only jump to explictly declared "Base(){}" default ctor, the debugger won't jump to Base()=default
			(and of course won't jump to the implicit non-declared ctor).
		2. if the current class doesn't inherit from a base - continue to b.

	b. members are initialized according to their order of declaration in the class,
	not according to the order of initialization in the mib (the order in both is recommended to be matched
	especially if somehow the members are dependant on each other).

	c. that being said, user-defined type (classes) members are always initialized
	before pod's (even if they're declared after them)

	d. pod / classes can be explicitly initialized inline at declaration OR in mib:
	inside mib, if a member isn't initliazed in mib then the compiler seeks if the pod/class was initialized 
	at declaration (default member initialization), and if thats the case the compiler jumps to the inline declaration and initializes
	the class / pod member and continues inside mib. if the member wasn't initialized inline or in mib:
		a. pod - remains with the garbage value it recieved in step 2
		b. class object - implicitly initialized with a default ctor.

	e. if pod / classes are explicitly initialized both inline at declaration and in mib
	***then member initialization list wins and inline initilaization at declaration is discarded completely****.

5. after finishing with the member initalizer list, the compiler enters the body of the ctor.
at this point, all pod's are defined (with explicit/garbage values - if no explicit definition),
and all user-defined types are defined+initilized (implicitly by the compiler using default ctor, or explicitly by the user)

This order is critical for the following reasons:
1. user-defined types are defined+initialized at the member list. this initialization
is important because the initialization of the classes in mib is done via a constructor:
if m_Name is a class member, and the initializer list contains "m_Name(name)" which is equivalent to m_Name = name, 
this is not an assignment operation since m_Name is not yet created (they were declared, but not defined+initialized).
therefore a constructor is called depeding on the rhs:
on the value being assigned:
	1. if the assigned value is rvalue reference (&& or std::move) -> move constructor invoked
	2. if the assigned value is an lvalue with the same type of the object being assigned to -> copy ctor invoked
	3. all others -> a call to some ctor depending on the value (implicit conversion - to a call for converting constructor)

6. in the body, after leaving the member initalizer list, the user-defined types are already defined+initalized,
all assignments to user-defined types in the body of the ctor are considered as assignment operations to an already created object!

*/

class Entity
{
private:
	int m_score;
	std::string m_name;
	int m_arr[5]{ 1,2,3,4,5 }; //default member initialization (at declaration)
	int m_t;
public:
	//despite m_name(name), m_score(x) , the initialization will be according to the order of 
	// decleration of class members may affect dependencies
	Entity(const std::string name, const int& x) : // member initializer list. 
		m_name(name), //defined and initialized 2nd
		m_score(x), //defined and initialized 1st
		m_arr(), // NOTE: ******member initalizer list wins over inline initlization of a member******:
		//due to the explicit initialization of m_arr in member init. list via "m_arr()" statement,
		//the inline initialization "	int m_arr[5]{ 1,2,3,4,5 };" will be discarded!
		//and the array will be initialized to all-zeros. if the statement "m_arr()"
		//was missing, then the inline member initialization "{1,2...}" would have been executed.
		m_t() //defined and initialized 3rd. t() means t initialize t with 0.
	{
		m_score += 1; //code in the ctor is easy to read
	}

	/* replaces this
	Entity(const std::string& name, const int& x)
	{
		m_name = name;
		m_score = x;
		t=0;
		m_score +=1
	}
	*/

	const std::string& get_name() const
	{
		return m_name;
	}
};

int main()
{
	Entity e1("shalom", 2);
	std::cout << e1.get_name() << std::endl;


	Driver e;
	//std::cout << t.y << std::endl;
	//Entity e2;
}