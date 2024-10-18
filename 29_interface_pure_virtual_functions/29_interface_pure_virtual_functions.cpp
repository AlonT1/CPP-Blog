#include <iostream>
/*
pure virtual functions work under 3 conditions mentioned in 28_virtual functions
(inheritance, implementation of virtual function by the child & polymorphic pointer to either stack/heap allocated instance of a class).

when implementing pure virtual functions (who don't have a definition)  - subclasses are forced to define them.

Interfaces (classes that contain only pure virtual functions ) can help utilize polymorphism - 
where one type can represent multiple types. Advantages:

1. polymorphic arrays: classes (e.g: keyboard, cardboardbox...) that implement an interface such as
IRecyclable (that contains pure virtual functions - isRecyclable()) are later can be put
inside an "IRecyclable obj" array and then we can execute obj->isRecyclable(). 
In the same way we can provide a concrete class (keyboard, bottle..) to a function that has an IRecyclable parameter.

2. Flexibility and extensibility: 
paymentProcessor.pay(Visa v); instead of supplying the concrete Visa, we can abstract Visa to CreditCard via interface
 – all credit cards will fit as long as they implement the interface
 
 3. provides encapsulation – only the interface methods are visible to paymentProcessor, Visa class methods remain 

 Note: all of this advantages are valid when using non-pure virtual functions as well!


Only member functions can be declared pure virtual (friend functions cannot be virtual,
they are not part of the class)

although classes that have even one unimplemented pure virutal function cannot be instantiated
(because they aren't completely concrete), we can have a pointer to that class - this
enables polymorphism.
*/


/*
Why is it called an interface? the interface can be thought of as a control panel of executable buttons (list of commands)
We can press a button (execute a function) through the interface (list of commands) without knowing the
actual circutry (implementation) of the interface (control panel).
for example through main() we instantiate an object that implements an interface, an then simply invoke the implemented functions
throught the object without knowing the implementation of thsee functions. So we are in essence interacting only with the interface.
Some describe interfaces as a contract = promise to implement, because without implementing the interface the class cannot be instantiated.
This is a side effect of the interface feature, not the root reason for its name.
*/
class IPrintable // I for interface
{
public:
	virtual std::string get_class_name() = 0; // pure virtual function therefore, we can't instantiate this class
	//pure specifier (=0) allowed only on virtual functions
	//pure virtual functions MUST implemented by the inherited class! 
	//otherwise if we have even 1  pure virutal function is not implemented, it 
	//will be considered abstract class - impossible to instantiate.
};

class Entity : public IPrintable 
{
public:
	std::string get_class_name() override //interface implementation
	{ 
		return "Entity"; 
	}; 
	// another pure virtual function:
	virtual std::string get_name() = 0;
};

class Player : public Entity
{
private:
	std::string m_name;
public:
	Player(const std::string& name) : m_name(name) {}

	//because we inherit from Entity, we have 2 options regarding pure virtual function get_name:
	//1. implement it here - then we can instantiate Player as a concrete class
	//2. declare get_name as pure virtual here also, Player will become abstract and uninstantiable.
	//3. not mentioning get_name at all (equals to bullet 2)
	std::string get_name() override
	{
		return m_name;
	}
	std::string get_class_name() override //interface implementation
	{ 
		return "Player";
	} 
};

class Boss : public Entity //the implementation of pure virutal / virutal functions is not required!
{

};

void print(IPrintable* obj) // Iprintable interface demonstration
{
	std::cout << obj->get_class_name() << std::endl;
}

void print_name(Entity* entity) // 	virtual std::string get_name() = 0; demonstration
{
	std::cout << entity->get_name() << std::endl;
}

int main()
{
	//Entity* e = new Entity; not possible for classes that contain only virtual functions (abstract methods)
	IPrintable* player = new Player("shalom");// Iprintable* reference type (based on class rather on a primitive type), Player object instance
	/*
	Because the class reference is of type "IPrintable" we can only access the data that is available in IPrintable.
	the class members of Player are inacessible. if we execute by player a virutal function that was declared in Iprintable
	and overriden in Player - the function in player will be executed! this is because although
	player is of type IPrintable,  it was initialized by "new Player" (polymorphism)
	*/

	Entity* player2 = new Player("shalom");
	

	// both statements below implement virtual functions and know that this is a player thanks to "new player"
	print(player); 
	print(player2);
}


/*
******C# vs CPP interface and abstract******

C# interface
public interface IVehicle // no interface keyword in cpp
{
	// no fields are possbible, only method signatures, events (no implmenetation) and indexers (no implementations
	void Accelerate(); // C# interface functions are implicitly public and abstract
}

C# abstract
public abstract IVehicle // no abstract keyword in cpp
{
	int speed = 0; // can have fields
  	// pure virtual (=abstract) function. Only abstract declared classes or interfaces can have abstract methods
   	// an inherting class of interface/abstract class in C# MUST implement all abstract methods, unless the
    	// inherting class is declared abstract is well
	abstract void Accelerate();
 	void Drive() { Console.WriteLine("Driving"); }
}


In cpp we don't have "abstract" or "interface".
Want a C# equivalent interface? have a class with only pure virtual functions
Want a C# equivalent abstract class? have a class with 1 or more pure virutal functions with optional fields/defined methods
public IVehicle
{
public:
	int speed = 0; // can have fields
	void Drive() { std::cout<<"Driving"; } 
	// equivalent to c#: "public abstrac void Accelrate();"
 	// 1 or more pure virtual function (aka abstract) makes this class an abstract class:
	// inheriting classes do not have to implement this, but if they won't they will become abstract too.
 	//  eventually we'll be able to instantiate only the class that defines all the virtual functions down the inheritance chain,
	virtual void Accelarate() = 0; 
}

*/
