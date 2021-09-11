
//https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers


#include <iostream>

class Widget {};

template<typename T>
class Entity
{
public:
	T m_val;

	template<typename U>
	Entity(U&& val) : m_val(val)
	{
		std::cout << std::is_same<decltype(val), int&&>::value << '\n'; //underlying type of a is indeed int&
	};

};


template<typename T>
class Foo
{
	typedef T& LvalueRefType;
};


template <typename T>
void goo(T&& param) {}

int main()
{
	/*
	Universal (Forwarding) References:

	how do we recognize universal references:
	If a variable or parameter is declared to have type cv-unqualified (no const ano no volatile)
	T&& for some deduced type T, that variable or parameter is not an rvalue 
	reference but a universal (forwarding) reference. for example:
	*/
	int&& p = 4; //p is an int - type is fully specified - no type deduction occurs - && is rvalue ref
	auto&& q = 5; //type of q is deduced by the compuler at compile time => && is a forward reference
	/*
	template function:
	template <typename T> void f(T&& param) {}  //T is deduced by the compiler => T is forward ref
	*/

	
	/*
	what so special about universal references:
	***************************************************************************************************
	Universal references can bind to all value categories (very flexible!).
	***************************************************************************************************

	The "final type" that universal reference converge to (lvalue ref or rvalue ref) is based upon two steps:
	***************************************************************************************************
	****Step 1:  deduce "T" or "auto" according to the value category of the assigned expression. ****
	***************************************************************************************************

		1. If the value category of an expression initializing a universal reference is an lvalue,
		the universal reference becomes an lvalue reference.

		2. If the value category of an expression initializing the universal reference is an rvalue
		(prvalue or xvalue), the universal reference becomes an rvalue reference.

		Important Note: The auto deduction of T in templates or "auto" is done according to the
		underlying type and VALUE CATEGORY of the expression we provide (examples below)
		Note: xvalue expressions of type T are deduced to (symbolized by) T&&.
			  lvalues expressions of type T are deduced to (symbolized by)T&.
			  pvalues expressions of type T are deduced to (symbolized by) T.

		*decltype returns the underlying type of an expression when it's unparaenthesized: decltype(e)
		*decltype returns the underlying type and value category of expression when it's paraenthesized: decltype((e))
		essentialy the same deduction as in the above 3 points.
		for example:
		int x;  decltype(x) => int (underlying type of expression x is returned)
		int x;  decltype((x)) => int& (underlying type AND value category of x is returned) int& means that x's
		underlying type is int and the value cateogry of expression x is an lvalue. (int& here doesn't that
		the base type of x is an lvalue ref, but rather that the value category of x is lvalue, and an lvalue 
		category is deduced by the compiler as T&, specifically (int&) (as noted above). (read 118_decltype)


	***************************************************************************************************
			****step 2 (ref collapse): perform reference collapasing if necessary.****
	***************************************************************************************************

		reference collapsing rules exist because cpp doesn't have a reference to reference type therefore 
		when presented with a type T&, where T is an int&&  => int&& &  => collapse to int& (rule 3).
		the collapse rules:
		1. T& &   ==> T&
		2. T& &&  ==> T& 
		3. T&& &  ==> T& 
		4. T&& && ==> T&&
		*tip: & is infectious
		*T& or T&& means type and value cateogry of an expression after deduction (step 1) where T& is an lvalue,
		T&& is an xvalue and T is a prvalue (where ref collapsing isn't necessary because T && is simply T&& (rvalue ref)
		*bullet 2 is exactly rule 1 of universal reference above (T& represents an expression that has an lvalue category
		and it initializes a universal reference &&) as seen in example 1 below.
		*bullet 4 is exactly rule 2 of universal reference abbove (T&& represents an expression that has an xvalue category
		and it initializes a universal reference &&) as seen in example 4 below.
		*bullet 1 and 3 are irrelevant for universal references because when using universal references,
		&& are always trailing as seen in bullet 2 and 4 and T&/T&& represent the value category of the incoming expression
		where T& means lvalue expression and T&& means xvalue expression. Therefore bullets 1 (no && at all) and 3 (&& ain't trailing)
		are more relevant to decltype and typedef scenarios where reference collapsing occurs without universal references.
		*sometimes reference collapsing isn't necessary when dealing with universal
		references as seen in example 3

		reference collapsing occurs in  4 scenarios:
		2 scenarios involving universal references:
			1. template functions (either member functions (belog to class) or normal functions
			(outside class) where we declare template type T but use it as universal reference T&&.
			2. auto forward reference variables (auto&&)
		2 scenarios NOT involving universal references:
			1. typedef formation of template type T (T not necessarily forward reference)
			2. decltype


		*/

	int x = 2;
	auto t = 6; //auto always deduced to int
	std::cout << std::is_same<decltype(t), int>::value << '\n'; ////underlying type of var1 is indeed int&

	

	/**************examples of forward reference and reference collapsing when using "auto"
	
		the final type of universal reference is determined in the following steps:
		1. step 1 (T deduction): deduce "T" or "auto" according to the value category of the assigned expression
		*auto can be seen as T in templates (both are deduced at compile time).
		2. step 2 (ref collapse): perform reference collapasing if necessary.
		

		/***********example 1: rule 1 of universal refernce (initializaing universal ref with lvalue category expression)*/
		/**********************************************************************************
		step 1 (T deduction):	auto&& var1 = x;  => int& && var1 = x; (auto (=T) is deduced to int& due to x being lvalue expression)
		step 2 (ref collapsing):  int& var1 = x; (due to ref collapsing (rule 2 - T& &&))
		***********************************************************************************/
		auto&& var1 = x; //universal ref can become lvalue ref or rvalue ref - here it becomes lvalue ref
		std::cout << std::is_same<decltype(var1), int&>::value << '\n'; ////underlying type of var1 is indeed int&


		/***********example 2: rule 2 of universal refernce (initializaing universal ref with prvalue category expression)
		note: prvalue is a primary value category which is a member of the rvalue non-primary value category family
		Because 4's type is int and value category is int (prvalue) auto is deduced to it:
		**********************************************************************************
		step 1 (T deduction):	auto&& var2 = 4 =>  int && var1 = 4; (auto deduced to int - 4 is prvalue (int))
		step 2 (ref collapsing): (no reference collapse necessary)
		*******************************************************************************/
		auto&& var2 = 4; //universal ref can become lvalue ref or rvalue ref - here it becomes rvalue ref
		std::cout << std::is_same<decltype(var2), int&&>::value << '\n'; //underlying type of var2 is indeed int&&


		/***********example 3 : rule 2 of universal refernce (initializaing universal ref with an xvalue category expression)
		**********************************************************************************
		step 1 (T deduction):  auto&& var3 = std::move(x)  => int&& && var1 = 4; 
		(auto replaced by int&& due to std::move(x) being xvalue expression (int&&) )
		step 2 (ref collapsing): int&& var3 = 4; (ref collapse rule 4)


		*******************************************************************************/
		auto&& var3 = std::move(x); //universal ref can become lvalue ref or rvalue ref - here it becomes rvalue ref
		std::cout << std::is_same<decltype(var3), int&&>::value << '\n'; //underlying type of var2 is indeed int&&


	/**************Examples of forward reference and reference collapsing when using template functions
	(in the following examples the template functions are methods inside a class, but the same applies
	for normal (outside-of-class) functions */
		
		/*note: auto template deduction can't occur when templating a class:
		we must stencil out the class with a specific type: int, char, double, etc..
		this makes sense because the compiler read our minds and find out what we
		are class to be based to.
		on the other hand, the methods inside the class can utilize  auto deduction,
		for example the parameter of the ctor is a universal reference (U&&) because
		it is deduceted parameter type and is final form is based upon the value category
		of the provided expression during object construction call.
		The parameter U in the ctor is a universal reference and that grants the ctor
		the flexibility to accept all value categories: xvalue, prvalue, lvalues.*/

		/***********example 4 : rule 1 of universal reference - becoming an lvalue ref
		(initializaing universal ref with an lvalue category expression)
		**********************************************************************************
		step 1 (T deduction):U&& val = x; (in the ctor)  =>   int& && val = x; 
		(auto T deduced to int& due to x being lvalue (int&) expression)

		step 2 (ref collapsing): int& val = x; due to ref collapsing (rule 2 - T& &&)
		***********************************************************************************/
		Entity<int> e1(x); 

		/***********example 5: rule 2 of universal refernce - becoming an rvalue ref
		(initializaing universal ref with prvalue category expression)
		**********************************************************************************
		Step 1 (T deduction): U&& val (in ctor)  => (U deduced to int meaning that  4 is a prvalue) 
		Step 2 (ref collapsing): int && val = 4;  (no reference collapse necessary)
		***********************************************************************************/
		Entity<int> e2(4);

		/***********example 6 : rule 2 of universal refernce - becoming an rvalue ref
		(initializaing universal ref with an xvalue category expression)
		**********************************************************************************
		Step 1 (T deduction): U&& val = std::move(x)  => int&& && var1 = 4; 
		(U replaced by int&& due to std::move(x) being an xvalue)
		Step 2 (ref collapsing): U&& val = 4; (ref collapse rule 4)
		*******************************************************************************/
		Entity<int> e3(std::move(x));


		/***********example 7 : templating with explicit parameterizing the type
		****************************************************************************
		Step 1 (T deduction): T&& param = 4  => int && param = 4 (4 is a prvalue (deduced to int)
		Step 2 (ref collasping): not necessary

		hence the call "goo(4)", is equivalent to "goo<int>(4)"
		the paramaterized "int" is due to 4 being a prvalue
		****************************************************************************
		*/
		goo(4);
		//but what if we explicitly parameterize the type: goo<int&>(4)  - Error!
		//"int&" will cause the universal ref to converge to an int& (lvalue ref (int& && => int& collapse rule 2)
		//and lvalue ref cannot bind to prvalue 4

	/********examples of ref collapsing (NO UNIVERSAL REFERENCE) when using typedef in templates*******/
		Foo<int&> w;
		/*
		this causes "typedef T& LvalueRefType;" in Foo class to evaluated in the following way:
		typedef int& & LvalueRefType  (T replaced by int&)   =>  int& LvalueRefType; (ref collapsing rule 1)
		*/
		


	/********examples of reference collapsing (NO UNIVERSAL REFERENCE) when using "decltype"*********/

		/*decltype(y) yields int& (the underlying type of y, NOT the value category of y
		- read info about decltype above or in decltype project):
		**********************************************************************************
		Step 1 (T deduction):	decltype(y) && w = x;   =>   int& && w =x;  (decltype(y) deduced to int&) 
		Step 2 (ref collapsing) int& w = x; (final form)
		***********************************************************************************/
		int& y = x;
		//decltype(y) && w = x;

		/*Note that that the type : "decltype(e) &&" IS NOT A universal reference unlike "auto&&".
		decltype(e) && cannot bind to any value cateory - the reason is that decltype(e) is deduced
		according to the underlying type of the expression in the parameter therefore the final 
		form is sometimes mismatched to the expression on the rhs, as opposed to universal
		ref which is deduced to the value category of the object on the rhs*/
		int o = 2;
		//decltype(x) && z = o;  // error! 
		//decltype(x) && z = o;   =>    int && z =o  (no reference collapsing, but we can see
		//that o (an lvalue) cannot be binded to an rvalue ref. The point is that unlike auto&&
		//which can be binded to all values as a forwarding reference, we see here that delctype(e)&& 
		//cannot be binded to some values


}