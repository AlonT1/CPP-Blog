/*
casting (type casting) - cpp is a strongly typed language - types are enforced.
type conversion has 2 types:

1.
implicit type conversion (coercion, forced) -
*************tl;dr:
widening  - of integral to ints or widening of ints to a float (double/ float)
shortening - of ints to short/chars, or doubles to floats...
*************
automatic by the compiler, occurs
when 1 data type is expected but a different one is supplied.
has two sub types which operate on integral/floating family (safe - no data loss):

	1. promotion (widening) (won't result in data loss) of integral/floating data types:

		a. at initialization:
		double d{0.12f} //float promoted to double
		long l{64} // int promoted to long
		Enum is promoted to int

		b. when evaluating expressions:

			i. an operand narrower than an int undergoes integral promotion (e.g: short to an int).

			ii. if one of the operands is a floating point (double, float..) then all integral and floating types
			undergo a promotion to the highest-priority-floating-type in the expression:
			example: int a + float b + double c   -> all promoted to a dobule! and then the expression is evaluated.
			3/5.0  ->  3 is promoted to a double -> 3.0/5.0   (NOTE: FLOATS ARE DENOTED WITH "f" literal while double is not)

	2.  shortening - numeric convertions- conversion from a larger type to a similar smaller type (may result in data loss):
	char c {3000} // char has a range of -128 to 127, 3000 int causes an overflow and unexpected result
	short s {2} //2 is an int, narrowed into a short, no data loss
	double d{3} //integer 3 to to a double (different families)
	float f {0.1234} // double to wide-enough float


2. explicit type conversion - developer uses casting operators for conversions.
short w = 5    ->     int x = (int)w  (c style cast),  or by using the more modern static_cast


*/
#include <iostream>

class Base
{
public:
	Base() {}
	virtual ~Base() {}
};

class Derived : public Base
{
public:
	Derived() {}
	~Derived() {}
};


class AnotherDerived : public Base
{
public:
	AnotherDerived() {}
	~AnotherDerived() {}
};


int main()
{
	//c style casting
	int a = 5;
	double value = a; //implicilty converted by cpp, equals to double value = (double)a;

	double b = 2.4;
	int c = b; // implicit conversion with data loss, equivalent to int c = (int)b

	double d = a + b;
	std::cout << d << std::endl;



	/*cpp style casting :
	static_cast(no runtime checks, for type conversion, very similiar to c cast),
	reinterpert cast (for punning),dynamic_cast (for polymorphism /inheritance,
	evaluated and checked at run time (from one derived class to another)),
	const_cast (remove/add cost) the actual conversion uses c style casts. it adds syntax sugar to the code (more readable)
	adds compile time checking  (dynamic_cast performs a check and returns exceptions (slower))
	additional benefits other than compile time checks, is the ability to search them, as opposed
	to c style castings
	*/

	//static cast - very similar to c cast but more restrictive - the safest cast
	//------------------------------------------------------
	double e = 5.24;
	int s = static_cast<int>(e);
	std::cout << s << std::endl;

	//ch is 1 byte, p is int* which points to 1 byte of memory, manipulating 1 byte of mem
	// with an integer (4 bytes) will result in run-time error
	char ch = 'w';
	int* p = (int*)&ch; //type punning /cast
	//*p = 5; // causes an error
	//static cast is more restricitive...
	//int* anotherP = static_cast<int*>(&ch); //error! -  the static cast allows the compiler to check
	//if the pointer and pointee are of the same data type, which is something that the c cast doesn't do

	/*
	Static_cast does not implement RTTI therefore it is more risky but faster,
	unlike dynamic_cast it won't raise errors when downcasting a  class
	to a class it does not inherit from (which cause undefined behaviour)
	*/


	//reinterper cast - essentialy enforced type punning - reinterprets the underlying bit pattern
	//reinterpret_cast can only perform pointer-to-pointer conversions and reference-to-reference conversions 
	//(plus pointer-to-integer and integer-to-pointer conversions).
	//This is consistent with the intent expressed in the very name of the cast:
	//it is intended to be used for pointer/reference reinterpretation.
	//------------------------------------------------------
	int* anotherP1 = reinterpret_cast<int*>(&ch); //successful!
	//char c 1 byte memory block is now treated as if it was an 4 byte int - dangerous!

	//double dble = static_cast<AnotherDerived*>(e); ain't gonna work
	//double dble = reinterpret_cast<AnotherDerived*>(&e);

	//const cast (removes const):
	//------------------------------------------------------
	const int myConst = 5;
	int* nonConst = const_cast<int*>(&myConst);
	*nonConst = 10;
	//const cast is problematic with variables that are stored in read-only memory
	//such as strings which are inherently const



	//dynamic cast -  handles polymprhism, for downcasting  base-class pointers/refs into derived-class pointers/refs
	//possible because derived classes have a portion in their memory representing base
	//and due to RTTI the compiler knows that base pointer actually points to a derived
	//class x and therefore can be downcasted to a derived class x, but if base pointed
	//to derived class y, we couldn't downcast base pointer to derived class x.
	//------------------------------------------------------
	Derived* derived = new Derived();
	Base* base = derived; // implicit cast to Base

	//AnotherDerived* der = base;  // error because base points to "Derived" (AnotherDerived 
	//has no connection to Derived, although both AnotherDerived and Derived point to Base,
	//AnotherDerived and Derived don't know about each other, therefore they cannot point
	//to each other. Note that in the statement "AnotherDerived* der = base" base actually
	//points to derived, therefore "der" of type AnotherDerived cannot point to "base" of type Derived,
	//because "AnotherDerived" and "Derived" types are foreign.

	//. in this case we must cast base to AnotherDerived -> AnotherDerived* der = (AnotherDerived*)base
	// in the rhs we explicitly refer only to the "AnotherDerived" memory portion of base (of type Base
	//which is-a AnotherDerived.
	//although the casting is sucessful, accessing data that is exclusive to "AnotherDerived" class
	//will result in an error, since the underlying type is still the original "Derived" Class.
	//this conversion is successful in c style conversion or via cpp static_cast, but will fail in cpp dynamic_cast!

	/*
	but how does cpp knows that "base" points to a Derived class? cpp stores runtime type information (RTTI)
	about all of our types - this adds overhead but allows us to do dynamic casting. dynamic casting
	costs overhead since we need to validate thorugh the RTTI what is the real type of the object.
	(see 124_RTTI).

	RTTI Can be disabled -> Project properties -> C/CPP -> Language  -> enable run-time-type information -> off
	this removes the overhead!! but causes dynamic_cast not to work.
	*/

	Derived* ac = dynamic_cast<Derived*>(base); //casting base pointer (that points
	//to a derived instance) down to a Derived type. the compiler knows that base points
	//to a Derived type thanks to RTTI

	/*
	if Base* base = new Derived;  we can dynamically cast base to Derived type.
	but what if Base* base = new AnotherDerived ? then we CANNOT cast base to Derived!
	because Derived and AnotherDerived are foreign to each other (explained above).
	in this case cpp dynamic_cast will return a null pointer (0), since the underlying
	type of base is AnotherDerived. this is an advantage to c style raw cast.
	therefore we can use dynamic_cast check to see the underlying type of the class

	dynamic_cast is used to "downcast" (can also "upcast") the polymorphic pointer "base" (of type Base, but
	points to "derived" of type Derived),   to the underlying type that base points to which is Derived
	(derived was initialized with "new Derived", later pointed to by a Base pointer (polymorphism))


	downcasting using dynamic_cast will not work:
	1) With protected or private inheritance.
	2) For classes that do not declare or inherit any virtual functions
	(and thus dont have a virtual table). At least the dtor can be virtual to enable
	dynamic_cast (something is got to be virtual).
	*/

	AnotherDerived* ac2 = dynamic_cast<AnotherDerived*>(base);
	/*will fail since "base" pointer points to "derived" type class
	and not to AnotherDerived type class
	this can help us to test the "underlying" type of "base"
	Always ensure your dynamic casts actually succeeded by checking for a null pointer result.
	
	can be used in real time to know if base is a pointer to Mercedez or Lamborghini
	if(dynamic_cast<Mercedez*>(base)) or if(dynamic_cast<Lamborghini*>(base))
	
	although :
	"typeid(vehicle) == typeid(Lamborghini)" or  "typeid(vehicle) == typeid(Lamborghini)"
	is three times faster than the techqniue above.
	*/


	if (!ac) //If the cast fails, it returns a null pointer of the type inside <> (AnotherDerived)
	{
		std::cout << "fail" << std::endl;
	}


	
}
