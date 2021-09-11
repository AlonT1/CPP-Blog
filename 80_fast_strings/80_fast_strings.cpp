
#include <iostream>
#include <string_view>

/*
Notes:

1. There are no heap allocations when dealing with std::string_view(only stack allocation)
as opposed to std::string that contains heap allocations. Passing string_view by value
to a ctor/function will not invoke "new" allocations, as opposed to std::string

2. std::string_view is even more optimized than char[]
when using char[] str = "shalom", string literals such as "shalom" are stored in the binary, then copied to str array.
string_view saves this copy by directly "constructing" a window into the binary.
the benefits of char[] or std::string is that they are modifiable, while string_view is immutable.
string_view vs const char* - probably the same in terms of performance, both immutable, but
string_view has fancy string methods. *in my benchamrks: string_view even faster than const char* strings

3. std::string_view acts as a pointer to a std::string or a char* C string. 
It contains a pointer and a length. 

4. if we need a string that requires modification of specific characters - use std::string,
otherwise string_view

5. in a loop trying to create an instance of a class that contains std::string_view as single data member,
the performance of creating that instance many times doesn't seem to cost performance:
the construction time of 100 instances is equal to 10000 instances - probably because the compiler
is able to reuse the string right from the read-only section where it is stored when it was first allocated
(read "26_destructor" memory layout notes).
However with std::string the the construction time of 1000 instance is far worse then 100 instances, probably because
of the repeating "new" allocation per each string repeatedly on the heap.


8. std::string a = b, when a already initialized, invokes a deep copy ctor as opposed to:
std::string_view {another_string_view} which creates another window to
the same string literal stored in another_string_view

9.
    std::string_view foo{ "foo"};
    std::string_view bar{ foo };
    foo = "goo";
    std::cout << foo << bar; // foo prints goo, bar is still foo!
    this happens because when we assgined bar with foo string_view
    bar has copied "foo" string by value into itself. In another words, both foo and bar
    point to different strings! 
    if we want two string_views to point to the same const char*:
    std::string_view foo{ "foo"};
    std::string_view* bar{ &foo }; // a pointer!

9. string_views weight 8 bytes (size of a ptr on a 64 bit architechture)
vs std::strings that weigh 28 bytes

*/


static uint32_t s_alloc_count = 0;

//"new" overload for testing out number of allocations
//read  39a_new_operator_expression for more info about this
void* operator new(size_t size)
{
    s_alloc_count++;
    std::cout << "allocating: " << size << " bytes. " << "allocations: " << s_alloc_count << std::endl;
    return malloc(size);
}

void print_name(const std::string& str)
{
    std::cout << str << std::endl;
}

// we can pass std::string to string_view but not to string_view&. explained more above
void print_name_view(const std::string_view str) 
{
    std::cout << str << std::endl;
}

int main()
{
    std::string name = "Nicholas Cage"; // New Heap allocation. this happens in more then 16-character-length
    //strings (compiler-dependant)/debug mode, for less then 16- characters cpp MIGHT optimize string construction
    //from being allocated on the heap to being allocated on the stack (called small string optimization SSO)
    std::string first_name = name.substr(0, 8); // New Heap allocation - creates a new mutable string with "nicholas"
    std::string last_name = name.substr(9, 14); // New Heap allocation
    print_name("hello"); // New Heap allocation!
    //total of 4 heap allocations!!

    //substring optimizations 
    const char* name1 = "Nicholas Cage"; //stack allocated
    //- std::string_view
    //- a const char pointer to an existing string (view to a substring)!!! without new allocations!
    //only for cpp17. it is possible to write the string_view technique manually.
     std::string_view first_name1(name.c_str(), 8); //stack allocated.
    //c_str = const char string. we are pointing to where the data is, and size
    //which denotes the number of bytes until the end of the string
     std::string_view last_name1(name.c_str()+9, 4);
     print_name_view("yes");
     //total of 0 heap allocations!

    /*
    Best practice:
    Use std::string_view instead of C-style strings.
    Prefer std::string_view over std::string for read-only strings, unless you already have a std::string.
    */
     std::string_view view = "makore"; // ONLY READ ONLY STRINGS, equivalent to char* to something, only safer with extra features
     //no heap allocations!!!!!!!!!!!!!!!!!!!!!!!!!!!!



}
