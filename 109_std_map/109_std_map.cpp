#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>
#include <string_view>
#include <algorithm>
/*
https://jguegant.github.io/blogs/tech/performing-try-emplace.html
https://dzone.com/articles/using-stdmap-wisely-with-modern-c
https://www.fluentcpp.com/2020/05/01/how-to-change-a-key-in-a-map-or-set-in-cpp/
https://stackoverflow.com/questions/5743545/what-is-the-fastest-way-to-change-a-key-of-an-element-inside-stdmap

tl;dr
std::map and std::unordered_map are an associative (every value is associated with a key) container
that containes std::pairs of key-value.
std::maps are usually implemented as red-black trees - no hash table and no hashing are used!
they are ordered accroding to a comparator object:
std::unordered_map is implemented via hash table.


1. use operator[] for simple insertions and retrieval of objects that we are sure 
that they exist in the map. That's because operator[key] returns a  default constructed
elemnt of the value type if the key we are looking for (and thus the std::pair) does not exist.
Therefore, if the key of 1 does not exist, when the query "if(map[1])" returns a 0: we need to ask,
was 1 intentionally paired with a 0 or 0 was returned as a result of
operator[] constructing a default 0 int object. This especially bad if default construction
of the type is very slow, assuming a very slow, heavy class.

2. find() (solves 0 problem above) when we are not sure if the object exists or not.
find() returns an iterator to found element(key-value pair)/ or end of map if element not found,
decoupled from 0 values like operator[] above.

3. try_emplace when we need to know if the insertion was successful
(a key-value pair with our input key wasn't there)
or failed (a key-value pair with our input key was there). try_emplace returns an std::pair that consists
of an iterator to the inserted/existing element and a bool that tells if 
the insertion happened or not (true/false depending if the key existed).

4. operator[] utilizes try_emplace in the overload behind the scenes
[] syntax is comfortable) for insertion / retrieval 

5. map is an ordered associated container

*/

class Entity
{
public:
    int m_x, m_y, m_z;
    Entity(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {}
};

void do_something_else(int w) {}

int main()
{
    //map is an associative (every value is associated with a key) container
    //that containes many std::pair (extension of std::tuple) of key-value.

    std::map<std::string, std::string> m { {"first", "moshe"}, { "second", "yossi" }};
    

    /********************** map::find()*/
    // if the key exists, find() returns an iterator to the  found element.
    // map elements are std::pairs of key-value is where the "key" is the
    //argument we supplied ("first") and the value is the value associated with the key.
    std::map<std::string, std::string>::iterator pair = m.find("first");  //auto to replace the type
    if (pair == m.end()) // if the key isn't found an iterator to the end of the map is returned
        return 0;
    std::cout << pair->first << '\n'; // prints "first" (the key of the pair)
    std::cout << pair->second << '\n'; // prints "second" (the value of associated with the key)


    /********************** map::operator[]*/
    //if operator[] doesn't find the value associated with the key, 0 is returned
    std::cout << m["first"] << '\n';

    // can also be used for insertion which will occur only
    //if the key doesn't already exist:
    m["third"] = "ben"; 
    m["third"] = "ben";// the key "third" already exists, this statement has no effect on the map
    //although we are still required to hash "third" and check if the key exists in the map
    std::cout << m["third"] << '\n';
    //Important note: map::operator[] utilizes try_emplace behind the scenes (try_emplace)
    //explained below

    //problem 1 with operator[] - checking if the key already exists before insertion:

    //if the key doesn't exist, operator[] returns default constructed element in the
    //type of the value. for std::string that would be {}, for bool - false, and for ints 0's.
    //but what if the value of a key in a map that stores values as ints is also 0?
    //in this case the map will retrieve 0 although the key exists. 
    //so is 0 the actual value associated with an existing key or does it mean the key isn't found?
    //same goes with keys whose values are null strings or false bool values, etc.

    //so basically we may get a false-positive about the existense/inexistence of a key + 
    //additional default construction of an element (it the key doesn't exist).
    std::map<int, int> map{ {0,0}, {1, 1} };
    int value = map[0];
    if (value) // false (returns 0) but the key of 0 exists!
        std::cout << map[0] << '\n';
    //note: better use cpp17 if with initializing statement conditional if(int value = map[0]; value !=0)
    //see 12_conditions_branch

    //indeed key 2 isn't in the map, but operator[] returns a default construct
    //elemnt of int (0), but what if the value of the map was a "heavy" object?
    //this is a rdeundant construction!
    if (map[2])
        std::cout << "key not found" << '\n';

    //solutions: 
    //1. find() for retrieval first, insert if doesn't exist after:
    //use find() which returns an iterator pointing to the end of the map
    //if the key isn't found. In this way the compiler avoids a redundant default constructed object
    //if the key isn't found, mitigating the question if 0 the actual value associated with the key
    //or does it mean the key isn't found?). Iterators are decoupled from these values.

    //2.DON'T CHECK IF THE KEY EXISTS AT ALL BEFORE INSERTION - use try_emplace right
    //away which returns a pair that consists an iterator to the inserted/existing element and
    //a bool that tells if the insertion happened or not (true/false depending if the key existed).
    //this functionality doesn't exist with operator[]

    map[2] = 3;
    //the downside is that we have no indication if the insertion was succesful or not, meaning
    //if the key existed or not, as opposed to try_emplace.
    

    /********************** map::insert()*/
    //inserts an std::pair with key-value
    //returns a prvalue temporary pair - the first member is an iterator pointing to the newly inserted
    //pair, or to a pair with an equivalent key (means that the pair was already inserted, the 
    //key was already a part of a key-value pair stored the map).
    //the second member is a bool that returns true if the pair is inserted (when it doesn't exist)
    //false (when the pair with the key we want to insert already exists in the map).

    //Notes about the returned pair - this also relevant for emplace and try_emplace - both return this type of pair:

        //1. what makes this very lightweight is the fact that although a temporary pair is created,
        //the pair only contains an iterator (pointer) to the inserted/exisiting pair in our map
        //and a boolean value. the "Big" objects that we use as key/values are not created in this process,
        //only an iterator and a boolean

        //2. the iterator returned by emplace, find, etc. points to a pair that has an umodifiable key
        //(value is non-const) - essentially a read-only iterator in terms of the key. this makes sense because
        //changing the key  will cause it to no longer match the value).
        //The solution to modifying a key of a pair without breaking the map:
        //use auto node = myMap.extract("two"); - the pair whose key is "two" will no longer be in the map,
        //it is moved via move semantics into the node (of type std::node_nandle)
        //we can modify the key via: node.key() = "dos"; and inset the node back
        //via move semantics to save copies myMap.insert(std::move(node));
        //emplace won't work because we are not using the constructors of the types we've chosen.

    //changing the key of map node(key-value pair)
    auto myMap = std::map<std::string, int>{ {"one", 1}, {"two", 2}, {"three", 3} };
    auto node = myMap.extract("two");

    node.key() = "dos";
    myMap.emplace(std::move(node));
        //https://www.fluentcpp.com/2020/05/01/how-to-change-a-key-in-a-map-or-set-in-cpp/
        
    //note that there 2 constructions done here: we construct an std::pair in the insert()
    //and then this pair is copy constructed (we construct an std::pair inside the map with a temporary
    //std::pair we just built).
    //emplace solves this
    std::pair<std::map<std::string, std::string>::iterator, bool> result =
        m.insert(std::pair<std::string, std::string>("fourth", "alex"));
    std::cout << result.first->first << '\n'; //access the iterator to the inserted or already existing pair
    //(accessing the first element of this pair - meaning getting the key)
    std::cout << result.second << '\n'; // bool that says whether the element was successfully inserted

    //short version to the above
    //auto type deduction + structured bindings [it, result] that accepts the iterator
    //and bool returned from insert() + template argument deduction (std::pair doesn't
    //require <std::string, std::string>, it deduces these types from the arguemnts ("fifth, "avi")
    auto [it1, result1] = m.insert(std::pair("fifth", "avi"));
    
    //Note that although "fifth" key already exists, the pair will still be redundantly built, but not inserted,
    // i.e result2 is false
    auto [it2, result2] = m.insert(std::pair("fifth", "avi"));


    /**********************map::emplace() - more performant than insert, and without std::pair*/
    //emplace directly uses the ctor (inplace) of std::pair which map wraps around and the ctor of pair
    //will use the the ctors of the key and value, in the same
    //way that an emplace on vector<Test> will utilize one of Test's ctor, and is 
    //even more performant (no need to construct a pair outside and then provide it into the
    //map which causes another copy(construction) - happens in insert(); with emplace() we build the pair
    //directly into the map using the ctor of pair.
    //emplace returns a temporary prvalue pair.
    auto [it3, result3] = m.emplace("sixth", "natan"); //returns the same value as insert()

    //in addition, because the "sixth" key already exists, nothing will be built,
    //as opposed to insert() which builds a pair regardless if the key already exists
    //in the map or not.
    auto [it4, result4] = m.emplace("sixth", "natan");
    //return value identical to insert()

    //using const auto& is not more efficient in this case! (read 75_structured bindings. tl;dr
    //in both lvalue and const lvalue ref cases, the hidden variable created for the structured bindings
    //will bind to the iterator-boolean pair, in other words, a new object will be built in both cases!)
    const auto& [it5, result5] = m.emplace("seventh", "kobi"); //read 75_structured_bindings for emplace() details


    /**********************map::emplace() vs map::try_emplace*/
    std::map<std::string, std::unique_ptr<int>> m2;
    std::unique_ptr<int> int_ptr1 = std::make_unique<int>(10);
    m2.emplace("shalom", std::move(int_ptr1)); //about the 2nd parameter:
    //only std::move(int_ptr1) or std::make_uniqe<int>(val) are accepted, because
    //emplace directly constructs a unique_ptr, thus the second argument needs
    //to be an expression that is supported by a ctor that can construct unique_ptr
    //(move or make_unique), we cannot pass int_ptr1, because there isn't a ctor
    //that accepts this expression inside std::unique_ptr, a unique_ptr cannot even be assigned (copy)
    // with another unique_ptr (only moved) on the contrary std::string can accept a const char*.

    /*
    the problem with emplace() is that whether the insert is successful or not,
    the move operation in the instance being constructed (std::unique_ptr in this case) will ALWAYS execute.
    meaning that int_ptr2 resources will be stolen  whether the insert is successful (when a pair with the
    key doesn't exist) or not (when a pair with the key does exist),
    and int_ptr1 will left in a null state (it will point to null) - this is an issue/bug.
    *****************try_emplace fixes this, if insertion fails, no move operations will occur.
    Other then that, try_emplace and emplace are identical(?) 

    example:

    //replace m2.emplace() with m2.try_emplace() in the following example to fix the issue detailed above!!!!
    //although "shalom" key already exists in the map (and even if it wasn't!), int_ptr2 would be moved-from
    //(its resources would have been stolen) when using emplace() - try_emplace fixes this if insertion fails,
    //no move operations will occur.


    std::unique_ptr<int> int_ptr2 = std::make_unique<int>(10);
    auto [it3, result3] = m2.emplace("shalom", std::move(int_ptr2));
    std::cout << *int_ptr2;

    if (!result3) //  the insertion failed because we assume that 
    //a pair with the key "shalom" already exists, let's do something else with int_ptr2.
    { 
        do_something_else(*int_ptr2); // Or can we? int_ptr2 is now null, resources stolen
        //although the insertion failed. try_emplace fixes this - failed insertion
        //will not cause the redundant move of the arguments
    }
    */



    /**********************check before insertion if the key exists*/
    //if we want to check if the key exists
    if (map[2]) // 1st hashing of 2 to check if map[2] exists - it doesn't
        map[2] = 2; //2nd hasing of 2 to perform an insertion of 2 into a memory location of
    
    //solution: use try_emplace which performs a single hash opertaion
    auto [it5, result5] = m.emplace("seventh", "amos");
    std::string& str = it5->second; //retrieving "amos"

    //to check if the key exists, we can query result5 that states whether 
    //a pair with "seventh" key was already inserted or not
    //BUT WHY SHOULD WE check the key exists? sometimes it isn't even necessary!
    //if the "seventh" key was already inserted, then we retrieve the already constructed "amos" value 
    //back into "str" via the iterator
    //if "seventh" wasn't inserted, then emplace constructs "amos" as a part of the key-value pair
    //and then we retrieve "amos" via the iterator into "str"
    //****in both cases, we can safely retrieve the value, without querying if its key was in the map or not.*******


    /**********************iterating over a map*/
    std::map<std::string_view, std::string_view> m1{ {"joe", "warren"}, {"jenny", "coleman"} };
    // std::map<std::string_view, std::string_view>::iterator type can be replaced by "auto"
    for (std::map<std::string_view, std::string_view>::iterator itr{ m1.begin() }; itr != m1.end(); ++itr)
        std::cout << "key: " << itr->first << " " << "value: " << itr->second << " | ";
    std::cout << '\n';
     
    // for each - auto represents "std::pair<std::string_view, std::string_view>"
    for (auto& pair : m1) //remember "by-reference"! better performance, no unecessary copies
        std::cout << "key: " << pair.first << " " << "value: " << pair.second << " | ";
    std::cout << '\n';


    //cpp17 structured bindings - the best!!
    // by reference to avoid redundant copies
    for ( auto& [key, value] : m1) 
        std::cout << "key: " << key << " " << "value: " << value << " | ";


    /***********************pointers as key in a std::map - Possible!

    std::maps are usually implemented as red-black trees - no hash table and no hashing are used!
    they are ordered accroding to a comparator object:

    the map is a class with the following template parameters:
    template <class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty>>
    _kTy - the key of the map
    _Ty - the value
    _Pr - which is a Compare class (comparator that implements operator<).
    Keys are sorted by using the comparison objct Compare(_Pr).
    it is default-initialized to std::less, which is the following struct:

    template <class _Ty = void>
    struct less 
    {
        constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const {
            return _Left < _Right;
        }
    };

    std::less is Function object for performing comparisons. Unless specialized, invokes operator< on type T.
    This means that all types (including pod's - such as pointers) are ordered by default according to their
    operator<, invoked (delegated) through std::less.
    std::less comes in handy when you need to pass operator< to a function, because C++ doesn’t allow to pass operators.
    f(42, 43, <); // not valid C++
    f(42, 43, std::less<int>{}); // valid
    In addition //https://stackoverflow.com/questions/42762633/why-is-stdless-better-than

    therefore, third parameter in std::map template is optional, it defaults to std::less
    and all pod's and all relevant cpp classes support operator<.

    C++ standard provided specialisation of std::less for pointers, so yes you can safely use them as map keys etc.


    Note: for user-defined types (custom objects) we have the following options to ensure order - 
    std::map is implemented as red-black tree - this is why the three options above define ORDER and not a specific hash
    function.
    1. implement operator< in our class (similar to comparable interface in java)
    2. provide a struct/lambda functor that overrides operator() and performs a comparison via opreator <.
    this is fed to the 3rd template parameter type of the map.
    (similar to comparator in java). This will replace the default std::less
    3. specialize std::less in our solution for our specific class - this solution is picked up automatically
    by std::map as long as we use namespace std, so we don't need to fed this struct manually to std::map. 

    namespace std  //because STL is inside std, so this specialization will be picked up by the compiler
    {
        template<> 
        struct less<A> //specialization of less for Class A
        {
            bool operator()(const A& k1, const A& k2) const
            {
                return k1.v < k2.v;
            }
        };
    }
    This is useful when our key doesn't have meaningful comparison semantics, making
    it bad to overload operator< just to cover the specific case for ordering. (although the 2nd solution
    is more "clear" in this case).


    //https://stackoverflow.com/questions/2282349/specialization-of-templateclass-tp-struct-stdless-in-different-namespace


    Note for pointers as keys:
    The default implementation will compare the addresses stored by the pointers, so different 
    objects will be considered as different keys. However, the logical state of the object will not be considered.
    For example, if you use std::string * as the key, two different std::string objects with the same text
    of "Hello" would be considered a different key! (When stored in the map by their addresses)


    */
    std::string str1{ "str1" };
    std::string str2{ "str2" };
    std::map<std::string*, int> mappa { {&str1, 2}, { &str2, 3 }};


    /*********************** pointers as key in a std::unordered_map - also Possible!

    unordered_map is implemented via hash table. The class is:
    template <class _Kty (The key), class _Ty (The Value), class _Hasher = hash<_Kty> (hash function)
    , class _Keyeq = equal_to<_Kty> (comparator operator< function), class _Alloc = allocator<pair<const _Kty, _Ty>>>

    To be able to use std::unordered_map (or one of the other unordered associative containers) with a user-defined key-type,
    you need to define two things:

    1. A hash function; this must be a class (stuct /lambda)that overrides operator()  and calculates the hash 
    value given an object of the key-type, this class is provided as the third template parameter when creating the map.
    another way is to specialize the std::hash template for your key-type, in the same way we specialized
    std::less for std::map in the example above (both std::less and std::map specialization should reside in namespace std
    to get picked up the compiler).

    2. A comparison function for equality. Note that this function defaults to std::equal, 
    while std::map compare function defaults std::less; this is required because the hash cannot rely on the 
    fact that the hash function will always provide a unique hash value for every distinct key 
    (i.e., it needs to be able to deal with collisions), so it needs a way to compare two given keys for an exact match,
    this is an extra check, even if hash values are the same, the comparison function ensures that the keys are really the same.
    both the hash function and the comparison function aim to ensure the feature of a mathematical set - no duplicates allowed!
    we can implement this either as a class that overrides operator() (a struct that overrides operator() 
    and performs a comparison), or as a specialization of std::equal,
    or – easiest of all – by overloading operator==() for your key type.
    
    example here (1 is represented as KeyHasher and 2 as operator== in Key)
    https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    */

    std::unordered_map<std::string*, int> mappa2{ {&str1, 2}, { &str2, 3 } };



    /*********************** emplace with multiple arguemnts */
    std::map<int, Entity> entity_map;
    entity_map.emplace(std::piecewise_construct, 4 , std::make_pair(4, 5));
    //equivalent to: (note that make_tuple has no problem receiving 1 argument)
    entity_map.emplace(std::piecewise_construct, std::make_tuple(4), std::make_tuple(4, 5)); 
    
    /*how this works:
    Emplace constructs a tuple inplace, inside the stack frame of the map, using the ctors
    of key and value.
    Pair has a special ctor:
    pair(piecewise_construct_t, tuple<_Types1...> _Val1, tuple<_Types2...> _Val2);

    piecewise_construct_t is a tag dispatcher (read 115_tag_dispatcher), essentially an empty struct (see below)
    that serves as flag that enables us to invoke the ctor above (the invokation happens
    because this ctor accepts a piecewise_construct_t and 2 template type parameter packs.

    //essentialy an empty class. we just use it as a flag for invoking the ctor
    STRUCT piecewise_construct_t
    struct piecewise_construct_t // tag type for pair tuple arguments
    { 
        explicit piecewise_construct_t() = default;
    };

    piecewise comes from the math piecewise function - a function defined by multiple sub-functions,
    probably because this ctor can build a tuple via different multi-parameter constructors from different objects 
          
          ___
          |int{4}
    f(x)= |
          |std::make_tuple(4, 5)
          ---
        */
}

// implementing user defined object for map, unordered_map, unoredered_set:

//https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key
//contains lambda + class + specialization of hash options for customizing the hash function for our class:
//https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key 
//https://stackoverflow.com/questions/38554083/how-can-i-use-a-c-unordered-set-for-a-custom-class

//contains example at bottom of the page with lambda + class + specialization options for a custom hash function
//for a user defined object, and an equality function that can be implemented as operator==, lambda or struct (functor)
//https://en.cppreference.com/w/cpp/container/unordered_map/unordered_map


/*
general notes:

1. emplace is more efficient for initialization - even more then initialization list

    //causes 17 allocations!
    std::map<std::string, int> m{
    { "Hello Super Long String", 1 },
    { "Another Longish String", 2 },
    { "This cannot fall into SSO buffer", 3 }
    };

    //causes 11 allocations! - more efficient
    std::map<std::string, int> m;
    m.emplace("Hello Super Long String", 1);
    m.emplace("Hello Super Long String", 2);
    m.emplace("Hello Super Long String", 3);

*/