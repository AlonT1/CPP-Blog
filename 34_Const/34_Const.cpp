/*
type qualifier const doesn't change generated code - 
its a mechanism that makes the code cleaner - promising that a var won't
be changed (immutable safety) - although it can be broekn
another type qualifiers are volatile, const

unlike string literals which are store in const segment in assembly (read-only memory),
other const variables such as const int x, aren't stored in read-only memeory,
the const in this context is more of compiler guarantee, and doesn't have any effect behind the scences
apparently doesn't even affect perforamnce, its only for maintainability
*/
#include <iostream>

class Entity
{
private: // m - member variable
    //int* m_w, m_z; m_w int pointer, m_z is an int (because * is a type modifier)!
    //int* m_w, *m_z; everyone is a pointer thanks to *!

    int m_x = 3, m_y = 2;
    int* m_ptr = nullptr;
    mutable int var = 2; // allows const methods to modify the variable
public:
    int x = 2;
    static const int y = 5; // explained in class_static
	
	// const int w; // MUST BE INITIALIZED AND ONLY INSIDE member initializer list!
	
    //IMPORTANT NOTE: invoking a non const function through a const instance IS NOT POSSIBLE.
     //e.g: "const Entity e;" --> "e.get_x();"  error!
     //if the function signature would have been "int get_x() const {...}" then the statements
     //above would have exectued fine.
    int get_x() // non const getter
    {
        return m_x;
    }

    // const at the end of method (const method) - read only method -
    //  this method won't modify class member variables
    // only const functions can be invoked by const instances!!!!!!
    // even the implicit this pointer is const: (Entity* const this)
    int get_x() const 
    {
        //
        //m_x = 2 //impossible
        var = 3; // possible if var is "mutable", overrides const. in scenarios where the method is const,
        //yet we need to perform slight modifications to data (for debugging)
        return m_x;
    }

 

    //method is read only, it returns a m_ptr that is constant pointer (can't be reassigned m_ptr = w //forbidden)
    //to an int constant (the integer data in the pointed location can't be changed *m_ptr = 2 -forbidden),
    //NOTE: in a const method, returning a reference of an object, the return value must also be const
    //because everything inside the function is const! (even though the object itself is declared as non const)
    const int* const get_m_ptr() const 
    {
        return m_ptr;
    }
    
    void set_x(int x)
    {
        m_x = x;
    }
};

//cheaper to pass reference than entire copy
//
void print_entity(const Entity& e) 
{
    /* same rules that apply to const poitners apply to reference - since e is an alias to a pointer (reference)!
    in other words, e is a pointer to an entity which is const - we can 
    repoint e but not mess with the constant entity (member vars)
    */
    std::cout << e.get_x() << std::endl; // will choose get_x() const because entity parameter is const
}


int main()
{

    int a = 5;
    int b = 3;
    const int MAX_AGE = 90; //not variable... it is always a constant value (variable implies change. constant implies lack of change) compiler treats as read only pointer


    /*
    The C++ language says that a local const reference, where the const refers to the type
    specifier (int in the following example below) prolongs the lifetime of temporary values
    until the end of the containing scope, but saving you the cost of a copy-construction
    (i.e. if you were to use an local variable instead). see 45_copy_ctor
    */
    const int& t = 5; // a reference to an rvalue - references can "usually" refer to an lvalue because they
    //have a memory location. but here they refer to an rvalue which doesn't have a memory location.
    //this is possible because the const promises that we won't change the literal and this makes sense
    //because 5 doesn't have a memory location which can be refereed to and be modified by us.
    //the const expands the reference and allows it to be assgined with rvalues CONSTANTS , prolonging their scope
    //from expression scope to the scope of the variable.
    //In addition copy constructors which are const int& can receive either lvalues or rvalues, and the ctor
    //can create an object from either. see 45_copy_ctor

    //read declarations backwards
    const int* ptr1 = new int; //ptr1 is a pointer to an int that is constant: the int data is constant, can't be changed via the ptr1
    //*ptr1 = 5; not possible, ptr1 is a pointer to a constant int
    ptr1 = &b; //although the int data is constant, the value of the pointer isn't
    //equals to  int const* ptr1 // ptr1 is a pointer to a constant int  

    // const can also refer to the declarator (identifier - ptr2 below) and not the the type (int)
    int* const ptr2 = new int; //ptr2 is constant pointer to an int: the value of the pointer is constant(can't be reassigned to point at other vars),
    //but the int value in that adress can be changed by the pointer
    //ptr2 = &b; not possible, pt2 is const pointer
    *ptr2 = 4; //although ptr2 is a constant pointer, the int itself is not 
    //(the int value that resides at the memory location the pointer points to - memory location is the actual value of the pointer).
    
    //const can be assigned both to the identifier (declarator) AND type (type specifier)
    const int* const ptr3 = new int; //ptr3 is a const pointer (value can't be changed, can't point to something else),
    //to an int constant (the data that resides in the pointed memory adress is constant)


    //ok
    Entity e;
    e.get_m_ptr();
    
    // const correctness - using the keyword const to prevent const objects from getting mutated, for example:
    // using const in "e1" below ensures that the const "e1" can only call const member functions,
    // otherwise without using const, e1 could call any function.
    // https://isocpp.org/wiki/faq/const-correctness#overview-const
    const Entity e1; 
    e1.get_x(); // refers to the const (read-only) getter above. if it wasn't const -> error
    // const instances reveal only const methods and won't allow changing of const variables:
    // 1. set_x() must be const, so it doesn't offer the possibility to change member values
    // 2. accessing any other member whether its const or not, is only read-only. const e1 can't modify members.
    /*********************************************************************
    Note: remembmer that const instances can only call const methods and not non - const methods!
    *****************************************************************************/
    // in general use - use const as early as possible.


    /**************************styles***************************/
    // style: "const int* ptr1" vs  "const int *ptr1": prefer the 2nd veersion because the
    // pointer refers to the identifier (* and & are type modifiers - see 30b_declarations_declarators)
    //for example:
    int m, *z; // z is a pointer to an int (the * refers to z), and m is simply an int

    // east-const vs west-const - see 34a_east-const  (use east_const)

}   

