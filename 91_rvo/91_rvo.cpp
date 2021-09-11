/*Return Value Optimization*/
#include <iostream>

class Entity
{
public:
	int x = 17;
	Entity() { std::cout << "ctor" << std::endl; }
	~Entity() { std::cout << "dtor" << std::endl; }
	Entity(const Entity& other) { std::cout << "copy ctor" << std::endl;}
	Entity(const Entity&& other) noexcept { std::cout << "move ctor" << std::endl; }
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
if rvo/nrvo optimization deosn't occur, the compiler will try to move back the lvalue,
if a move ctor doesn't exist, the compiler will try to copy back the lvalue, if
a copy ctor doesn't exist we will get an error because the compiler won't be able
to use the implict-declared copy ctor (considered deleted).
****************************************************************
*/

Entity rvo_uoptimized_func()
{
	Entity temp = Entity();
	return temp;
}

/*

*/
Entity rvo_optimized()
{
	return Entity();
}

int main()
{

	//rvo unoptimized
	Entity e_rvo_unpotimized; 
	e_rvo_unpotimized = rvo_uoptimized_func();

	//rvo optimized
	Entity e_rvo_optmized = rvo_optimized();



	/*
	with rvo:
	1."ctor" - main() stack frame is invoked, which invokes the stack frame of func() which
	in turn invokes stack frame of Entity ctor - inside this ctor's stack frame
	we construct the entity object right in the stack frame of main() into the address of variable "e"! (an address that belongs 
	to variable "e" in main()'s stack frame is passed via registers to the stack frame of the Entity ctor).
	2. "dtor" - returning back from the ctor stack frame, into the stack frame of func(), and back to stack frame of main(),
	the "dtor" of Entity is invoked (supposed to remove the entity that was built in main(), but doesn't
	really do anything because our dtor is empty)

	*/

	/*
	without rvo (prints):
	* possible of additional construction of e_rvo_unoptimized in main() stack frame
	1. "ctor" - starting in main() stack frame, we invoke (call) func() stack frame, and within func() the ctor()
	of entity invokes a stack frame of itself.
	inside the ctor stack frame  we create the Entity object in an address that belongs to the func() stack frame
	(done via passing a memory address of func() into the stack frame of ctor() via registers)
	2. "move ctor" - returning back from ctor() stack frame into func()'s stack frame(), the  move ctor stack frame is invoked
	in which we copy the object from func() stack frame back to main() stack frame (memory address of stacks are passed via registers)
	3. "dtor" - The object inside func() stack frame is now destroyed (in practice,
	because the dtor is empty, the assembly code doesn't do anything significant
	4. "move ctor" - now we return to main()'s stack frame(), in which we invoke move ctor once  again!
	the compiler, which already moved the data the object from the stack frame of func() into main() (step 2)
	now does it again - it manages to access the stack frame of func() and copy the entity object into  the address
	of "e_rvo_unpotimized" variable inside main()'s stack frame which now holds two identical copies of the
	Entity object in two different memory addresses.
	so in step 2 we moved the object from func()'s stack frame to main()'s stack frame, and now we peform the same operation
	to actually assign the object to variable e_rvo_unpotimized which also lives in the stack frame of main().
	5. "dtor" - dtor invoked, destroyes the temporary object from step 4 (the rhs of the assignemnt), this
	object has expression scope, going into the next statement causes its destruction (created by the first move ctor - 0x084)
	6. "dtor" - destructing the object created in step 6 "variable e_rvo_unpotimized" (created by the second move ctor - 0x092)
	*/

	/*
	see 89_move_semantics notes at the bottom - move_semantics enable to return from a function without redundant copies,
	like an rvo, but with more syntax (std::move and rvalue refs)
	*/

}
