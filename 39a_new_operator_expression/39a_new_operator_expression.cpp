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


    //1. tl;dr operator new (explicit call to operator new function - can be overloaded as seen above)
    //-  performs only memory allocation (chunk of memory in the requested size arguemnt), and returns a void pointer to it
    Player *p1 = (Player*) operator new (sizeof Player); //operator new (calls the "new" function - operators are function) 
    //allocates raw memory - malloc style! cpp requires an explicit cast 
    //meaning it requests 16 bytes from the os (the size of Player) and the os returns an address to the begining
    //of a 16 byte memory chunk. the malloc/new operator in turn returns a void pointer to that memory address
    //so we have to cast it to a pointer to Player, otherwise we won't be able to read/write
    //from/to that memory address ((the compiler will treat that location as
    //if it stores a Player object, rather then void - stores nothing).
    //Note: cpp requires an explicit cast of the void pointer returned by
    //malloc/new unlike c where the cast is done implicitly.
    // IMPORTANT: operator new doesn't construct a Player object!!!!!
    //p1 just points to the start of an empty memory chunk of 16 bytes, which supposed to store
    //a Player object (being a Player type, allows the pointer/compiler to reach out for class members/functions)
    //that are stored in that memory address, if the pointer was of type void, access to any information regarding
    //Player class was invalid
    
    /*
    we can also override operator new:
    we can see that indeed the operator returns void* to the allocated memory, thus
    we need to cast it.

    void* operator new(size_t size) //size - the size of the chunk of memory (sizeof player)
    {
        return malloc(size);
    }
    */

    //2. tl;dr new expression (keyword) - allocates memory, constructs an object there, and returns
    //(expression because it is evaluated to a single output...) a pointer type T (casted) to that location
    Player* p2 = new Player(); 
    //new expression (built in keyword) does three things:
    //1. invokes "operator new", aka, calls the new function to allocate memory to accomodate sizeof(player) as described above
    //this function returns a void a pointer that points to the start of the newly allocated memory chunk.
    //2. calls the constructor of Player which will be built in the allocated memory in step 1
    //3. performs an implicit cast from the void* returned from 1 into the type that is derived from "new X" - when X is the type
    // Player in this case

    //note:in step 1 we invoked the operator new which requires an explicit cast from the returning void* to Player*,
    //but we didn't perform the cast here. The reason is that step 3 of the "new expression" does this implicitly.
    //this is also noticable when overriding the new operator (function) which returns void* - essentialy
    //replacing step 1 of "new expression". the construction + implicit conversion of pointer of type T will be 
    //the "new expression" later performed by

    //3. placement new - new expression with additional parameters - like "new expression" above + allows specifying an address
    void* address_of_player = operator new (sizeof Player); // allocating sizeof player memory chunk into a void pointer
    Player* p3 = new (address_of_player) Player(); // placement new allows to specify a specific address of where
    //to initialize the Player object, and later placement new returns a pointer of type Player.



    https://docs.microsoft.com/en-us/cpp/cpp/new-operator-cpp?view=msvc-160
    https://stackoverflow.com/questions/25717639/why-does-operator-new-function-need-cast#:~:text=In%20C%2B%2B%20(unlike%20in%20C,a%20cast%20in%20C%2B%2B.
    https://stackoverflow.com/questions/1885849/difference-between-new-operator-and-operator-new
    https://stackoverflow.com/questions/9832107/void-in-malloc-vs-operator-new
    std::cout << "Hello World!\n";
}