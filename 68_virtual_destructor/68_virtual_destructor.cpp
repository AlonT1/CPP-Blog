#include <iostream>

class Base
{
public:
	Base() { std::cout << "base ctor\n"; }
	//~Base() { std::cout << "base dtor\n"; } // fit for non polymoprhic scenarios, problematic otherwise.
	virtual ~Base() { std::cout << "base dtor\n"; } //solves the issue explained below

	/*
	Note about pure virtual destructors
	virtual ~Base() = 0;
	they aren't overridable because they belong exclusively to the class they are in.
	therefore they can be declared as pure virtual but we must provide 
	a definition for them outside of the class:
	Base::~base() {}
	otherwise we'll get a linker error for not having a definition (we only satisified the compiler
	by declaring virtual declartion, the linker wants the definition).
	*/
};


class Derived: public Base
{
public:
	int* array;
	Derived() { array = new int[50]; std::cout << "derived ctor\n"; }
	~Derived() { delete[] array; std::cout << "derived dtor\n"; }
};

int main()
{
	Base* base = new Base(); //base ctor
	delete base; // base dtor

	std::cout << "-----------------\n";

	Derived* derive = new Derived(); // construction (outside -> inside): base ctor -> derived ctor
	delete derive; //destruction: (inside -> outside) derived dtor - > base dtor
	//works as expected until now

	//polymorphic example - where things get complicated
	std::cout << "-----------------\n";
	Base* poly = new Derived(); // construction (of new Derived()): base ctor -> derived ctor  - correct
	delete poly;//desturction : base dtor - > ********ONLY THE Base DTOR IS CALLED********
	//should be (derived dtor->base dtor - inside outside)
	//Reminder: the ptr limits access to members that belongs to the type of the pointer(members of Base),
	//(poly points to the Base portion of Derived). So instead of destroying derived portion first and base portion later
	//(as should be) only the dtor of Base is called (because poly ptr is of type Base), and THE DTOR of "Derived" is not called!
	//- MEMORY LEAK!  int* array  of Derived will not be deleted!

	/*
	Because the type of "poly" ptr is Base, and  the dtor of Base is not marked as virtual, cpp doesn't know 
	that there is additional destructor in derived. (with overriden virtual methods cpp knows with the help of a vtable to 
	execute the correct overriden function, also applies during the destruction of the object).

	*************************************************************************************
	WHENEVER WE WRITE A CLASS THAT SUPPOSED TO BE SUBCLASSED and be used in
	a polymorphic manner (pointer of type base to a derived object),
	WE MUST DECLARE THE DTORS OF THE CLASSES  WE INHERIT FROM (e.g: Base) AS VIRTUAL so that the compiler
	will also know to destroy the inheriting sub classes (Derived) during the deletion 
	of base class polymorphic pointers (poly ptr above), and prevent memory leaks as described above.

	Note that this is only critical with polymoprhism (references/pointers), with non polymorphism,
	for exmaple: "Dog dog; (where Dog inherits from Animal), when dog is destroyed,
	then Animal will be also destryoed, no need for virtual destructor.

	*************************************************************************************

	*/

	
}
