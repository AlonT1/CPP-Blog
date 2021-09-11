
// must include the class, otherwise the compiler won't know what Array<T> is
#include "Array.h" 


//this serves as an example for a defintion for a function of a templated class.
//the class has only a forward decleration of getLength()
// member functions defined outside the class need their own template declaration.
template <typename T> //mandatory to declare the template used by the class again here when defining the function
int Array<T>::getLength() const // note class name is Array<T>, not Array. when defining functions inside the class
//we user Array without <T>, look at the ctor inside the header (Array.h) file for example
{
	return m_length;
}

/*
Notes:************************************

Note 1:
	// declaration
	template <class T>
	class C;

	// definition
	template <class T>
	class C {};

	We use the non-templated version of Array INSIDE the class,
	for example the ctor of class would be Array() and not Array<T>(), 
	and the return value of functions won't be Array<T> but rather Array.
	Inside the class using Array<T> or Array is equivalent. But, When forward declaring
	a function inside a class and defining it OUTSIDE the class (like above) we must declare the template
	and again declare the function by using the templated version Array<T>, otherwise the compiler
	would think we are defining a non-templated getlength() function version of "Array":

	template <class T>
	int Array<T>::getLength() const // note class name is Array<T>, not Array
	{
	  return m_length;
	}

	that being said all other classes that utilize the template parameter T must explictly
	declared with a <T>, for exmple an a ctor that accepts an std::initializer will be defined as:
	Array(std::initializer_list<T> list), note the "<T>"

Note 2:
	A template is not a class or a function -- it is a stencil used to create classes or functions:
	1.template declaration (incomplete type?):
		template <class T>
		class C;
	2.template definition (defined/stenciled out at compile time):
		template <class T>
		class C {};
	3. Array<int> intArray(12); --------> template type used to instantiate the template

*/

