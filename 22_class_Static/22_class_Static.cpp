#include <iostream>

//private members in classes are accesible outside of the class by getters setters
struct Entity //public by default
{
	/*
	static member variables in cpp have the following properties due to design decisions:
	1. They are forward declaration (incomplete type) (albeit, without extern keyword) - satisfies the compiler
	(you can compile safely, we promise that the definition exists somewhere, to be found by the linker).
	2. the forward declartion refers to a global varaible defined somewhere - linked by the linker.
	3. the forward declaration is namespaced to the class and belongs to it, and doesn't belong to any
	specific instance (shared across instances). */
	static int s_x, s_y;
	
	/*1. we can define them outside the class as examplified below, only then they will
	become a complete (defined) type

	2. inline(cpp17)/const/constexpr static int x = 4;
	The standard states that const/constexpr static ints are constant literals, meaning
	that 4 and x are equivalent - both are prvalues, don't take up space, no performance overhead,
	and both don't have an identity (memory address) - see 24_enums for similar techniques.
	https://stackoverflow.com/questions/50033574/why-initialization-of-static-member-variable-is-not-allowed-inside-class-but-ini?rq=1
	
	3. inline static int s_x = 4 //since cpp17 - inline allows to declare and define static
	members just like non static members.

	4. Not recommended because it isn't syntactically clear:
	static USER-DEFINED members such as classes, can be defined and returned from a function, e.g:
	class {	static MyClass c;  static MyClass get_instance() {return c;} }
	invoking Class::func() will construct the static "c" class member (probably due to return-by-value invoking
	a construction of the member c.
	This doesn't work for pod's:
	class { static int x;	static int func() {return x;} }
	invoking func() will cause a linking error because x isn't defined nowhere.
.
	Notes:

	1. even if we don't define the static members - we can still instantiate the class
	or use other defined static variables - as long as we don't invoke the undefined static members.

	2. static variables and static members are initialized at application startup
	(the standard guarantees that initialization of static objects (entities with sotrage duration, and a type -
	classes or pod's) will be done before use of any object or function from the translation unit)
	https://stackoverflow.com/questions/1421671/when-are-static-c-class-members-initialized (2nd answer)

	3.	static variables are initialized only once! for example:
	a local static variable in a function will not be redeclared or reinitalized
	even if the function is executed multiple times - although the variable is scoped
	to the function, it lives from the execution - termination of the program

	4. static member variable is shared across all instances of a class and are initialized
	before the execution of the program.

	5. forward declaration to undefined variables don't take up memory - they only raise
	error if they are invoked. in other words, we can instantitate the class even if s_x
	is undefined, this will go through compiltation becuase forward declartion satisfy the compiler
	(ensuring the compiler that a definition exist), but invoking it will raise a linker error
	(unresolved external symbol) because the linker couldn't the definition to link up the s_x forward declartion with.

	6. static variable can be declared + initialized inside one of the static or
	non static functions (just like in main()) of a class

	7. the order in which static members are initialized is not guaranteed
	this means that they can't reilably depend on each other. (though they
	are initialized before all other non-static variables).
	*/

	int x, y;
	Entity()
	{
		x = 1;
		y = 2;
	}

	int test()
	{
		//creating and returning a static variable from a method:

		//static vars are initialized at program startup before any other objects
		//and are stored in global-static section of the memory layout. They have 
		//internal linkage, block/file scope (block in this case, but their storage duration
		//is exectution-termination. This means that even though var is local-scoped,
		//we can return it because it will live until the termination of the program,
		//it won't die at the end of this scope like automatic storage variables (default locals).
		static int var = 4;
		return var;
	}

	void print() //as opposed to static methods, every non-static method gets an instance of the class as a hidden parameter
	{
		std::cout << x << ", " << y << std::endl;
		// a non-static method (print()) CAN call a static method (print_static(), but not vice versa.
		// in other words, an object is aware of the class (and its static methods) but a class isn't aware of its objects
		print_static(); 
	}

	static void print_static() //static methods can only communicate with static class members and static functions
	//since they only belong to the class itself and not to any specific object -> Entity::print_static() is ok!
	{
		//"this" keyword cannot be used in static functions, because this function belongs
		//to the class, and not for any specific instance.  
		static int x = 4;
		std::cout << s_x << ", " << s_y <<  std::endl;
	}
};

/*
***********user access control doesn't apply for defining private/protected/public forward declarations of classes via
scope resolution operator!*************
e.g: static member definition(can also be initialized, here we allocate the memory) (we can define s_x even if it is private via
the scope resolution operator. this only works for defining the static variable: if we want to perform a read/write operation
on the private variable then access control applies (e.g: std::cout << Entity::s_x; only possible when s_x is public, not private) 

 */

int Entity::s_x = 5; // this is the DEFINITION +initialization to 5 of the forward declaration of static member
int Entity::s_y; // unlike non-static members which are given garbage values (unless explicitly defined),
//global definition of static-members defaults to pod default value (s_y stores 0). This also apply to non static
//global variables, but does does not apply to inner-class static methods

static void print2(Entity e) //static here means belongs to this transation unit (internal linkage),
//it can read staic varialbes or class members. this is because if x is static, e.x is the same as Entity::x
{
	std::cout << e.x << ", " << e.y << ", " << e.s_x << ", " << Entity::s_y << std::endl;
}


struct Animal
{
public:
	static const int w = 4;
	static void walk() { std::cout << "walking"; };
};

struct Crocodile : public Animal
{
	static void bite()
	{
		walk(); // static methods are inherited, but can be invoked only through static member functions.
	}

	void breathe()
	{
		// a non-static method (print()) CAN call a static method (print_static(), but not vice versa.
		// in other words, an object is aware of the class (and its static methods) but a class isn't aware of its objects
		walk(); 
	}
};


//static vars are initialized at the start of the exectuion (before main) and die with it 
int main()
{
	Entity e;// without curly braces it relies on a constructor. if a ctor doesn't exist - compile error or garbage values
	//e{4} means x=4, y=0 (s_x, s_y are discarded). e{4,2} means x=4 y=2 , by order
	Entity::s_x = 5; // accessing static class members
	e.x; // instances can also access class members!
	print2(e);

	void(*function1)(void) = Entity::print_static; // function1 is pointer to a static function inside
	//a class which returns void and takes in void params

	Crocodile croc;
	croc.breathe();
}