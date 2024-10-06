#include <iostream>

/*best practices - tl;dr -
****************************************************************************
1. use scope resolution operator :: - be explicit (do not rely on ADL)
2. with friend functions, declare them inside the class and define them outside (extroverts
making them visible to some namespace, enabling the usage of :: to look them up.
its also more organized because friends are not member of the class, simply "friends" with them.
*********************************************************************************
lower priority:
3. friend functions that are defined inside (not recommended as said in "2")
the class are lexically in the scope of the class but are invisible to look up via ::
(since friends are not members of the class)
we can reach those functions either via a forward declarations or ADL - :: is not option
since they are invisible to the name /scope of the class.
4. ADL with templates work only with invoking a templated function via type deduction.
using an explicit type requires using :: operator or forward declaring the template
(as said in 1, better to use :: anyway then rely on ADL.)

End of tl;dr*/


/*
Name lookup is the procedure by which a name(identifier) is associated with the declaration
that introduced it. Name lookup comes in two forms:

1. Qualified name lookup - a name that appears right to the scope resolution operator "::".
appears in the following form:
(scope - optional)::name
a. scope - name of a namespace, class, a struct, any cpp construct that can group data
together and prevent name conflicts. scope name is optional - it is missing
it means that we refer to the global namespace, for example:
::func() means calling func() defined in the global namespace).
b. name - the identifier of a class member, a namespace member or enumerator,
all defined in the scope we refer to via  operator :: . examples:*/

int t = 5;
int m = ::t; //t is qualified to the  global namespace where its declared and defined

namespace Z { int t = 0; }
int k = Z::t; //t is qualified to the namespace Z where its declared and defined


/*2. Unqualified name lookup - when a name is used without scope resolution operator.
categroized into two:

a. variables:
The lookup is performed an an inside-out style: the variable is looked up in
the innermost scope (namespace, class, struct...) it is defined with, and checks
the scope it is nested in all the way up until the global namespace. examples:*/

int x = 4;
int w = x; // look up finds x in ::x (global namespace)

int n = 1;
namespace N
{
	int m = 2;
	namespace Y
	{
		int x = n; // lookup finds n in ::n (global namespace)
		int y = m; // lookup finds m in ::N::m (inside namespace N)
	}
}

/*
b. functions - for unqualified function calls:
i. if they do not have arguemnts: they are implictlylooked up in the global namespace,
sometimes called "normal lookup", for example:*/
int g_func() { return 2; }
int res = g_func();


/*ii. if they do have arguments: Argument-dependent lookup, or Koenig lookup,
is used. ADL is a mechanism for looking up (associating) unqualified function
names (identifiers) in function-call expressions, including implicit function calls
to overloaded operators with the function definition -
*****all with the help of the namespace of the function-call arguments.****

how ADL works (how unqualified functions are associated with the declaration):
if the the function call has user-defined arguments (no pod's),
the function is looked in all the namespaces that are connected directly between
the global namespace and the namespace of the argument.
for example if 

namespace A
{
	struct Entity {};
	int func1(Entity& e) { return 2; }
	namespace B
	{
		int func2(Entity& e) { return 3; }
	}
}

A::Entity e;

//because e is of type Entity and qualified to A, ADL will look for func1
//in the global namespace, and if it isn't there, a look up based on the argument
//"e" namespace will be performed - searching for func1 in namespace A where it is found
int res2 = func1(e);


/*Notes:
1. ADL performs functions lookups mainly in namespaces with one exception:
ADL CAN look up FRIEND  functions (defined inline or outside) (template/non template)
inside classes, and only friend functions (makes sense since friend functions are
not member of the class, although they are scoped to that class, but invisible
to lookup via ::, rendering the "scoping" as impractical). for example:*/
class Entity1
{
public:
	struct Dummy {};
	friend int test(Dummy& dummy) { return 2; }
};

Entity1::Dummy d;
int res3 = test(d); //adl activated due to argument + friend function in class


/*2. ADL searches the global namespace and the namespace associated with the argument,
it does not drill up or down namespaces as. for example:
*/

//int res4 = func2(e);  //error!
//ADL will not drill down from namespace A to namespace B
//to associate the call with func2 from namespace B. ADL will only search
//the global namespace, an the namespace associated with the argument "e".


/*
General look up guidelines:

1. with member functions(templateand non - template) of classes, invoke them
through an instance. example:*/

class Entity2 { public: int func() { return 3; } };
Entity2 e2_instance;
int res5 = e2_instance.func();

/*2. with namespaces, simply use qualifcation(::), for example:*/
namespace N1 { int func2() { return 5; } }
int res6 = N1::func2();


/*3. if we don't want to use :: with a namespace (not recommended):
if the function in the namespace is a non-template we can utilize adl
as shown above, but if the function is a template function, adl only works
when calling the function via type deduction:*/
namespace B
{
	struct Entity {};
	template <typename T>
	int b_func1(T& e) { return 2; }
	int test4() {}
}
B::Entity entity_b;
int res7 = b_func1(entity_b); // ADL template lookup works only via type deduction
//int res8 = b_func1<B::Entity>(entity_b); //ADL not works with explicit types 


//*********in any case USE :: with namespace - its more clear!
int res0 = B::b_func1<B::Entity>(entity_b);


/*
4. when dealing with friend / non - friend functions(only in classes) :
if the friend function(template / non template) is an extrovert(declared in class,
defined outside), normal qualified lookup occurs, before even needing ADL(finding the function
at global namespace), for example:*/

class FriendZone
{
public:
	struct D {};
	friend int introvert(D& d) { return 3; }
	friend int extrovert();
	friend int introvert1() { return 4; }
};
int extrovert() {}

int res10 = extrovert(); //extrovert found by normal lookup in global namespace


/*we can utilize explicit qualification via :: if the extrovert
is nested in a namespace.*/

/*5. if the friend function(template/non-template) is an introvert inside a
namespace or a class, it can be found via adl(when using type deduction).
or via forward declaration:*/
FriendZone::D fz;
int res11 = introvert(fz); //introvert via adl

int introvert1();
int res12 = introvert1(); //introvert via forward declaration


int main()
{
	//Bonus: ADL and operator overloads (specifically operator<<)

	//Preface about std::cout
	//std::cout has multiple overloads categorized into two:
	//1. member function overloads - these belong to class std::cout and take in
	//manipulators such as std::boolalpha, std::endl. this makes sense since
	//the manipulators MANIPULATE the cout (standard output stream wired to the screen):
	//e.g: "std::cout<< std::boolalpha" translates to "std::cout.operator<<(std::boolalpha);"
	//2. free functions - also exist in std namespace (so is cin),
	//and takes in strings and pods (everything else except manipulators?):
	//"std::operator << "shalom";" translates to "std::operator<<(std::cout, "shalom");"
	//as we can see std::operator<< deosn't belong to any specific class.


	//ADL examples:
	/******************************Example1**********************************/
	std::cout << "shalom\n";
	//therefore the statement actually translates to:
	std::operator<<(std::cout, "shalom\n");

	/******************************Example2**********************************/
	std::cout << std::endl;
	// translates to:
	std::cout.operator<<(std::endl); // ok!

}