#include <iostream>

/******************classes can be nested**************************
classes can also be used as a field, just like int, float, etc... they are all data
types, albeit Classes/structs are user-defined data types.
Therfore all encapsulation rules that apply to pods and other objects, apply
to nested class declarations.

benefits:
Better encapsulation - Node class only needed when used with List? -> nest Node
class inside List
*/

//******************Most simple example of a nested class****************
class Entity1
{
public:
    int x;

    class Nested
    {
    public:
        int y;
    };
};

//******************encapsulation****************************
// the enclosing class can instnatiate 
class Entity2
{
public:
    //public nested class. instantiable from outside, but we cannot use its private fields
    //just as we can't use private fields of any other class form outside.
    class PublicNested 
    {
    private:
        int y;
    public:
        int w;
    };


private: 
    // private nested class, inaccessible from outside like any other field 
    // (although accessible from within the class)
    class PrivateNested 
    {
    public:
        int z;
    };

    //both can be used inside the class, whether private or public (like any other field!)
    PublicNested a;
    PrivateNested b;

    void test()
    {
        //a.x; // we can't access private fields of public/private class
        b.z; //we CAN access public fields of public/private class
    }
};


/******************Practical useage***************************
since Node class is only used in the context of the List class, we can encapsulate
Node inside List
*/

struct List
{
private:
    struct Node
    {
		int* next;
        int* prev;
        int data;
    };

public:
    Node* root;
};


int main()
{
    Entity1 entity1_enclosing; // instantiating outer class
    Entity1::Nested entity1_nested; //instantiating nested - remeber that classes are type of namespaces

    Entity2::PublicNested public_nested;
    public_nested.w; // public fields of nested classes are accessible, but private fields aren't
    // Entity2::PrivateNested private_nested; // inacessible! this field is private!

    std::cout << "Hello World!\n";
}