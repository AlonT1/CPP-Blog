/*
solving the diamond problem by virtual inheritance - only one copy of the top most class
will be created and shared among the middle of the diamond, this is done via vtable that
each class has and points to the relevant data of the top class - instead
of having CommonAncestor built twice and store inside each of the classes and the middle of the diamond,
this also solves ambiguity problems, becuase there aren't anymore two versions of the same functions inherited
to the middle classes.
all classes inheriting a virtual base class will have a virtual table, even if they would normally
not have one otherwise, and thus instances of the class will be larger by a pointer.
*/

#include <iostream>

class CommonAncestor
{
public:
	int m_age;
	/*
	virtual base classes are always created before non-virtal base classes, which ensures all bases
	get created before their derived classes -> THIS REQUIRES an explicit ctor that we can call from Child
	who is resposible for creating CommonAncestor, otherwise
	the compiler won't be able to construct CommonAncestor first
	*/
	CommonAncestor(int age) : m_age{ age * 2 } { std::cout << "CommonAncestor age: " << age << '\n'; };
	void makore() { std::cout << "makore" << '\n'; }
};

class Father : virtual public CommonAncestor
{
public:
	Father(int age) :
		/*
		this ctor is ignored when creating a child object, because when both Father and/or Mother
		virtually inherit CommonAncestor through single/multiple inheritance, only Child
		is responsible for creating CommonAncestor. However when creating a Father or a Mother object
		than these ctors will be used
		*/
		CommonAncestor(age)
	{
		std::cout << "Father age: " << age << '\n';
	};
};

class Mother : virtual public CommonAncestor
{
public:
	Mother(int age) : CommonAncestor(age) { std::cout << "Mother age: " << age << '\n'; };
};

class Child : public Father, public Mother
{
public:
	/*
	ctor without virtual inheritance. The compiler would have constructed CommonAncestor
	by calling Father ctor, Father would have called CommonAncestor ctor, same thing for Mother,
	and only then Child would been constrcuted.
	Child(int father_age, int mother_age) : Father( father_age ), Mother( mother_age )
	{
		std::cout << "Child age: " << father_age - mother_age << '\n' ;
	};
	*/

	//when dealing with virtual inheritance, the most Derived class is responsible for creating Base
	Child(int father_age, int mother_age) : CommonAncestor(father_age), Father(father_age), Mother(mother_age)
	{
		std::cout << "Child age: " << father_age - mother_age << '\n';
	};
};

int main()
{
	Child c(45, 32);
	c.makore(); // virtual inheritance solves ambiguity of the diamond problem - 
	//only one object of CommonAncestor exists, and Father & Mother point to it via tables.
	//(implementation hidden in vstudio, doesn't show the vtables)
	//without using virtual inhertiance both Father and Mother will hold a different copy
	//of CommonAncestor's data which leads to ambiguity (e.g c.makore() do we mean
	//c.Father::makore() or c.Mother::makore() - which memory section of c 
	//do we want to choose? the mother section or father?
	//with virtual inheritance, only one copy exists, thus c.makore() works
	/*
	without virtual inheritance - prints:
	CommonAncestor
	Father
	CommonAncestor
	Mother
	child

	with virtual inheritance - prints:
	Father
	Mother
	child
	*/

}