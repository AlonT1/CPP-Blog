#include <iostream>
#include <vector>
#include <string>

void* operator new(size_t size)
{
	std::cout << "new\n";
	return malloc(size);
}

void test(std::string&& str) {}

int main()
{
	std::string a{ "shalom" };
	test(std::move(a));
}