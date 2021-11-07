#include <iostream>

int main()
{
    typedef int INT; //int is now INT
    INT x = 4;
    std::cout << x << '\n';

    using INT1 = int; //using is more modern and clear
    INT1 y = 5;
    std::cout << y << '\n';

    // "node" is an instance of Entity. equivalent to " int  node;"
    //where "int" and "struct Entity" are both types (declaration specifiers)
    //and "node" is the identifier (declarator).
    struct Entity
    {
        Entity* next{nullptr}; // a reference to itself
        int x = 6;
    }node; 
    std::cout << node.x << '\n';

    //but when using typedef we denote that "struct Entity1" is now equivalent
    //to "Node". In other words "Node" isn't an instance like "node" above, 
    //but is a synonym for "struct Entity1".
    typedef struct Entity1
    {
        Entity1* next{nullptr};// reference to itself (Node syonym is not defined yet, so we use Entity1)
        int x = 7;
	}Node;

    struct Entity1 e; //equivalent to "Entity1 e" same thing occurs with class declarations
    std::cout << e.x << '\n';
    Node node1;
    std::cout << node1.x << '\n';

}
