/*
initialization of a variable provides its initial value at the time of construction (not yet created obj).

global notes:
           compiler generates.....
Entity e;   -------------------------> Entity e = Entity();
Entity e(4);   -------------------------> Entity e = Entity(4);
Entity e{}; -------------------------> Entity e = Entity{};
Entity e(4,5); -------------------------> Entity e = Entity(4, 5);
Entity f = e; ------> Entity f = Entity(e);  <=> Entity f = Entity(static_cast<const Entity>(e));
int a;   -------------------------> int a;
int a{};   -------------------------> int a = {};

Entity entity2; // x,y get garabage values! equivalent in code to "Entity entity = Entity();
Entity entity{}; //implicit default ctor x and y initialized to 0. Compiles to "Entity entity = Entity{};"
"int a = {};"

*/

#include <iostream>
#include <map>
#include <vector>
#include <initializer_list>

class Entity
{
public:
    int x, y;
};

class Entity2
{
public:
    int m_x;
    explicit Entity2(int x) : m_x{ x } {} // explicit - explained below
};

class Entity3
{
public:
    int m_x;
    Entity3(int x) : m_x{ x } {}
};

class Entity4
{
public:
    int m_x, m_y, m_array[2];
    // by reference: avoid unecessary copies + cannot be null (safer). The problem: can change
    // the argument causing side effects, thus applying constant (safer for integrity of 
    // the object outside the function). want to change it? make a copy.
    // in addition, while references alone do not accept prvalues (temporary with no ID,
    // such as 5), the const attached pronlongs the lifetime of temporary values, allowing
    // to accept temporary initializer_list built with braced-init-list ("{}"),
    // which only exists for the duration of that constructor call.
    Entity4(const std::initializer_list<int>& list)
    {
        int index{};
        for (int x : list)
        {
            m_array[index] = x;
            ++index;
        }
    }
    Entity4(int x, int y) : m_x{ x }, m_y{y} {}

};

void func(const std::initializer_list<int>& test) {}

/************************constant initialization????????*************************
https://en.cppreference.com/w/cpp/language/constant_initialization
when initializaing constant static variables with a compile-time constant (such as 'a' or 4)*/
const int x = 5;

/************************Zero initialization**************************
https://en.cppreference.com/w/cpp/language/zero_initialization
occurs only with *global* variables, static and non-static, where they are 
initialized with default values without any explicit initialization
note that local vars without explicit initialization are assigned with garabe values*/
double array[3]; //zero-initialized to three 0.0's
int* pointer; // zero-initialized (null)
std::string s; //the default ctor notices zero-initialization and initializes to ""



int main()
{
    /************************Default initialization***************************
    https://en.cppreference.com/w/cpp/language/default_initialization
    syntax: "T object;" or "new T"
    an intialization performed when an object is constructed with no initializer
    (without = () or {}). Note that for global variables, a zero initialization
    IS the default initialization
    For auto (local) variables (auto because the storage is claimed automatically
    at the end of the scope) we can divide default initialization into 2:

    1. Default initialization for non global POD (plain old data- int, char, double....):
    leads to indeterminate values!
    */

    int x; // initialized with garbage (indeterminate) values!
    //int y = x; //undefined behaviour
    int array2[3]; // indeterminate values

    //2. Default initialization for user-defined objects (class, structs...) - 
    //uses declared default implicit constructor (which becomes defined when used here,
    //see 25_constructor)
    Entity entity2; // x,y get garabage values! equivalent in code to "Entity entity = Entity();
    //Entity entity2(); note that this is a function call (also called vexing parse)!!!!!!


    /************************Value initialization****************************
    https://en.cppreference.com/w/cpp/language/value_initialization
    when an object is constructed with an an empty initializer - done either
    via empty () or empty {}. Value initialization assigns default values!*/
    int a{}; // zero-initialized. Compiles to "int a = {};"
    Entity entity{}; //implicit default ctor x and y initialized to 0. Compiles to "Entity entity = Entity{};"


    /************************Scalar initialization*****************************
    https://en.cppreference.com/w/c/language/scalar_initialization
    initializaing a scalar type (int, char, floats...) with a single expression. Can be done via
    = (copy assignment) or {} */
    int b{ 2 }; // compiles to "int b = {2};"
    int c = 2;


    /************************Direct initialization (since c++98)***************************
    https://en.cppreference.com/w/cpp/language/direct_initialization
    initializes an object from explicit set of constructor arguments.
    Syntax: all variations with "()", also used when casting one object to another(?).
    works with explicit and non-explicit constructors*/
    int f(4); //compiler equivalent to "int a = 4;"
    Entity2 entity3(4); //this is the final code generated by the compiler (a construction call)



    /************************Copy initialization**************************
    https://en.cppreference.com/w/cpp/language/copy_initialization
    initializes an object from another object, can be used via "T object = other;",	
    via "T object = {other}, when passing/returning/catching/thrwoing-by-value.
    Nothe that copy initializaltion only works with non-explicit constructors, as opposed
    to direct initialization which works with both explicit and non-expicit ctors"*/
    
    //error! implicit conversion of 4 into Entity2 object can only happen if the ctor
    //of Entity2 is non-explicit, and because it is explicit no implict conversion can occur
    //when using copy initialization (or when using any other form of init. for that matter).
    //Entity2 entity4 = 4; 

    //implicitly converts "test" to std::string (ctor is non-explicit) and copies the newly created object to string1,
    //eequivalent to: "std::string string1 = std::basic_string<char>("test", std::allocator<char>());"
    std::string string1 = "test";
    //the following has the same effect as above. compiler equivalent to:
    // std::string string1 = std::basic_string<char>("test", std::allocator<char>());
    std::string string2 = { "test" };
    int d = 3; //3 is implicitly converted to an int


    /************************List initialization (since c++11)*************************
    https://en.cppreference.com/w/cpp/language/list_initialization
    applies whenever braced initializer lists (brace-init-list) are used ("{}")
    list initialization mirros the behaviour of value, scalar, direct and copy initialization
    with the added restriction that narrowing conversions are not allowed
    *Note: brace-init-list is not an expression, therefore has not type.
    * ******************************************************************************
    In addition {}, can be used only during initialization, emphasizing that we are giving
    a value to a not-yet-created object. (= can be used both for created and not-created objects).
    *******************************************************************************
    all the following are considered list initialization from c++11 onwards:

    1. value - list initialization - essentially exactly the same as the pre c++11
    value initialization, only rebranded now under the umbrella of list-initialization*/
    int h{};

    //2. scalar - list initialization - identical to pre c++11 scalar initialization
    int i{ 4 };

    //3. direct-list initialization
    Entity2 entity4{4}; // regular construction call

    //note: for pod's, scalar-list initialization and direct-list initialization
    //are identical:
    int j{ 4 }; 

    //4. copy-list initialization
    Entity3 entity6 = { 4 };

    //5. aggregate initialization
    //https://en.cppreference.com/w/cpp/language/aggregate_initialization
    //syntax: "T object = {arg1, arg2, ...}" or "T object{arg1, arg2, ...};" (both compiler-equivalent)
    //aggregate - array / class with no private/protected/static members and ctors
    // 1 initializes x, 2 initializes y (by order). compiler generates: "Entity entity5 = {1,2};
    Entity entity5{ 1,2 }; //direct-list aggregate initialization
    Entity entity9 = { 1,2 }; //copy-list aggergate initialization

    //if the array is of unknown size, and the supplied braced-init list contains
    //n elements - the size of the array is n
    int array3[] = { 1,2,3 }; //the size of the array is 3

    /*6. std::initializer_list:

    braced-init-list aka ("{}") can be used to construct std::initializer_list in the
    following scenarios:
    
    1. if a constructor accepts an std::initializer_list, then the braced-init list
    is used to construct the std::initializer_list, thus "list-initializing" the object

    *Important Note!!!! when using list initialization, a.k.a using braced-init-list
	("{}"), the compiler will favor constructors with std::initializer_list as parameter
    first, such as: Entity4(const std::initializer_list<int>& list) 
    vs ctors as:
    Entity4(int x, int y) : m_x{ x }, m_y{y} {}
    std::initializer_list<int> is a standard library container in C++ that represents an i
    nitializer list, which is a lightweight container-like object used to initialize
    a sequence of elements. This adds the abillity to iterate the inputs like a list.
    How to enforce the compiler to choose the second ctor with the two ints?
    apply direct-initialization via (), as seen with "entity10" var: */

    //direct-list initialization where the brace-init-list constructs an std::initializer_list
	Entity4 entity7{ 1,2 }; //compiles to Entity4 entity8 = { 1,2 };
    //copy-list initialization where the brace-init-list constructs an std::initializer_list
    // which only exists for the duration of that constructor call.
    Entity4 entity8 = { 1,2 };
    //direct initialization, the comiler selects the second ctor with the two ints
    Entity4 entity10(1, 2);
    
    std::vector<int> v{ {1,2,3} }; // invokes std::initializer_list ctor of Vector

    //2. when providing a braced-init-list as a function argument that accepts
    //an std::initializer_list - it is constructed via the braced-init-list
    func({ 1,2,3 });

    
    //3. when an auto declared variable is initialized with a braced-init-list,
    //k is deduced as a std::initializer list
    auto k = { 1,2,3 };
    //compiler equivalent to:
    //"std::initializer_list<int> k = std::initializer_list<int>{1, 2, 3};"


    //4. when a braced-init-list is used inside a range-based loop, an std::initializer_list
    //is constructed via the braced-init-list
    for (auto i : { 1,2,3 })
        std::cout << i << '\n';

    /*compiler equivalent to:

    std::initializer_list<int> && __range1 = std::initializer_list<int>{1, 2, 3};
    const int * __begin1 = __range1.begin();
    const int * __end1 = __range1.end();
    for(; __begin1 != __end1; ++__begin1) 
    {
      int i = *__begin1;
      std::cout.operator<<(i);
    }

    more detailed on range_based loop in 5_loops
    */
}