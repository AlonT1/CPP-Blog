
/*
with copy ctor we first check if the current object and the
object we copy from are different and then we 
deep copy all the fields from the object we copy from into the fields of our object.
in the end, we return *this to enable chaining of "=" operations (e.g: a=b=c)
-------------------------------------------------------------------------------
Rule of thirds: Every object in a well-designed C++ system has a default constructor, a copy constructor, and an assignment operator.
-------------------------------------------------------------------------------
*/
#include <iostream>

class Test
{
private:
	int x, y;
public:
	Test(int x, int y) : x(x), y(y) {}
	void print() { std::cout << x << " " << y << std::endl; }
};



class TBar
{

};

class TSuperFoo
{
private:
	int x;
};


class TFoo : public TSuperFoo
{
public:
	TBar* fBar1;
	TBar* fBar2;

	/*tl;dr
	the main point of a copy assignemnt operator is to copy data from an existing object into an already created
	object. The object we copy from is NOT DAMAGED and NOT MODIFIED by definition (it is const for a reason).
	we DEEP copy all members from the object we copy from into the new object without modifying the object we copy from.
	*/
	TFoo& operator=(const TFoo& other)
	{
		if (this != &other) // ensuring that the lhs and rhs are different! this is a "cheap" address comparison
		//as opposed to the slower (*this==other) which invokes implicit/explicit equality operator (compares class members)
		{
			TSuperFoo::operator=(other);// If TSuperFoo already has a copy assignment operator overloaded,
			//we can utilize it and provide it with "other" to initialize TSuperFoo portion of the current
			//object (int x of TSuperFoo) with the TSuperFoo portion of other (also int x). This saves us code,
			//instead of performing "this.x = other.x" here. Important: when "other" is supplied to TSuperFoo, copy
			//assignment operator it will be treated as a TSuperFoo and not as a TFoo (copy ctors receive objects by ref,
			//this supports polymorphism (usage of virtual functions), but the access to members will be limited to
			//members of TSuperFoo

			delete fBar1; //why? because we are about to assign fBar1 with a deep copy of "other"s fBar1 content
			//therefore the current data in fBar1 memory location must be deleted before assigning fBar1 with new data!
			//not doing this will cause a memory leak (unless fBar1 points to null), where we'll have data in memory
			//that nobody points to anymore! another explanation:
			//fBar1 points to 100, we must delete the data at 100 before constructing new data and pointing fBar1 to it
			//otherwise the data at 100 will leak, with no pointers to it
			fBar1 = new TBar(*other.fBar1); //using the copy ctor of TBar to deeply construct fBar1 
			//(new returns a pointer to a mem address to the newly created object)
			//what if we get an error when constructing fBar1? we will have a dangling pointer
			//to a deleted memory location (we used "delete fBar1"
			//in this case, reading or writing to fBar1 may cause a crash, therefore:
			//////////////////////////////IFFFFFFF POINTER!! CHECK NEW
			delete fBar2;
			fBar2 = new TBar(*other.fBar2);

			//Note: if the copy ctor is successful without exceptions for all statements, it is said that
			//we transactionized the whole assginemnt

			//better alternative:
			/*
			TSuperFoo::operator=(other);
			*fBar1
			*/

		}

		return *this;

		/*
		Important note: a=b is equivalent to a.operator=(b)  in this case we actually don't need to return
		anything since we just copy elements of b into a. 
		but when dealing with statements as a=b=c (chaining) which is equivalent to a.operator=(b.operator=(c))
		we have to return a value -> b.operator=(c) returns a ref to a Vector2 which later evaluated
		in a.operator=(the returned value). even if lhs & rhs are equal, returning *this is valid.

		in addition we return *this since we want to return the data itself and not a memory address
		and return an lvalue reference to the newly created object to avoid creating new data (lvalue can be assigned with an lvalue).
		the relationship between the *this and the return value is like an assignment (String& in an lvalue reference to *this).

		Another reason for returning *this is because the signature of the overloaded copy-assignment function signature (convention)
		which requires to return an lvalue ref to the created object, for reasons stated above
		*/
	}

};

int main()
{
	TFoo e;
	TFoo b;

	e = b; //copy - assignment operator (not initialization) since e & b are of equal types and e was already initialized above.
	
	int a = 5; //= is a syntactic sugar for initializing a, equivalent to int a{5}
	int x = 2;
	a = x; //copy assignemnt operator!


	/*
	if Derived1 and Derived2 inherit from Base and we would have initialized the following objects:
	Base b1 = new Derived1;
	Base b2 = new Derived1;
	b1 = b2 //will only call the the assignment operator of Base and not of Derived1
	since the assignment operator of Base is not virtual. this means that the class members
	of Derived1 will not be initialized, only the Base members will be initialized.
	this is called polymorphic slicing, since we have sliced the "derived"-ness of b1 and b2.

	Solution: we can mark Base's assignment operator as virtual (?)

	*/

	/*
	------------------------------------------------
	when a&b are of the same type, b is already initialized is and is Being initialized (about to be created):
	Entity a = b // this invokes the copy ctor (copy initialization)
	for objects that were already initialized, the = operator (a=b , a&b same type) means
	the copy-assignment operator, in other words, a.operator=(b).
	see 25_constructor
	------------------------------------------------
	*/
}
