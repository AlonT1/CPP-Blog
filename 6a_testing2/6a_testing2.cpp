#include <iostream>

struct A { virtual void foo() { }};
struct B: public A { virtual void foo() override { }};
void wr(B* b) {}

int main()
{
	A* a = new B;
	wr(a);
}


