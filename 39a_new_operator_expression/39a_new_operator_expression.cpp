#include <iostream>

class Player
{
public:
    double m_precision;
    int m_health;
	Player(double precision = 100.0, int health = 100) : m_precision{ precision }, m_health{ health } {}
};



int main()
{
    Player p;
    std::cout << sizeof Player;// size of is an operator, not a function! doesn't require parenthesis


    //1. tl;dr "operator new" (can be overloaded) vs "new expression" (cannot be overloaded)
    //-  performs only memory allocation (chunk of memory in the requested size arguemnt), and returns a void pointer to it
    // without calling a ctor, and without casting the pointer
    Player *p1 = (Player*) operator new (sizeof Player); //operator new (calls the "new" function - operators are function) 
    //allocates raw memory - malloc style! cpp requires an explicit cast 
    //meaning it requests 16 bytes from the os (the size of Player) and the os returns an address to the begining
    //of a 16 byte memory chunk. the malloc/new operator in turn returns a void pointer to that memory address
    //so we have to cast it to a pointer to Player, otherwise we won't be able to read/write
    // IMPORTANT: operator new doesn't initialize a Player object, since no ctor is invoked (like with new expression), 
    // albeit the memory block is partitioned according to the members of the class!!!!!
    // if the pointer was of type void, access to any information regarding
    // Player class was invalid
    
    /*
    we can also override operator new:
    we can see that indeed the operator returns void* to the allocated memory, thus
    we need to cast it.

    void* operator new(size_t size) //size - the size of the chunk of memory (sizeof player)
    {
        return malloc(size);
    }

    in C operator new is equivalent to malloc, for example if we have the following struct:
    struct Node
    {
        struct Node* next;  (in C we denote full type "strcut Node", CPP allows to omit "struct" keyword)
    };

    //malloc asks the OS for a memory at the sizeof(struct Node), malloc returns a void* to that chunk
    //of memory, and then we cast the void* to a "struct Node*"
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    using typedef we can shorten the declartion above:
     typedef struct node { struct node* next; } Node;
    Node* node = (Node*)malloc(sizeof(Node));
    //(Node becomes alias for "struct node", but we must use "struct node" inside the struct
    //since the alias Node is not yet delcared inside the scope of the struct
    // search typedef for more info.

    */


    //2. tl;dr new expression (keyword) - allocates memory, constructs an object there using the arguments in (),
    //and returns (expression because it is evaluated to a single output...) a pointer type T (casted) to that location
    Player* p2 = new Player(); 
    //new expression (built in keyword) does three things:
    //1. invokes "operator new", aka, calls the new function to allocate memory to accomodate sizeof(player) as described above
    //this function returns a void a pointer that points to the start of the newly allocated memory chunk.
    //2. calls the constructor of Player which will be built in the allocated memory in step 1
    //3. performs an implicit cast from the void* returned from 1 into the type that is derived from "new X" - when X is the type
    // Player in this case


    //3. placement new - new expression with additional parameters - like "new expression" above + allows specifying an address
    void* address_of_player = operator new (sizeof Player); // allocating sizeof player memory chunk into a void pointer
    Player* p3 = new (address_of_player) Player(); // placement new allows to specify a specific address of where
    //to construct the Player object, and later placement new returns a pointer of type Player.



    https://docs.microsoft.com/en-us/cpp/cpp/new-operator-cpp?view=msvc-160
    https://stackoverflow.com/questions/25717639/why-does-operator-new-function-need-cast#:~:text=In%20C%2B%2B%20(unlike%20in%20C,a%20cast%20in%20C%2B%2B.
    https://stackoverflow.com/questions/1885849/difference-between-new-operator-and-operator-new
    https://stackoverflow.com/questions/9832107/void-in-malloc-vs-operator-new
    std::cout << "Hello World!\n";
}
