/*
structured bindings bind specific identifiers to elements of the subobjects/expression
tl;dr

1. hidden variable is created and initialized with the expressions we want to bind to
this hidden variable can be an lvalue ref, an alias of the object we bind to - more performant
because we don't create a new object! (if we use "auto&)
or it can be a  complete copy of the object we bind to (if we use "auto").
2.  lvalue references are created (regardless if we bind with auto or auto&, both initialized with 
corresponding sub object we bind to from the hidden var.

scenarios:

    1. if auto is cv-unqualified and without ref-declarator (no const and no &):
    for example : auto [a,b] = array;
    the hidden variable is a complete copy of the expression we want to bind to.

    2. if auto is assigned with & (auto&), for example: auto& [a,b] = array;
    the hidden variable is an lvalue reference to the expression we bind to (NO COPIES)

    3. if auto is assigned with const, for example: const auto [a,b] = array;
    the hidden variable becomes const.
    only varaibles that bind to lvalues become const lvalue ref
    variables that bind to lvalue refs do not become const
    reason unknown.
*/

#include <iostream>
#include <tuple>

struct Entity
{
    int x = 2; //default member init.
    int y = 3;
};

int main()
{

    //structured bindings cpp 17! they allow to construct and assign
    //values to variables in one line, as opposed to "tie", which requires
    //the creation of the variables before using "tie". works with tuples pairs
    //similiar to destructuring in javascript.

    //simple example:
    int arr[]{ 1,2 };
    auto [u, v] = arr;
    std::cout << u << " " << v << "\n"; // prints 1 and 2


    /*how structured bindings work behind the scenes?

    ***********************Case 1: binding an array

    "auto [u, v] = arr;" is a syntactic sugar for the following:

    // the compiler creates a hidden array based on the expression we initialize
    //u and v with. basically a complete copy of arr array.
    int __ar[2] = { arr[0], arr[1] };

    //u and v are created - these are the actual u and v the programmer will use.
    //the variables we bind objects to WILL ALWAYS BE lvalue references,
    regardless if auto is declared & or && (reference qualifier).
    the reference qualifiers only effect the hidden created variable (__ar[2])
    int& u = __ar[0]; // compiler assigns the temp values to
    int& v = __ar[1];

    the result: u and v are both lvalue references to the hidden-created __ar[2].
    u and v DO NOT refer to the oringial "arr" array.
    */

    //what about auto& ?
    int(&__a14)[2] = arr;
    int& o = __a14[0];
    int& p = __a14[1];


    /*
    the compiler produces the following:

    //__ar is a reference to array (arr) of 2 ints.
    //as opposed to the previous example that created a complete copy of the array
    //here we simply refer to it via an lvalue ref - better performance!
    //remember that an array of references is prohibited  (look for 17_reference)
    //__ar is now an alias for "arr" array - IT IS "arr", therefore we can access
    //members of arr through this reference.
    //note that the reference declarator & only affects the hidden "__ar" variable
    ///"__ar is a reference to an array of 2 ints"

    int (&__ar[2]) = arr;
    int& o = __ar[0];
    int& p = __ar[1];

    the result: o and p are lvalue references to the hidden-created __ar variable
    which itself is an lvalue reference to "arr", an alias for "arr" ->
    meaning that o and p are both referring to the original arr[0] and arr[1].
    */

    //up until this point all principles repeat
    /***************************************************************************/

    arr[0] = 100;
    //u is still 1 because it referes to the temp "_ar" array
    //o is 100 because it refers to the temp _ar array which refers to the original "arr"
    std::cout << u << " " << o << '\n';


    //what about const auto&
    const auto& [j, k] = arr;
    /*
    //cv (const/volatile) qualifier affect boths the hidden __ar variable
    //and the eventually-used variables j and k - everything becomes const

    the compiler produces the following:
    int const (&__ar[2]) = arr;
    const int& j = __a[0];
    const int& k = __a[1];

    in other words - to accomodate const correctness - everything becomes const
    */


    /*********************** Case 2: binding to a struct - exactly similar to array*/

    Entity z; // Entity z = Entity();  //equivalent to "Entity z";
    auto [x, y] = z; // structured bindings deduces from std::tuple and std::tie
    //behind the scenes 

    /*
    for the line "auto[x,y] = z" the compiler produces the following:
    Entity __temp = Entity{{2}, {3}};
    int & t = __18.x;
    int & w = __18.y;

    //same story that happened with the array example above:
    1. a hidden variable is intitalized with the value that holds the subobjects
    we our variables to bind to
    2. the compiler creates lvalue references to this temp value.

    same exact story happens with auto& and const auto& with structs
    */

    /*********************** Case 3: binding to a tuple/pair - principles above still hold*/
    float n{}; //0
    char  b{}; //0
    int   q{}; //0

    std::tuple<float&, char&&, int> tpl(n, std::move(b), q);

    const auto& [w, m, e] { tpl };
    /*
    the compiler produces the following statements for "const auto& [a, b, c] = tpl;"

    const std::tuple<float &, char &&, int> & __tpl20 = tpl; // reference to tuple tpl
    float & w = std::get<0UL>(__tpl20); 
    char & m = std::get<1UL>(__tpl20);
    const int & e = std::get<2UL>(__tpl20); //UL - unsigned long
    
    the hidden variable becomes const
    only varaibles that bind to lvalues become const lvalue ref
    variables that bind to lvalue refs do not become const for unknown reason.



    /*********************** Case 4: emplace and binding structures (binding to a pair)
     std::map<int, std::string> m { {1, "ken"}, {2, "lo"} };
     const auto& [iterator, key_exists] = m.emplace(3, "sd");

    compiles to:

    //initializing map
    std::map<int, std::string> m = std::map<int, std::basic_string<char>, 
    std::less<int>, std::allocator<std::pair<const int, std::basic_string<char> > > >
    {std::initializer_list<std::pair<const int, std::basic_string<char> > >
    {
        std::pair<const int, std::basic_string<char> >{1, "ken"}, 
        std::pair<const int, std::basic_string<char> >{2, "lo"}}, 
        std::less<int>(), std::allocator<std::pair<const int,  std::basic_string<char> > >()
    };


    //Note: becuase emplace returns a prvalue pair of an iterator and boolean, we must use const auto& for the structured binding.
    //this will cause the hidden variable to also be a const auto&. In comparison to just using "auto",
    //const auto& doesn't provide any benefits - in any case a prvalue is created and assigned to a variable.
    //const extends lvalues refs to accept prvalues. lvalues can bind to prvalues also with no problem - in both 
    //cases a new object is created. This is unlike using a reference with the array example above, where we actually
    //save a construction of a new array that is used for the hidden variable.
    const std::pair<std::_Rb_tree_iterator<std::pair<const int, std::basic_string<char> > >, bool> & __m8 = m.emplace<int, char const (&)[3]>(3, "sd");

    //the actual parameters "iterator" and key_exists are created
    const std::_Rb_tree_iterator<std::pair<const int, std::basic_string<char> > > & iterator = std::get<0UL>(__m8);
    const bool & key_exists = std::get<1UL>(__m8);
    */


}