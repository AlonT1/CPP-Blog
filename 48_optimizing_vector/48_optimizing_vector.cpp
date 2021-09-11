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

	//if Test class would not have supported a ctor with a single parameter, the 
	//following initialization of vec would have failed. for example, with 2 parameter
	//ctors, the compiler can't tell if by {21, 45, 36} we want to construct 
	//an object with values of {21,45} or {45,36}, or {21, 36}.

	//compiler recognizes int supporting ctor with SINGLE PARAMETER,
	//causing an anomaly of 3 identical m_i = 21, m_i = 45, m_i = 36
	//inside vec
	std::vector<Test> vec = { 21, 45 , 36 };


	//push back:
	//if the compiler has a single parameter ctor, we implicitly construct a Test
	//object by supplying a value that the single-parameter ctor of Test recognizes (int i).
	//otherwise, for multiple argument construcotrs, we must instantiate the object
	// pass it to the push_back, e.g:
	//vec.push_back(Test(6, 57))
	
	vec.push_back(43);


	//in terms of construction, emplace_back can accept a constructed object
	//AND multiple arguments - compiler will choose the appropriate constructor
	//from the templated parameter (from Test class).
	//for example std::vector<Test>, using emplace will use one of Test's ctors directly
	//it is also more performant then push back - explained below
	vec.emplace_back(43);



	//***************UNOPTIMIZED VECTOR

	std::vector<Vertex> vertices; // capacity of the vector is 0 

	//vertex does not spport single-parameter ctor, therefore we must construct the object
	//and pass it to the push_back function
	vertices.push_back(Vertex(1, 2, 3)); //1 copy - why? because when we construct Vertex(1,2,3) it is constructed
	//on the stack frame of construction function, and then it is copied from there into the stack frame of main
	//that the vector has allocated - this is done by copy ctor
	//capcity of the vector is update to 1
	//reallocation of old verts is not needed because this the first push back!
	vertices.push_back(Vertex(4, 5, 6)); //2 copies:
	//the reason for the 1st copy created by  the current pushback is listed above. after the push_back is completed,
	//the 2nd copy occurs because after finishing the the current push back, the capacity of the vector is updated to two,
	//this requires memory reallocation (the current location only supports one vertex),
	//so the previous vertex is recopied to a new location together with the current Vertex
	vertices.push_back(Vertex(7, 8, 9)); //3 copies - 1st for the current push back, and 2 more for the reallocation - 
	//after the current push_Back, the size of the vector is updated to 3, this requires 
	//copying the old two vertices into the new location where the current push back sits
	//EACH REALLOCATION REQUIRES THE COPYING THE Vertices ONE BY ONE! 

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
	//stack frame of the ctor - RVO OPTIMIZED. Therefore the vertex is effectively constructed in the stack frame of main,
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
