// due to "export" keyword  the module  (this file) becomes "module interface file" (analogous to header)
// i.e the module becomes "import"-able by other files/modules
// just like header files, module interface file offers a quick overview of what a particular module has to offer,
// just like the public interface of a class
export module roman;

/*
now we have 3 options (all 3 well make the entities usable by the importing class via "import roman"):
1. attach the export keyword to a function that is defined in the module.
2. attach the export keyowrd to a declaration, and define the function somwhere
else, either in the module file itself or in an implementation file that defines the function - 
this implementations files use the keyword "moudle roman", NOT "import roman" to define the function.
3. instead of repeating the export keyword for each function/variable, we can have a single
export block that contains either function declarations (which are implemented elsewhere, like
explained in (2)) or
function definitions.
*/

import <string>;
import <string_view>;
void test() {} //the module does not export this entity, thus it cannot be invoked by a consumer (who imports the module)
export std::string to_roman(unsigned int i); // the module exports this entity, i.e we can invoke it

// export keyword is only mandatory at the declaration. in the implementation module "from_roman.cpp"
// we are allowed to add "export" to the function, but it isn't mandatoy.
export unsigned int from_roman(std::string_view roman); 
//unsigned int from_roman(std::string_view roman); //error! from_roman already declared as exportable

export //export block - everything here is usable by the one who imports the module
{
	auto square(const auto& x) { return x * x; }
	const double lambda{ 1.303577269034296391257 }; // Conway’s constant
	// the same module interface file can also have the implementation (defined below).
	// the file that imports the module only interacts with entites that are exported
	// and not with the implementation! thus we don't violate ODR, i.e the implementation is imported.
	bool isOdd(int x); 
}

bool isOdd(int x) { return x % 2 != 0; } //the implementation of isOdd can be inside the module file.


// we can also export namespaces
export namespace math
{
	int square(const int x);
}

// defining the function in the namespace
int math::square(const int x) { return x; }

/*
alternative for implementing the namesapce - allows implementing
multiple functions without using math:: prefix
namespace math
{
	int square(const int x) { return x; }
}
 
*/

//module for classes
// export module roman; //remember to declare the module as module interface file!
export class Entity // equivalent to the header
{
	int x;
	void test();
};

void Entity::test() { std::cout << "test"; } //the implementation
