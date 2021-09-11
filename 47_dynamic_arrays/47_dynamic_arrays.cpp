/*
stl - standard template library - the underlying data type is up for us to decide ("template")
vector (just like a math vector) array - doesn't have a set size. every time we put an element into it - 
the array is copied to a new location in memory while the old location is wiped.
std::vector is a sequence container, as opposed to associative containers such as map
(which stores key associated with a value (key-map)
std::vector replaces heap allocated arrays via "new"
std::array replaces stack allocated arrays
*/
#include <iostream>
#include <vector>
struct Vertex
{
	float x, y, z;
};

std::ostream& operator<<(std::ostream& ostream, const Vertex& vertex)
{
	return ostream << vertex.x << ", " << vertex.y << ", " << vertex.z;
}

void function(const std::vector<Vertex>& other) //pass by reference!!! (performance)
{

}


std::vector<int>& vec()
{
	/*std::vector<int> a; //this will die in this scope!!! although a is based on heap allocated array,
	the object itself is on the stack - the data in the object, including the class reference itself
	will die when going out of scope, thanks to RAII and smart pointers
	the vector is designed to be killed when it goes out of scope
	see 26_destructor*/
	std::vector<int>* a = new std::vector<int>(); // here all the instance is allocated on the heap - dynamic storage specifier!
	//isn't bound to the current scope
	a->push_back(4);
	a->push_back(3);
	return *a;
}


int main()
{
	Vertex vertices[5]; // static sized array on stack - size cannot be changed
	Vertex* vertices1 = new Vertex[5]; // static sized array on heap - size cannot be changed
	Vertex* vertices2 = new Vertex[50000]; // huge allocation.. but maybe we will use only 2 verts?

	std::vector<Vertex> vertices3; //THE vector! supports primitive types such as int, float (in contrast to Java's Integer..)
	/*
	std::vector<Vertex> vertices storing "inline" allocated vertex  vs  std::vector<Vertex*> vertices
	the former is technically more optimal - vertex objects are stored inline - ONE AFTER ANOTHER without fragmentation
	(same cache line). the latter are pointers stored contiguously which points the verteices
	that are scattered in various places in memory (not one after another).
	pointers have an overhead of a extra jump in memory.
	the downside in the first example is when we want to resize the vector, it needs to copy all the data.
	with pointers the memory stays intact, and when the resize occurs, only the memory addreses
	are copied into a new array, but the data stays in place.
	heap allocated also need to be cleaned by "delete" while stack objects are scoped
	Rule of thumb: try to store objects not pointers..
	*/

	vertices3.push_back({ 1,2,3 }); // equivalent to vertices3.push_back(Vertex(1,2,3))
	vertices3.push_back({ 4,5,6 });

	for (int i = 0; i < vertices3.size(); i++)
		std::cout << vertices3[i] << std::endl; // [] is overloaded in vector to get the elements

	// for each - less effective since each "Vertex" is copied(!) into the scope of the for loop!
	// we want to avoid copying when possible so we add Vertex& (ampersand), meaning we want an lvalue reference,
	//to v (alias for v itself, not a copy of it)
	for (Vertex& v: vertices3) 
		std::cout << v << std::endl;

	vertices3.clear(); // set the vector's size to 0
	vertices3.erase(vertices3.begin() + 1); //erase the second element
	
}

/*
In some cases the ctor of vector doesn't work as expected:

class TreeFactory
{
public:
	Tree tree1, tree2, tree3;
	Tree trees[3]{ tree1, tree2, tree3 };

	//building a vector from an array
	//note that we initialize the vector inline at declaration. this statement
	//raises an error that the Tree isn't a type (for some unknown reason)
	std::vector<Tree> tree_models(trees, trees+3); 

	//A fix is to initialize the vector in the member initializer list of the ctor
	TreeFactory(): tree_models(trees,trees+3) {} // works!

};


How to use std::find/std::find_if with a vector of custom class objects?
You have to overload operator== 
https://thispointer.com/using-stdfind-stdfind_if-with-user-defined-classes/

*/