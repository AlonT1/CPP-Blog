
#include <iostream>

using String = std::string;

class Entity
{
private:
    String m_name; // no need for a dtor that will delete stack allocated objects such as m_name
public:
    Entity() : m_name("hello"){}
};

int main()
{
    Entity e; // auto storage specifier, on the stack, faster allocation than heap, locally scoped to this code block.
    //fastest way for object instantiation in cpp, "managed", objects don't need to be freed.
    //stack is approx. 1 mb, not fit for huge classes. java is always on the heap, also in c# (except from struct). in cpp we can also allocate on stack!
    Entity e1 = Entity(); //equals to above with addtitonal asssignemnt...
    Entity* ptr = &e; // possible  same as int x=4; int* ptr = &x


    Entity* ptr2;
    {//empty scope
        Entity * entity = new Entity(); //new allocates on heap size according to the size of Entity., dynamic storage specifier (thanks to "new")
        // the pointer entity will be destroyed (the memory adress of the ptr &entity gets deleted)- but the object will still exist on the heap!!
        //visible by inspecting the memory address of the object (== value of the pointer - the pointer points to it) - we can see that the object still exists there!
        //everything with "new" must be cleaned with "delete" to prevent memory leaks
        ptr2 = entity;
    }

    delete ptr2; //the pointer itself is stored on the stack (automatic storage specifier). "delete" will only delete the object that the pointer points to
    //"delete" will not delete the pointer itself. The pointer will be "popped" when reaching the end of the current scope
    
}
