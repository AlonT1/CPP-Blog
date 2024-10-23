#include <iostream>

//note: everything below is also relevant for classes, not only structs

int main()
{
    typedef int INT; //int is now INT
    INT x = 4;
    std::cout << x << '\n';

    using INT1 = int; //using is more modern and clear
    INT1 y = 5;
    std::cout << y << '\n';

    // in cpp the following declaration works well, but in C we will get an error:
    //unkown type name Node:
    struct Noder // we declare a new type: "struct Noder", a type just like "int" is  atype
    {
        //although the new type is "struct Noder", cpp allows us to omit the "struct" keyword
        //for convinience. in C, the delcaration below would have looked like: "struct Noder* next",
        //i.e we must declare "struct Noder*" in C, we cannot omit the struct keyword
        Noder* next; 
    };

    //equivalent to "struct Noder noder" (struct keyword is optional in CPP, mandatory in C)
    //also equivalent to "Noder noder = noder()";
    Noder noder; 

    struct Character { int pos; };
    Character c{ 4 }; //in CPP structs are aggregates, we can initialize them at declaration with list init {}
    struct Character c2; // in C we must declare first (with full "struct Character" type) - fields are zeroed/nulled out

    c2.pos = 4; // and then assign a value


    struct Noder2
    {
        struct Noder2* next; //in c we must fully declare "struct Noder2*"
    };

    /*
    in c the full name "struct Noder2". with cpp "Noder2" is suffice. Why?
    Syntactically both treat struct almost the same. Only C++ has added an extra rule that allows to omit the struct
    (and class) keyword if there is no ambiguity.
    If there is ambiguity, also C++ requires the struct keyword in some places. 
    A notorious example is stat on POSIX systems where there is a struct stat and a function stat - explained:
    POSIX (Portable Operating System Interface) defines the API, along with command line shells and utility interfaces, 
    for software compatibility with variants of Unix and other operating systems (defines shell commands such as mv,
    grep, thread apis - pthread_create, pthread_join,...)
    POSIX-Compliant Systems (Unix, macOS, Windows,..) specify a "stat" function:
    
    int stat(const char *path, struct stat *buf); // used to retrieve information about a file.

    but POSIX also specifies a stat struct used to retrieve filesystem status information:
    
    struct stat
    {
	    dev_t     st_dev;     // ID of device containing file
	    ino_t     st_ino;     // inode number
	    // ... other members
    };

    When you want to use "struct stat" in C, and not the "stat" function you need to include the struct keyword
    to avoid confusion with the stat function.
    
    */
    struct Noder2 noder2;

    // we can instantiate a struct directly after declaration:
    struct Entity
    {
        // a reference to itself (in C we must write the full type name struct Entity*)
        Entity* next{ nullptr }; 
        int x = 6;
    }node;    //"node" is an instance of Entity. equivalent to " int  node;"
    //where "int" and "struct Entity" are both types (declaration specifiers)
    //and "node" is the identifier (declarator).
    //the same declaration also works in C

    std::cout << node.x << '\n';

    //when using typedef we denote that "struct node" is now equivalent
    //to "Node". In other words "Node" isn't an instance like "node" above, 
    //but is a synonym for "struct Entity1".
    typedef struct node // "node" is necessary if we want to refer to the struct:
    {
        // reference to itself (Node synonym is not defined yet, so we must use "struct node" type).
        // in c we must declare the full type "struct node*", cpp allows us to omit "struct" keyword
        node* next{nullptr}; //struct node* next; in C
        int x = 7;
	}Node;

    Node node1; //also works in C (saves us wrtiting "struct Entity1 entity;"

    std::cout << node1.x << '\n';


}
