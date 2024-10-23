#include <iostream>

/*
"new" asks the OS to allocate a contiguous block of memory on the heap in the
size of the type (e.g: int - 4 bytes) and a pointer to the memory
address of that location. The OS doesn't linearly scans for a contiguous block
of memory, but rather it uses a free-list which maintains free bytes. it still
slower than stack allocation. In addition, to reach the data of the class, 
we must perform an extra jump to a memory location of the data.

anything created with "new" needs to be deleted with "delete"
anything created with "new []" needs to be deleted with "delete[]"
the "deletes" clear up the memory locations but do not remove the pointer to that location!
the pointers are pod and allocated on the stack, they are bound to the scope of a code segment

anything allocated with new is heap-allocated and uses a "dynamic storage specifier" - 
the object lives until delete.


The problems associated with using new and delete:

1. Dangling ptr - when a pointer points to deleted mem location (an address that was returned 
to the os and no longer available to our program). who knows what the os
now does with this address..., accessing causes undefined behaviour/crash),
Therefore we should remember to set the the ptr to "nullptr".

2. Mem leak - not deleting unused memory pointed to by ptrs. 
a leak will occur when we forget to "delete" an heap allocated memory,
while also losing touch with that memory by losing the scope of the pointer, or when
the pointer gets destroyed.
For example, when "newing" up an object in a function we must remember to perform
a "delete" on the pointer to return the memory it points to back to the os. If
we don't perform that "delete" and we'll leave the scope of the function,
the pointer (which is always a stack allocated object) will get automatically removed
from the stack (destroyed) by the compiler, leaving the memory it pointed at
as "floating in space", "leaking", basically we lost the only contact we had with 
him via the pointer which got destroyed when we left the stack. Now we can't delete
that memory (return it back to the os). It will only be freed when the program stops.

*/

class Entity
{
public:
    int x = 4;
};

int main()
{
    int a = 2; //stack allocated
    int* b = new int[50]; // a contiguous memory of 200 bytes (4*50) is allocated and its memory adress is returned to ptr b


    Entity* c = new Entity; //allocates memory in the size of Entity + constructs Entity with garbage values
    //and retrieves a memory address of the allocated block. new Entity() creates Entity with zero values for pod's
    

    Entity w;//  w is a value variable which directly holds the data of the class! no pointer overhead exists
    //the data is stored at &w. in the example above we need to get the value of c (memory address of Entity)
    //and jump to that location - an extra step. // w is on the stack, auto storage, and scoped


    //new uses c's malloc behind the scenes
    Entity* d = (Entity*)malloc(sizeof(Entity)); // retrives a memory address of size entity,
    //but! doesn't construct the object! the location of is empty of content - in cpp use only "new"
    /* To be more specific - given:
    struct Node
    {
        int data;
        struct Node* next;
    };

    in both c and cpp using malloc and new the memory block will look like:
    +------------+------------------+
    | 4 byte int |  8 byte pointer  |
    +------------+------------------+
    Malloc does not initialize this memory block, but new does (via ctor).

    new also offers type safety by returning a pointer of the correct type, avoiding the need
    for explicit type casting like happens with malloc.

    */
    
    //use placement new:  new(b) to choose the location of where the memory is stored
    Entity* f = new(b) Entity; // Entity is constructed at "b"s location

    //delets uses c's() free behind the scenes - does not remove the pointer from memory!
    // the pointer is allocated on the stack as a plain old data (POD), the object at the memory address is indeed removed.
    //delete prevents memory leaks - the instantiated object is not needed but still resides in memory.
    delete[] b; //use "delete[]" to remove allocation done with - "new []"
    delete c;//use "delete" to remove allocation done with - "new"


    /* Memory fragmentation
    if heap was 10kb space.
    
    int* x = (int*)malloc(3000) //3000 bytes == 3 kb allocated on heap
    
    heap: size: 10kb  occupied: 3kb free: 7kb

    int* y = (int*)malloc(4000)

    heap: size: 10kb   occupied: 3kb 4kb  free:3kb
    
    free(x);

    heap: size: 10kb   occupied: deleted(3kb) 4kb     free:3kb

    x = (int*)malloc(4000) MEMORY FRAGMENTATION
    even though 6k memory is available - there isn't a 4k contiguous block available
    defragmenting the memory by merging the two 3kb blocks into a single 6k chunk is not
    possible because this would require to move the 4kb chunk - NOT good since y points
    to this adress, changing it will break the pointer.

    In Java, C#  the garbage collection handles defragmentation 
    without adverse effects, this is becuase those languages don't support
    direct pointers. This process occurrs when memory allocation fails/periodic
    gargbage collection cycle - this compromises real time performance and determinism.
    */

}
