#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <typeinfo>

/*
tl;dr

we specialize a templated class/function to give an answer for a case where specific types/values
are used to instnatiate the template. e.g: if the client parameterizes a class "Storage"
with T as a double, and size as 15, we can specialize the class with these parameters,
offering another variation of the class which provides different services (functions, fields)
when these specific parameters are used.

1. type of template specialization:
a. non-specialized class/function templates - regular templates
b. full specialization of a class / member function / normal function
	*full specialization of a member function must be defined outside of the class
c. partial class specialization
	*(partial function specialization doesn't exist)
*partial/full specialization of a class won't grant inheritance (the non-specialized and
specialized classes are different entities and don't share members)

2. inheritance - non specialized
a. create non-specialized template A
b. create non-specialized template B which derives from non-specialized template A
inheritance - partial/full specialization (additional step to a,b)
c. create a partial/fully specialized template B which derives from partial/fully specialized template A

*/

// T is a template parameter inside a template decleartion
//size is a non-type (non-generic) template parameter - why non-type? because it does
//not substitute a type, unlike T. (also called expression parameter:
//an integral value, pointer, variable, reference...)
//*****THIS  TEMPLATE SERVES AS NON-SPECIALIZED TEMPLATE DECLERATION -
//The definition will occur when the template is stenciled out at compile-time****
//partial/full specialization of templates are geared towards specific cases
//where one or more of the template parameters are explicitly specified
template <typename T, int size> 
class Storage
{
private:
	T m_value;
public:
	void print_size() { std::cout << size << '\n'; }
	Storage(T value) : m_value(value) {}
	~Storage() {}
	void print() { std::cout << m_value << " print() func of non-sepcialized templated version of Storage class \n"; }
};


/*********** explicit (full) template specialization of a function belongs to a class (member)*************:
intended to create a function-of-class that is slightly different for a specific type (double in this case).
Note: member functions means that they belong to the class (excluding friend
which just gives the friend function access to private members), the  definition of member
functions can be either inside the class or outside of it via ::, but when dealing with 
specialization of member classes, the specialization must be defined outside the body of the class
using scope resolution operator
*/

//template<> means telling the compiler that there are no template parameters, since we are about
//to explicitly specify all types in the next line.
//note: we can also define/specialize private functions using the scope resolution operator, since
//user access controls isn't applied for defining members, even if print() was private
//(it does apply when we try to use/call the members)
template<> 
void Storage<double, 15>::print() //Full SPECIALIZATION FOR A DOUBLE CASE, size is 15
//called full template specialization, because all parameters (type and non-type) must become explicitly specified:
//a type T must become an explicit type such as a "double", a non-type parameter "int size" must become
//a literal int (we can specify "int keyword" instead of 15)

//Note: there is no access in code to the explicit specialized non-type template parameter "15". Some workarounds exist:
//https://stackoverflow.com/questions/1162401/is-it-possible-to-access-values-of-non-type-template-parameters-in-specialized-t
{
	std::cout << m_value << " print() func of full sepcialized template <double,15> version of Storage class \n";
}



/*********** explicit (full) template specialization of a "normal" function*************:
 (normal becuase it doesn't belong to a class, declared in global namespace):*/
//Assume non-specialized template function:
template <typename T, int size>
void test(T w)
{
	std::cout << size << " non-specialized templated version of a test() normal function \n";
}
//fully specialized version of the normal function  above:
template<> //this is empty....
void test<char, 5>(char w) //because we explicitly specialize the parameters here
////Note: there is no access in code to the explicit specialized non-type template parameter "15". Some workarounds exist:
//https://stackoverflow.com/questions/1162401/is-it-possible-to-access-values-of-non-type-template-parameters-in-specialized-t
{
	std::cout << "test() func full sepcialized version with <" << typeid(w).name() << ", "  << "version  \n";
}




/*********full class template specialization************/
//allows us to specialize a template class for a particular data type or data types
//if there are multiple template parameters in the orignial template form of the class
//in practice, this helps in having a class with only one name "Storage" which will
//act differently if instantiated with a bool parameter
template<>
class Storage<bool, 15> //full class template specialization (parameters must become explicit
//T becomes explicit type bool, int becomes explcitly specified integer 15
//Note: there is no access in code to the explicit specialized non-type template parameter "15". Some workarounds exist:
//https://stackoverflow.com/questions/1162401/is-it-possible-to-access-values-of-non-type-template-parameters-in-specialized-t
{
public:
	Storage()
	{
		std::cout << "i'm a boolean and int (full) specialization\n";
	}
};



/*********partial class template specialization************/
//why partially? because we didn't specialize the non-type parameter "int size" only bool was explicitly
//specified, size remained as a non-type parameter, not explictly specified as a literal, and can receive
//any int we want to specify
template<int size>
class Storage<bool, size> // bool is explicit, size is still a non-type parameter (partial specialization)
{
public:
	Storage()
	{
		std::cout << "i'm a boolean (partial) specialization\n";
	}
};


//aside from partially specializing a whole class, partial specialization also allows us to 
//partially specialize a class instance as an ARGUEMTN to a function:
//assume the  following "normal  function" (normal becuase it doesn't belong to a class, declared in global namespace):
// the function parameter "storage" is partially specialized here as "Storage<bool, size>& storage"
//origianlly (Storage<T,size>& storage)
//why partially? because we didn't specialize the non-type parameter "int size" only bool was explicitly
//specified, size remained as a non-type parameter, not explictly specified as a literal, and can receive
//any int we want to specify

//NOTE: THIS IS NOT A FUNCTION PARTIAL SPECIALIZATION - we didn't create another function called
// storage_pointer with some of the template parameters specialized!
// BUT RATHER we partially specialized the argument "storage" itself, no additional storage_pointer
// functions are created! partial function specialization is not allowed according to the standard!
// This allows the client to call this function via: "storage_pointer<5>(storage)
template<int size>
void storage_pointer(Storage<bool, size>& storage)
{
	std::cout << "is a template non-type parameter, while bool is explicitly specified!\n";
	//std::cout << storage.m_value; //vs studio doesn't warn private member access! why?
	/*The possible reason that vstudio doesn't warn us about this is because
	templates are stenciled out only when they are called as functions or instantiated as objects.
	It may be that vstudio doesn't detect the function calls to storage_printer(), therefore
	it assumes that the function won't be stenciled out, and therefore there isn't a reason
	to warn us about access to a private member. Only during compilation (when the function is actually
	stenciled out) we get an error of access to a private member.*/
}


/*********partial function specialization ************/
//partial specialization of a function is not allowed by the standard (overlooked ?)! only full specialization is allowed
//assume the following normal, non-specialized template function
template <typename T, int size>
void test2(T w)
{
	std::cout << "partial function specialization";
}

/*
//patially specializing the function yields an ERROR!!! (T specialized as double, size remains non-type parameter):
template<int size>
void test2<double>(double w) //error: illegal use of explicit template arguments (only double used in <>

{
	std::cout << "specialized partial function specialization";
}
//this also applies to member functions, we can't partially specialize print() of Storage, the following yields an error:
template<int size> //must perform full specialization, "size" still a non-type parameter - leads to partial specialization - error
void Storage<double, size>::print()
{
	//......
}
*/

//ONLY FULL FUNCTION SPECIALIZATION IS ALLOWED:
template<>
void test2<double,5>(double w) //now T sepcialized as double, and int specialized as 5
{
	std::cout << "specialized partial function specialization";
}


/********************Solving code duplication when fully/partially specializing templates *********************/
//when specializing a class we have to rewrite the code for the specialized class, just to give an answer for a case
//where specific, explicit types are needed (e.g: class Storage with T as a double, and size as 15).
//all of Storage's members are lost in the specialized class. inhertiance can solve this problem and reduce
//code duplication and increase reuse. 

//this also gives somewhat of an answer to the fact that we cannot partially specialize normal/member functions
//(e.g a print function in Storage template, cannot be partially specialize to take a double (replaces T) and 
//int size (not specialized, remains non-type param.):


//tl;dr - template inhertiance saves code duplication when fully/partially specializing templates:
// the trick is to use inheritance: create another class B that inherits from class A, and then 
// specialize both A and B, while both the specialized version of B inherits from A.
// so we:
// 1. original class A
// 2. new class B which inhertis from A (allows for B to inherit code of A)
// 3. both B and A are specialized and these specialized versions still inherit from eachother


//long version:
//1. create a base non-specialized template class - this will be our "Storage" declared and defined class.

//2. create a derived non-specialized template class that inherits from the non-specialized base class
//IN ESSEENCE WE CREATE A NON-SPECIALIZED-TEMPLATE INHERITANCE.
template <typename T, int size>
class StorageDerived : public Storage<T, size>
{
public: //public so it can be built, otherwise the implicit default ctor will be private
};


//3. fully/partially specialize the derived class and inherit from a  fully/partially  specialized base class
//SPECIALIZED INEHRTIANCE!
template <int size>
class StorageDerived<double, size> : public Storage<double,size>
{
public:
};

// because StorageDerived inherits from Storage, StorageDerived gets the code of Storage via inheritance.
// Because the specialized StorageDerived ALSO inherits from the same specialized "Storage"
// then the code of specialized Storage will ALSO be inhertied by the specialized StorageDerived




#endif
