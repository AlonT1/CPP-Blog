#include <iostream>
#include <map>
#include <set>
#include <tuple>

//https://www.bfilipek.com/2019/05/heterogeneous-lookup-cpp14.html

void* operator new (size_t size)
{
    static int num_of_allocations{ 0 };
    std::cout << "allocating: " << size << " " << ++num_of_allocations << '\n';
    return malloc(size);
}

class Employee
{
public:
    int m_id;
    std::string m_name;
    Employee(int id, std::string const& name) : m_id(id), m_name(name) {}
};

struct CompareId
{
    using is_transparent = void; // any type works

    bool operator()(const Employee & employee1, const Employee & employee2) const
    {
        return (employee1.m_id < employee2.m_id);
    }

    bool operator()(int employee1_id, Employee const& employee2) const
    {
        return employee1_id < employee2.m_id;
    }

    bool operator()(Employee const& employee1, int employee2_id) const
    {
        return employee1.m_id < employee2_id;
    }
};


int main()
{

    /*
    heterogenous lookup means that we can perform a lookup in a container, based
    on one of the fields of the key (e.g: int id) rather then creating a temporary object
    of the key just for the sake of look up (e.g employee object with int id, string name, int age)

    tl;dr
    for built-in types we can plug in a specialized std::less<void> as compare function that can perform
    comparison between any types, thus enabling us to compare using comparable and not identical types,
    for example std::string and const char*, instead of the default std::less<Key> that enables comparison
    between 2 keys only of the same type to find an object (2 std::strings) - thus enforcing us
    to redundantly create an std::string just for a search instead of using a const char*.

    if we want to lookup a user-defined type key using one of its class fields, std::less<void> won't suffice:
    we must implement a comparison functor that contains the flag
    is_transparent and overloads operator() 3 times incorporating the lookup field as shown below.
    flag is_trasparent enables the stl library to recognize that the functor (the comparison struct)
    is allowed to compare between different types (the key type and the field type).
    */


    /*********homogenous look up when the key is a user defined type (class employee)*/
#if 1
    std::set<Employee, CompareId> employees{ {1, "avi"}, {2, "bobby"} };
    

    //assuming the set above is ordered by the id of the employee (and therefore
    //is unique by id - no two object can have the same id),
    //say we want to find object with id of 1 - well... we can't
    //we must create a temporary object with the exact same parameters and plug
    //it in the find method:
    Employee e{ 1, "avi" };
    employees.find(e);

    //this problematic becuase we need to create a redundant temporary object just to perform
    //a search. The solution:

    //1. choose a class member that will be used for searching - the id in this case

    //2. create a functor struct for comparison purposes  that has 3 operator() overloads: 
    //(a must for custom objects such as employee, which are compared using operator<, 
    //built-in types such as ints or doubles are compared using the default std::less comparator)
        //a. 1st operator() performs a comparison between two Employee objects
        //b. 2nd operator() performs a comparison between the chosen field (id) and an employee object
        //c. 3rd operator() performs a comparison between the employee object and an chosen field (id) 

    //the set will sort three objects (x,y,z) by checking if (x<y), (y<x), (x<z) and so forth
    //to determine order and if there are any duplicates

    //note that b. and c. are used to guarantee strict total ordering via operator<
    /*
    how does operator< knows that 2 values aren't equal (strict total order)
    a comparison between x<y and y<x is not the same and must occur to figure out
    if they are equal/unequal/smaller/bigger 
     https://proofwiki.org/wiki/Definition:Strict_Total_Ordering
    ≺ is a strict total ordering on S if and only if (S,≺) has no non-comparable pairs:
    ∀x,y∈S:  x≠y⟹ x≺y ∨ y≺x   this means:
     (if 5<6 or 6<5  ==>  1 or 0 ==> 1 ==> x≠y)
     (if 5<5 or 5<5  ==>  0 or 0 ==> 0 ==> x=y)
    ∃x,y∈S:  x≠y ∧ (x≥y ∧ y≥x)  (material implication - p⟹q  ⟺ p∨¬q )
    ∀x,y∈S:  (x≥y ∧ y≥x) ⟹  x=y (contraposition -  p⟹q  ⟺ ¬q∨¬p )
    
    
    */

    //3. define a typedef called is_transparent. this flag is (SOMEHOW) picked up by stl library, and denotes
    //that this functor is a transparent functor - it can take arbitrary types such as int (id)
    //and doesn't only compares two employee objects - this enables the compiler (SOMEHOW) to pick up
    //the 3 different operator() overloads with different type.



    employees.find(1); // works

#endif


    /*********homogenous look up when the key is a built-in type (std::string)*/
#if 0

    //when no compare function is assigned when initializing the container, the container
    //defaults to the default compare functor it is assigned with as a default parameter.
    //with map the compare is std::less<Key> -> in our case std::less<std::string>.
    //this means that the map is ordered via the key, using std::less<std::stromg> which orders via operator<
    //of std::string (overloads oeprator<).

    // the 3rd template parameter type defaults to std::less<std::string> if not specified otherwise
    std::map<std::string, int> map{ {"hello",1} }; 

    //the problem is that std::less<std::string> can only compare two std::string objects.
    //to accommodate this fact, the compiler actually constructs a redundant std::string
    //via "hello" just to perfrom the lookup.
    //This happens despite the fact that an std::string is perfectly comparable with const char*
    map.find("hello");

    //to solve this, we must initialize the map with std::less<void> which defines is_transparent flag
    // std::less<void> is a specialization of std::less
    //std::less<void>  accepts arguments of arbitrary types and uses perfect forwarding,
    //which avoids unnecessary copying and conversion:


    std::map<std::string, int, std::less<void>> m{
    { "Hello Super Long String", 1 },
    { "Another Longish String", 2 },
    { "This cannot fall into SSO buffer", 3 }
    };

    //no redundant copies, std::less<void> can compare std::string with const char*
    m.find("Hello Super Long String");


#endif


}