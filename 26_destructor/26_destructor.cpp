#include <iostream>

class Entity
{
public:
    int x, y;
    int* nums;
     Entity()
    {
        std::cout << "constructed" << std::endl;
        x = 2;
        y = 4;
        nums = new int[6];
        for (int i = 0; i < 6; i++)
        {
            nums[i] = i;        
        }
    }
    ~Entity()
    {
        std::cout << "destroyed" << std::endl;
        delete[] nums; // must be manually deleted to prevent memory leaks
    }
};

class ScopedPointer
{
public:
    Entity* m_ptr;
    ScopedPointer(Entity* ptr) : m_ptr (ptr) {} //m_ptr assigned (copy initialization...) with the same address of supplied "ptr"
    ~ScopedPointer() { delete m_ptr; }
};


int main()
{
    /*************destruction of stack allocated objects*/
    Entity entity1();
    /*
    entity1 is stack allocated with implicit auto storage spceifier. when it reaches the end of the current
    scope, it will get destroyed which means two things:
    1. all pod class members (ints, floats...) will be deleted from memory. 
    2. the dtor function is invoked which gives us an opportunity to explicitly use the "delete" operator
    to delete heap allocated class members. not using "delete" to remove them in the dtor, will cause a memory leak
    (those heap objects will live on in memory).
    if a ptr points to a dynamically allocated object - using delete on that ptr will remove the object from memory
    if a ptr points to a stack allocated object - using delete on that ptr is undefined with a warning.
    */

    /*************destruction of heap allocated objects*/
    Entity* entity2 = new Entity;
    /*
    entity2 is a pointer. As opposed to the above inline "entity1" stack allocated member that gets destroyed
    when going out of scope (pod members deleted + dtor call), WHEN entity2 goes out of scope -
    ONLY THE POINTER GETS POPPED (removed) from the stack, the object remains alive in memory.
    To remove the object we must explicitly call the "delete" operator on entity2 which starts the destruction
    procedure explained above (stages 1,2).
    Note: the delete operator does not remove the "entity2" pointer (it only gets removed when going out of scope
    as a pod stack allocated object).
    */
    delete entity2;

    /*************automating delete - smart pointers (scoped pointers)*/

    ScopedPointer entity3{ new Entity }; // copy initialization via copy ctor of temp rhs through implict type conversion
    /*
    1. compiler detects that entity3 has a ctor that accepts a pointer to entity
    2. rhs object is constructed
    3. rhs is implicitly typecast to Scoped pointer, equivalent to "ScopedPointer(new Entity)"
    4. using the newly temporary constructed object (prvalue - stealable, no id), a copy ctor copy-initializes entity3 via the temp obj.
    Note: if copy elision (compiler optimization) is enabled the construction of the temp object directly occurs in the memory
    address of entity3, instead of constructing it at a separate adress and the copying its properties into entity3.
    Hence copy elision eliminates unnecessary copying of temp objects.

    ScopedPointer serves as a "stack pointer" - it houses a class "Entity" as a class member that has pointers
    to heap allocated objects. when entity3 goes out of scope it is deleted automatically
    (as all stack allocated objects), its dtor is invoked, and there we invoke a "delete"
    on the Entity class that is housed as a class member, essentially automating the "delete" keyword*/
}


/*
when a stack var goes out of scope the memory window shows that the value still exists 
but another adress is visibly changed to something else
When a dynamic allocated var is deleted, everything gets delted adress changes to dddddddddddddddd
*/



/*
The (->) arrow operator
To access members of a structure, use the dot operator. Stack var - Struct s
To access members of a structure through a pointer, use the arrow operator.
*/

/*.
RAII stand for Resource Acquisition Is Initialization. The better name is Scope-Bound Resource Management - 
resource acquired in the constructor (new in ctor) & released in the destructor of the object (delete in dtor).
"Resource" - can be file handles, network sockets, database handles, GDI objects, etc.
We encapsulate the resource in a local instance of a class that is automatically freed when the obj gets out of scope.
used in smart pointers  C++ Don't pay for something you don't use - there is no GC mechanism here like in C#.
We need to implement RAII as a memory management technique.
it is also used with files, for example:

void print_file(std::string fname)
{
   std::ifstream f(fname); // file opened here
   for(std::string line; std::getline(f, line); )
       std::cout << line << '\n';
}  // file closed here (out-of-scope)
//other wise, allocating and not releasing a resource is a resource leak, just like a memory leak
//(e.g: "new"ing an array inside a function without performing a "delete" on the pointer that holds the array(resource)
//before leaving the scope of the function).

GC (Garbage Collection) like the one in Java, that adds its own overhead, removes some of the determinism 
(the abillity to explicitly call destructor) from the resource release process and handles circular
references (not handeled in cpp - variables refering to each other are being released).
Garbage Collection solves memory management  making it so that the developer doesn't 
need to pay as much attention to managing those resources.
RAII solves it by making it easier for developers to pay attention to their resource management. 
Anyone who says they do the same thing has something to sell you.
//Herb Sutter’s cppcon 2016 presentation[1] on deferred_ptr[2] : smart pointers are garbage collection.
RAII in CPP means that any allocated resource is bound to some scope. 
When that scope is left, the resource is destroyed/released automatically.
Any object allocated by new() MUST be de-allocated by a matching delete().
Any memory allocated by malloc() MUST be de-allocated by a matching free().



RAII                        GC
Programming idiom vs    Mechanism
                        handles circular references (reference counting?)
no over head
user deterministic           automatic memory reclamation
mainly in smart pointers

*/



/*
memory layout (memory model) also called the address space because the memory layout
can occupy all the memory space 0x00000000 to 0xFFFFFFFF
:

os is responsible for stack and heap
compiler & linker organize in the following way:
1. text - code as binary (object files)
contains a read only section
for char s[]  &  char* s - read only data segment in memory (.rodata) both are stored in const segment of assembly language
because both versions are immutable (stored in read-only memory), any change to the them actually starts steps 1,2,3
the notes of immutable
https://stackoverflow.com/questions/2589949/string-literals-where-do-they-go

2. Data segment  - static/global variables, explicitly initialized
3. BSS (Block Started by Symbol) segment - uninitialized global & static vars. filled implicitly with zeros
4. Stack segment - (size determined by os) all local vars go here including functions. object data is stored contiguously.
grows downwards (from high to low memory addresses). (size determined by os) all local vars go here including functions.
object data is stored contiguously. when we allocate data the pointer moves to a new position, all data is stored close to one another.
stack allocation is fast - one cpu instruction - move stack pointer up x bytes (4 for int)
and return the address of the new position 4 byte int allocation -> stack pointer moves up 4 bytes
5. heap segment - part of ram , grows upwards (from low memory address to high)
6. OS Kernel space - user code cannot read or write to these addresses


*/
