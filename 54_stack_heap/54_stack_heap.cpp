/*
when the program starts it gets divided into a few area of memory:
os is responsible for stack and heap
compiler & linker organize in the following way:
1. text - code as binary (object files)
2. Data segment  - static/global variables, explicitly initialized
3. BSS (Block Started by Symbol) segment - uninitialized global & static vars. filled implicitly with zeros
4. Stack segment - (size determined by os) all local vars go here including functions. object data is stored contiguously.
grows downwards (from high to low memory addresses). (size determined by os) all local vars go here including functions.
object data is stored contiguously. when we allocate data the pointer moves to a new position, all data is stored close to one another.
stack allocation is fast - one cpu instruction - move stack pointer up x bytes (4 for int)
and return the address of the new position 4 byte int allocation -> stack pointer moves up 4 bytes
5. heap segment - part of ram , grows upwards (from low memory address to high)
6. OS Kernel space - user code cannot read or write to these addresses


the os loads entire program into memory, allocates physical ram for it.
the stack and the heap are 2 areas in the ram. stack - ~2mb, heap can grow and change.
the actual location of these areas is in the ram. stack my be "hot" in the cache, because we often access the
stack, but it is stroed in the ram. memory is used to store data.
stack and heap work differently but are the same. we ask cpp data from these locations and the os returns the block.
the difference is in memory allocation. an int is 4 bytes - finding contiguous memory in the stack and heap is different.


heap fragmanetation:

Imagine that you have a "large" (32 bytes) expanse of free memory:
----------------------------------
|                                |
----------------------------------
Now, allocate some of it (5 different types, 18 byes in total)
----------------------------------
|aaaabbccccccddeeee              |
----------------------------------
Now, free the first four allocations (14 bytes) but not the fifth:
----------------------------------
|              eeee              |
----------------------------------
Now, try to allocate 16 bytes. Oops, can't because there are 14 bytes "free" before and after "eeee",
but no one single block that can fit 16 bytes, even though there's nearly double that much free (28 bytes free).
On systems with virtual memory, fragmentation is less of a problem than you might think, 
because large allocations only need to be contiguous in virtual address space, not in physical address space. 
So in my example, if I had virtual memory with a page size of 2 bytes then I could make my 16 byte allocation with no problem.
Physical memory would look like this:
----------------------------------
|ffffffffffffffeeeeff            |
----------------------------------
whereas virtual memory (being much bigger) could look like this:
------------------------------------------------------...
|              eeeeffffffffffffffff
------------------------------------------------------...
The classic symptom of memory fragmentation is that you try to allocate a large block and you can't, 
even though you appear to have enough memory free. Another possible consequence is the inability of the 
process to release memory back to the OS because each of the large blocks it has allocated from the OS, 
via malloc etc has something left in it, even though most of each block is now unused.


class can be instantiated both on heap or stack.
When to use stack allocated or heap allocated variables?
Clarification: we can have pointers to stack allocated and heap allocated objects
stack vs heap allocated objects vary by lifetime, size considerations, r/w performance, ownership, and mem management.

Stack - tied to scope, limited space (small objects), faster r/w (due to small objs), stored inline in memory, sole ownership,
automatic destruction.
heap - lifetime adheres to dtor/delete, uses pointers (new), usually for large objects, multi-ownership (multiple ptrs to
single allocated mem), slower performance due to possible fragmentation, manual allocation + destruction.

Rule of thumb: if we are dealing with a small sized object (float, array of 5 elements,...) whose scope is is limited
allocate it on the stack.
Otherwise if we are dealing with some big object (excel data, texture,...) - use the heap.

Regarding struct vs classes in terms of stack vs heap, besides structs in cpp have public members by default and classes having
private members, If the class/struct is a small POD type (Vector3, Color, Matrix, Quaternion) - use stack allocated structs (like Unity)
In C# called a value type (as opposed to reference type), stored inline on the stack (less space, but fast access,
less space to traverse) + structs are immutable (in C#) and don't require garbage collection (automatically cleaned up),
as opposed to heap allocated objects requiring GC (cpu cycles).
Especially critical if for some reason we need to create a Vector3 each frame (if heap --> cause GC spikes)
Cleaner code: no need for new, delete. Destruction is guaranteed, no leaks.
Stack objects must be small not only because the stack is small, but because their content is stored inline
in the variable, which must be copied when returning/assigning, as opposed to pointers where we copy only the address.
This is mitigagted in cpp with move semantics and with ref keyword in C#.
In addition, heap objects can be created in run-time dynamically (variable sized array), and incur indirection
cost for the pointers (although we can have ptrs to stack allocated objects), and performance hit during allocation/deallocation.
If the entity is NOT a POD, for example a Player class with many fields, huge arrays, methods, etc - use a class that is allocated
on the heap.
Also use smart pointers, instead of raw ptrs for heap allocated objects, which ensure ownsership and activate the dtor of the object.
***************************************************************
Use stack allocation for small, local, short-lived POD style objects (Vector3)
Use heap allocation for objects whose lifetime exceeds the scope, resource heavy (Texture), 
and heap allocated classes for non-POD entities (Player)
******************************************************************

Note that class vs struct debate is only relevant for OOP, in c we only have structs - plain data ggregation, no methods.
So if POD use struct on stack, and for everything else - consider it as a class on the heap (allocate on heap).
*/

#include <iostream>

struct Vector3
{
    float x, y, z;
    Vector3() :x(10), y(11), z(12) {}
};

int main()
{
    int value = 5; //stack, implicit auto storage specifier. see 21a_storage specifier
    int* hvalue = new int; //heap value, new allocates on heap, dynamic storage specifier
    *hvalue = 5;
    
    Vector3* vec = new Vector3(); // heap
    //array of vector pointers are stored contiguously on the stack, but the vectors they are pointing to on the heap are probably scattered
    //in contrast, array of vectors on the stack are probably stored contiguously (better performance, less memory jumps - cache advantages).
    //using smart pointers - the also call new, smart pointers to this for us (same for delete)
    delete vec; // clear the data to which the pointer points to, but the pointer isn't deleted since it
    //is a plain-old-data store on the stack - it will be gone when leaving the scope
    //delete actually frees memory (?) in contrast to stack
    /*
    the new keyword will call malloc in turn will call the os underlying specific function and allocate memory on the heap.
    when we start a program the os allocates memory for that program. the program will maintian a free list which keeps tracks
    of free blocks and their positions. when we ask for heap memory using malloc, the program traverses the free list, finds a free block 
    of memory, the os gives back memory address (pointer to the beginning of the newly allocated memory block)
    and record things such as the size of the allocation and that the block is reserved.
    implementation of malloc is complex - in contrast to the simplicity of the stack. if we ask more memory than the available memory
    in the free list than our program asks the os for more memory - even more expensive! whole process vs 1 cpu instruction of stack.
    The differences between stack & heap is in allocation. the other benefit of stack allocation is that they are close together
    and fit into 1 cpu cache line. this reduces cache miss - a failed attempt to read or write a piece of data in the cache
    which results in a main memory access with much longer latency. this problem becomes prominent with lots of variables,
    which increases the risks of cache misses. THE ALLOCATION IS THE SLOW PART - this is the peformane difference!
    the "delete" is also a heavy operation.

    WE can preallocate memory preheand and minimize the performance hit(?).
    the actual access to memory is negligble, tha allocation itself is slow.
    continuously allocate each frame/loop is bad. memory manangement is necessary.

    The only reason to allocate on the heap is for more data (Textures of 50 mb) or for lifetime reasons
    */


    /*
       stack variables are scoped, when exiting this scope the stack variables will be gone (all popped),
       and the stack pointer will return to the memory address it pointed to before entering this scope
       in most stack implementations the stack is grown backwards(?).
        it doesn't cost performance to "free" memory, done automatically (1 cpu instruction)
        by moving the pointer to the address it where before entering the scope!!!
       only the stack pointer changes after leaving the scope!! the data itself is stil in the memory
    */
    {
        int value2 = 4;
        // cc cc cc cc cc cc in memory window in debug mode means uninitialized variable
        // memory on the stack stored contiguously 00000001 00000002 00000003
        //debug adds safety guard around variables so we won't accidently overflow/access them
        int array[5];// stack
        for (int i = 0; i < 5; i++)
            array[i] = i;

    }
    

    std::cout << "Hello World!\n";
}

