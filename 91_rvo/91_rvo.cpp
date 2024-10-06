/*Return Value Optimization
tl;dr
trust the compiler to perform a return without performing a redundant copy.
no need to "move" back the object.
*/
#include <iostream>

class Entity
{
public:
	int x = 17;
	Entity() { std::cout << "ctor" << std::endl; }
	~Entity() { std::cout << "dtor" << std::endl; }
	Entity(const Entity& other) { std::cout << "copy ctor" << std::endl;}
	Entity(const Entity&& other)  noexcept { std::cout << "move ctor" << std::endl; }
	Entity& operator=(const Entity& other) { std::cout << "copy assignment" << std::endl; return *this; }
	Entity& operator=(const Entity&& other) noexcept { std::cout << "move assignment" << std::endl; return *this; }
};


/*
functions with a return value have an implicit 1 argument which is pushed to the stack
via "call" instruction in  assembly (the adress of the lhs ("e"),
where the return value will be stored). instead of creting Entity as a seperate object
and copying it back to x via copy constrcutor, entity() will be created in the location of e!
it works via allocating memory for the to-be-returned object in the caller’s stack frame. Tthis is the return
value optimizatio, done by the compiler (Can't be turned off in msvc).
It only works when the return value is known at compile time!
if entity() would have contain returns inside if statments whose values are known
only at runtime, the "return" will give back a full copy of the returned object

in clang rvo can be disabled with: -fno-elide-constructors

*/


// with pod's the of w is created on the register and returned to the stack frame of the caller
int simple()
{
	int w = 2;
	return w; //this is even more optimized then move/copy semantics due to rvo!
}

//note: rvo is always on in vstudio. this function simulates the output as if
//the rvo was off!

/*
The C++ specification has a special rule that says automatic objects (lvalues
such as "temp") returned  from a function by value can be "moved" even if they are lvalues. 
This makes sense, since temp is going to be destroyed at the end of the function 
anyway! We might as well steal its resources instead of making an expensive and unnecessary copy.
Although the compiler can move l-value return values, in some cases it may be
able to do even better by simply eliding the copy/move altogether 
and constructs the object right into the receiving address (e_rvo_unpotimized):
this might happen when:
1. the returned value is a prvalue (as seen in rvo_optimized()) (return value optimization)
2. Using NRVO (named value optimization), similar to rvo_untopmized_func() where temp
is actually named, its construction should be inside e_rvo_unpotimized,
but for some reason vstudio doesn't perform this optmization
In such a case, neither the copy constructor nor move constructor would be called.

****************************************************************
if rvo/nrvo optimization doesn't occur, the compiler will try to move back the lvalue,
if a move ctor doesn't exist, the compiler will try to copy back the lvalue, if
a copy ctor doesn't exist we will get an error because the compiler WILL seek 
a copy ctor but since a regular ctor exist, the implict-declared copy ctor is
considered deleted, therefore the copy ctor won't be found and an error will be
invoked.
****************************************************************
*/



/*
	with this version of MSVC, NRVO doesn't work with the function above.
	if it would have worked, the compiler would have replaced the code above
	with the following code:
	Entity rvo_unoptimized()ch
	{
	Entity temp = Entity() 
	//cast to &&, equivalent to std::move(temp)
	return Entity(static_cast<const Entity&&>(temp));
	}

	And the print would have been:
	ctor
	dtor

	note that code above would have worked even if only a copy ctor would 
	exist without a move ctor. if no copy ctor and no move ctor ---> error.
	ALTHOUGH A CAST TO A Rvalue ref occurs (equivalent to std::move)
	the actual move ctor is invoked "BEHIND THE SCENES", in other words, nothing
	prints out from the move ctor, and we can't reach it via the debugger.

	if we copy and paste the generated code and use it instead:
		Entity rvo_unoptimized()
		{
			Entity temp = Entity()
			return Entity(static_cast<const Entity&&>(temp));
		}
	we would see that the move ctor DOES fire up, but when using our original function,
	the move ctor runs IMPLICITLY - which means that if we have move ctor with errors
	in it, or alternitavely - a problematic ctor without a move ctor  - it can affect the NRVO process.
	Note that although the generated code casts temp to an rvalue ref and returns it,
	it seems that the returned type is converted to an rvalue ref, but stays an lvalue.
	

*/

//nrvo - named returned value optimization, the object we return has a name "x".
// returning an already-created object.
Entity nrvo_optimized()
{
	Entity x;
	return x;
}

Entity rvo_optimized()
{
	return Entity();
}

Entity move()
{
	Entity x;
	return std::move(x);
}

int main()
{

	//rvo optimized
	//Entity e_rvo_optmized = rvo_optimized();

	//nrvo optimized
	Entity e_nrvo_optimized = nrvo_optimized();
	
	//Entity e = move();

	/*
	with rvo / nrvo (single object construction) - running rvo_optimized() or nrvo_optimized():
	1."ctor" - main() stack frame is invoked, which invokes the stack frame of func() which
	in turn invokes stack frame of Entity ctor - inside this ctor's stack frame
	we construct the entity object right in the stack frame of main() into the address of variable "e"! (an address that belongs 
	to variable "e" in main()'s stack frame is passed via registers to the stack frame of the Entity ctor).
	2. "dtor" - returning back from the ctor stack frame, into the stack frame of func(), and back to stack frame of main(),
	the "dtor" of Entity is invoked (supposed to remove the entity that was built in main(), but doesn't
	really do anything because our dtor is empty)

	*/

	/*
	without rvo (prints), running the function (multiple object constructions):
	* possible of additional construction of e_rvo_unoptimized in main() stack frame
	1. "ctor" - starting in main() stack frame, we invoke (call) func() stack frame, and within func() the ctor()
	of entity invokes a stack frame of itself.
	inside the ctor stack frame  we create the Entity object in an address that belongs to the func() stack frame
	(done via passing a memory address of func() into the stack frame of ctor() via registers)
	2. "move ctor" - returning back from ctor() stack frame into func()'s stack frame(), the  move ctor stack frame is invoked
	in which we copy the object from func() stack frame back to main() stack frame (memory address of stacks are passed via registers).
	IF MOVE CTOR DOESN'T EXIST A COPY CTOR WILL BE USED INSTEAD.
	IF A copy ctor doesn't exist - error! because the regular constructor disables the implicitly declared copy ctor.
	3. "dtor" - The object inside func() stack frame is now destroyed (in practice,
	because the dtor is empty, the assembly code doesn't do anything significant
	4. "move assignment" - now we return to main()'s stack frame(), in which we invoke move assignment. 
	the compiler, which already moved the data the object from the stack frame of func() into main() (step 2)
	now does it again - it manages to access the stack frame of func() and copy the entity object into  the address
	of "e_rvo_unpotimized" variable inside main()'s stack frame which now holds two identical copies of the
	Entity object in two different memory addresses.
	so in step 2 we moved the object from func()'s stack frame to main()'s stack frame, and now we peform the same operation
	to actually assign the object to variable e_rvo_unpotimized which also lives in the stack frame of main().
	if the move assignment doesn't exist and the copy assignment operator exists - then it will be used instead.
	if both don't exist - error! since a regular ctor exists which implicitly deletes copy ctor.
	5. "dtor" - dtor invoked, destroyes the temporary object from step 4 (the rhs of the assignemnt), this
	object has expression scope, going into the next statement causes its destruction (created by the first move ctor - 0x084)
	6. "dtor" - destructing the object created in step 6 "variable e_rvo_unpotimized" (created by the second move ctor - 0x092)


	returning std::move when rvo is ON - running move() (when rvo is off, move() function
	yields the same result as seen above):
	1. ctor - constructing the object in the function move()
	2. move ctor - due to std::move + rvo the move ctor constructs the object
	in the stack frame of the caller main() - inside the memory of the accepting variable!
	3. dtor - of the object in 1
	4. dtor - of the object 2

	As we see std::move with rvo on disables the rvo process and is harmful!
	instead of constructing one object, the move semantics disable rvo
	and cause another object to be built

	*/


}
