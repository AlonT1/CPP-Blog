#include <map>
#include <iostream>

class Entity
{
public:
	int x = 17;
	Entity() { std::cout << "ctor" << std::endl; }
	~Entity() { std::cout << "dtor" << std::endl; }
	Entity(const Entity& other) { std::cout << "copy ctor" << std::endl; }
	Entity(const Entity&& other)  noexcept { std::cout << "move ctor" << std::endl; }
	Entity& operator=(const Entity& other) { std::cout << "copy assignment" << std::endl; return *this; }
	Entity& operator=(const Entity&& other) noexcept { std::cout << "move assignment" << std::endl; return *this; }
};


Entity t()
{
	Entity z;
	return std::move(z);
}

int main()
{
	Entity&& w = t();
}