/*
optimizing std::vector 
basic rule of optimization - know your environment
std::vector works by creating it and pushing it. if the size of the vector is not large enough,
it allocates new memory for the new element and the old and copies the old element+ new into
a new location - called reallocation - slow!
how can we avoid copying objects (not vector of pointers)!
*/

#include <iostream>
#include <vector>

struct Vertex
{
	float x, y, z;
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
	Vertex(const Vertex& other): x(other.x), y(other.y), z(other.z)
	{
		std::cout << "copeied!" << std::endl;
	}
};

class Test
{
private:
	int m_i;
public:
	Test(int i) : m_i{ i } {}
	int get() const { return m_i; }

};


int main()
{

	//*************** parameters of push_back vs emplace_back

	/*
 	Test takes in a ctor with a single element, hence we create 3 different Test objects
  	otherwise, for multiple argument construcotrs, we must instantiate the object
	pass it to the push_back, e.g:
	vec.push_back(Test(6, 57))
	*/
	std::vector<Test> vec = { 21, 45 , 36 };



	//***************UNOPTIMIZED VECTOR

	std::vector<Vertex> vertices; // capacity of the vector is 0 

	// Vertex does not support a single-parameter constructor, so we must construct the object
	// and pass it to the push_back function
	vertices.push_back(Vertex(1, 2, 3)); // 1 copy occurs
	// Explanation: When we construct Vertex(1, 2, 3), it is first created as a temporary object.
	// Then, it is copied from this temporary location into the vector's allocated memory using the copy constructor.
	// After this push_back, the vector's size is updated to 1, and its capacity is also at least 1.
	// No reallocation of previous elements is needed here because this is the first push_back.
	
	vertices.push_back(Vertex(4, 5, 6)); // 2 copies occur
	// Explanation: Since the current capacity is 1, pushing another vertex requires the vector to increase its capacity.
	// Typically, this means reallocating memory (often doubling the capacity to 2), and then moving the previous elements to this new location.
	// The first copy is for moving the previous Vertex (1, 2, 3) to the new memory location.
	// The second copy occurs as Vertex(4, 5, 6) is copied from its temporary location into the new allocated space.
	
	vertices.push_back(Vertex(7, 8, 9)); // 3 copies occur
	// Explanation: The vector's capacity (2) is exceeded again when we add this third vertex.
	// As a result, the vector reallocates memory, typically doubling the capacity (e.g., to 4), to accommodate more elements.
	// The first two copies are for moving the previous vertices (1, 2, 3) and (4, 5, 6) to the new location.
	// The third copy occurs as Vertex(7, 8, 9) is copied from its temporary location into the newly allocated space.
	// After this push_back, the vector's size is updated to 3, and its capacity is now at least 4.
	
	// Note: EACH REALLOCATION requires copying all existing elements into the new memory location, one by one.

	//Total OF 6 COPIES 
	//std::vector<Vertex> vertices(3); //constructs 3 vertex objects using an explicit default ctor...


	////***************OPTIMIZED VECTOR
	std::vector<Vertex> vertices2;

	//optimization strategy number 1 - make enough memory for 3 verts (the amount we need) to avoid resizing itself twice
	vertices2.reserve(3); // we only need vertices! saves resizing and copying.

	//optimization strategy number 2 - emplace_back - tl;dr build the vertex directly into the vector, without
	//the additional copy that occurs when building it outside and providing it to the vector which causes a copy
	//(specifically a copy construction - because the object provided from outside is an prvalue).
	//instead of constructing Vertex in  the stack frame of the construcor function within a new allocated memory chunk
	//and then copying it back to main()'s stack frame where vertices2 is allocated,
	//we construct the vector in the constructor function, but we pass the memory address of vector of main() to the
	//stack frame of the ctor - RVO OPTIMIZED (?). Therefore the vertex is effectively constructed in the stack frame of main,
	//there is no more copying of the constructed Vertex objects from the stack frame of
	//the ctor to the stack frame of main.
	
	//emplace_back can accept multiple arguments, and the compiler will choose 
	//the appropriate constructor, as opposed to push_back which
	//supports implicit construction of only single-parameter objects.
	vertices2.emplace_back(1, 2, 3);
	vertices2.emplace_back(4, 5, 6);
	vertices2.emplace_back(7, 8, 9);
	//Total of 0 Copies! - Optimized. COPIES ARE BAD!
}
