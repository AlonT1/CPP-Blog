#include <iostream>
/*
by implementing pure virtual functions (who don't have a definition) 
subclasses are forced to define them.

Interfaces (classes that contain only pure virtual functions )can help utilize polymorphism - 
where one type can represent multiple types: 
classes (e.g: keyboard, cardboardbox...) that implement an interface such as
IRecyclable (that contains pure virtual functions - isRecyclable()) are later can be put
inside an "IRecyclable obj" array and then we can execute obj->isRecyclable(). 
In the same way we can provide a concrete class (keyboard, bottle..) to a function that has an IRecyclable parameter.

Only member functions can be declared pure virtual (friend functions cannot be virtual,
they are not part of the class)

although classes that have even one unimplemented pure virutal function cannot be instantiated
(because they aren't completely concrete), we can have a pointer to that class - this
enables polymorphism.
*/



class IPrintable // I for interface
{
public:
	virtual std::string get_class_name() = 0; // pure virtual function therefore, we can't instantiate this class
	//pure specifier (=0) allowed only on virtual functions
	//pure virtual functions MUST implemented by the inherited class! (interface = contract = promise to implement)
	//otherwise if the pure virutal function is not implemented in the inherited class, it 
	//will be considered abstract class - impossible to instantiate. classes that contain 1 or more
	//non-implemented pure virtual functions are considered abstract classes
};

class Entity : public IPrintable 
{
public:
	//sometimes we don't want to provide default implementation
	virtual std::string get_name() = 0; // pure virtual function, we can't instantiate this class
	std::string get_class_name() override //interface implementation
	{ 
		return "Entity"; 
	}; 
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
	player is of type IPrintable,  it is was initialized by "new Player" (polymorphism)
	*/

	Entity* player2 = new Player("shalom");
	

	// both statements below implement virtual functions and know that this is a player thanks to "new player"
	print(player); 
	print(player2);



}
