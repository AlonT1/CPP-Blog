#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <typeinfo>

/*
1. ****Generic (non-specialized) Template Class Definition:
T is a template parameter inside a template decleartion
size is a non-type (non-generic) template parameter because it does
not substitute a type, (aka expression parameter: an int, ptr, ref...).
Stenciled out compile- time.*/

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


/*
2. ****Full Template Specialization of a Member Function:
Given a full specialization by client of Storage<double,15> (full because all params are specified),
we specialize (provide a vriant) print() for this case Reminder: user access controls isn't 
applied for defining members, even if print() was private it does apply when we try to use/call the members.
*/
template<> // providing explicit specialization, no template parameters to be specified.
void Storage<double, 15>::print()
{
	std::cout << m_value << " print() func of full sepcialized template <double,15> version of Storage class \n";
}



/*
3. **** Full template specialization of a "normal" (free, not in a class) function:*/
template <typename T, int size>
void test(T w)
{
	std::cout << size << " non-specialized templated version of a test() normal function \n";
}
//fully specialized version of the normal function  above:
template<>
void test<char, 5>(char w) //because we explicitly specialize the parameters here
{
	std::cout << "test() func full sepcialized version with <" << typeid(w).name() << ", "  << "version  \n";
}


/*
4. **Full Class Template Specialization:
Storage<bool, 15> fully specialized. i.e for bool and size 15, the class behaves differently:
*/
template<>
class Storage<bool, 15>
public:
	Storage()
	{
		std::cout << "i'm a boolean and int (full) specialization\n";
	}
};



/*
5. **partial class template specialization:
Template parameter T is specialized as bool, while size remains a non-type parameter
(can be any number)..*/
template<int size> // size remains a non-type parameter
class Storage<bool, size> // bool is explicit
{
public:
	Storage()
	{
		std::cout << "i'm a boolean (partial) specialization\n";
	}
};
// both statements stencil out the partial template above, because T=bool and int is any num.
Storage<bool, 5> storage1(); //
Storage<bool, 10> storage2();


/* 
6. ** Specialized isntance provided as a function parameter:
Given the following free function, "storage" is parameter taking in a specialized class instance
of a template which specializes T as explicit bool, but not size, left unspecialized
(can be any number), also called a partially specialized instance.
*/
template<int size>
void storage_pointer(Storage<bool, size>& storage) // T is now bool, but size remains unspecailized
{
	std::cout << "is a template non-type parameter, while bool is explicitly specified!\n";
}

Storage<bool, 5> myStorage(true); // instance where "true" refers to bool (see definition for usaage)
storage_pointer(myStorage);
// storage_pointer will take in any instance where T is bool, and the int can be any number!!!


/* 
7.**Template Inheritance to Reduce Code Duplication:
A "StorageDerivedNonSpecialized" can inherit from Storage base class (which is a non-specialized):
instantiation example: StorageDerivedNonSpecialized<double, 7> myStorageDouble(3.14);

*/
template <typename T, int size>
class StorageDerivedNonSpecialized : public Storage<T, size> {
public:
    StorageDerived(T value) : Storage<T, size>(value) {}
};

// Also "StorageDerivedPartiallySpecialized" can inherit from the base class Storage class
// instantiation example: StorageDerivedPartiallySpecialized<5> myStorageDouble(3.14);
template <int size>
class StorageDerivedPartiallySpecialized<double, size> : public Storage<double, size> {
public:
    StorageDerived(double value) : Storage<double, size>(value) {}
};

/*
8.** Limitations:
While classes can be fully/partially specialized, functions (member/free) can only be fully specialized!
so given the following template function:*/
template <typename T, int size>
void test2(T w) {
    std::cout << size << " non-specialized templated version of a test2() function \n";
}

// the following will yield an error:
template<int size> // error we must specialized "size as well, not only T as double
void test2<double>(double w)
{
	std::cout << "specialized partial function specialization";
}
