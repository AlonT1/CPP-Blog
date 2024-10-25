/*
copy - copying data, memory, primitives, so we can modify them. takes time and performance!
in cpp when we use the assignment operator we ALWAYS copying the value
*references are const aliases...

copy ctors promise not to modify the object we copy from (it is const for a reason),
therefore they are considered "safe" as opposed to move semantics which does modify the "other" object
by nulling the members of "other* out (because it's members or moved to "this" class)
(search "exception" in 89_move_semantics)

with copy ctor we deep copy all the fields from the object we copy from into the fields of our object.

//Note that copy ctors can also have initializer list!
*/

#include <iostream>

struct Vector2
{
    float x, y;
};

class String
{
private:
    char* m_buffer;
    unsigned int m_size;
public:
    String(const char* string)
    {
        m_size = strlen(string) + 1; //+1 for null terminator which strlen doesn't count
        m_buffer = new char[m_size]; 
        memcpy(m_buffer, string, m_size);
    }

    /*Copy constructor

    the main point of a copy ctor is to copy data from an existing object into the newly, not-yet-created constructed
    object. The object we copy from is NOT DAMAGED and NOT MODIFIED by definition (it is const for a reason).
    we DEEP copy all members from the object we copy from into the new object without modifying the object we copy from.

    If no user-defined copy constructors are provided for a class type (struct, class, or union), 
    the compiler will always declare an implicit copy constructor as a non-explicit inline public member of its class.
    basically implicit ctor works well for pod's public/private/protected.
    For copying objects and heap allocated objects, its better to overwrite it.

    IMPORTANT Notes: 
    1. the copy ctor is only used for initializating an object! e.g: Entity a = b.
    this is not an assignment operation, because a is not-yet-created! it is 
    implicit converted to: Entity a = Entity(b), and rhs is constructed into the stack frame of a 
    (without copy ctor - RVO optimization, see 25_constructor)
    2. the reason that we pass a const lvalue reference to a String (String&) is because:
        a. it is the convention - and what the compiler expects.
        b. const lvalue reference can accept prvalues such as "new T() (temporary) because
        temporaries are automatically const, thus const lvalues reference extend the lifetime
        of prvalues from expression scope to the scope of the const lvalue ref
        c. reference saves us from making a copy. - So its comfortable that we can pass a prvalue
        without the need to manually create a new object beforehand.
        d. Passing by value, except from being more expensive performance wise than reference,
        also can cause an inifinite copy ctor loop! if the copy ctor was: String(const String other) 
        and the call was String a = b  where b is an existing String object, 
        and a is said to be initialized with a copy-ctor:
            1. a = b -> copy ctor of "a" is invoked. because "a" is not-yet-created, and
            we try to construct and initialize it with already-created "b" object, 
            cpp ALWAYS invokes a copy ctor (if "a" was already-created a copy assignemnt
            would have been invoked).
            2. then b would have been assigned to "other", resulting in "String other = b" (because
            we want to copy b")
            this would have invoked another copy ctor where "other" would again been assigned with
            b, and so forth in an endless loop.
            To solve this, "other" now becomes  a const lvalue ref and when assigned with lvalue b, "other"
            becomes an alias for b, and there is no need to initialize "other" with a complete copy of "b" - "other" IS b.
          
    3. the great thing is that by concept, copy ctor doesn't perform any changes on the "other" object
    because it is a const. the copy ctor's intent is to perform a deep copy of "other" (aka "b"), 
    without modifying it.  "b" is  is safe from any changes, whether its is an rvalue which is about to die,
    or an lvalue used by other components ( both (rvalue and lvalue) are acceptable arguemnts for a const lvalue ref).
     
    */
    String(const String& other) //other is the object we want to copy
    {
        m_size = other.m_size;
        m_buffer = new char[m_size];
        memcpy(m_buffer, other.m_buffer, m_size); //deep copy of m_buffer
        std::cout << "copied!" << std::endl;
    }

    ~String()
    {
        delete[] m_buffer;
    }
    
    //str[2]='a' [] is a function,  2 is the index, returns a reference to the char
    // at index 2  and then this char is assigned with 'a'
    char& operator[](unsigned int index)
    {
        return m_buffer[index];
    }


    friend std::ostream& operator<<(std::ostream& stream, const String& string);
};

std::ostream& operator<<(std::ostream& stream, const String& string)
{
    return stream << string.m_buffer;
}


//in cpp everything is passed (copied) by value! passing argument to a function is like assigning "=".
//so in a function like this: void print_string(String string), passing a String
//object as an arguemnt, invokes the copy ctor (wastes performance - cpu cycles)! therefore its better to pass a reference
//to the existing object. const means that the object can't be changed 
//read: string is a reference to a String object that is constant
void print_string(const String& string) 
{
    std::cout << string << std::endl;
}

int main()
{
    int a = 2;
    int b = a; //copying the value!
    b = 3; // a remains 2

    Vector2 c = { 2, 3 };
    Vector2 d = c; //d is not yet created, while c is already created ->
    //invokes copy initialization with implicit copy ctor
    //the implicit copy ctor works well with pod's, it copies them succefuly, in a deep manner

    //****************************************************************
    //the compiler generates the following code for the statement above :
    //Vector2 d = Vector2(c);   which is equivalent to:
    //Vector2 d = Vector2(static_cast<const Vector> (c)); // the compiler performs implict cast
    //to const Vector because the copy constructor has a const parameter
    //****************************************************************

    d.x = 5;  // c.x is still 2! 

    Vector2* e = new Vector2();
    Vector2* f = e; //copying where the pointer points to
    f->x = 4; //affects both e and f

    String first = "shalom"; // copy initialization via copy ctor through
    //implicit type conversion of temp rhs
    //compiler recognizes String supports a ctor with const char*, 
    //rhs is implicitly converted to -> String("shalom")
    //and constructed right in the memory location of first (of main())


    /*because "first" is a class reference variable, the assignment operator "=" 
    uses an implict copy ctor  that copies all the values that "first" has into "second". 
    These values are m_size value and m_buffer value (the memory address of the string)
    this is a shallow copy - (only stack allocated are copied) the content of where m_buffer points to is not copied (the content on the heap)!
    only the pointer that points to that location is copied!
    meaning that a change in *m_buffer in "first" and "second" will reflect in both of them!
    to overcome this, we must perform a deep copy by overriding the implicit copy ctor and perform a
    total copy including the content that lies at the pointed location*/
    String second = first;
    second[2] = 's';
    std::cout << first << std::endl;
    std::cout << second << std::endl;
    
    print_string(first);

    //with an implicit copy ctor, which causes m_buffer to be "shallowly" copied,
    //when reaching the end of the scope, m_buffer will deleted once for "first",
    //and 2nd time for "second" -> error because a "delete" was already executed 
    //on m_buffer in "first", and because m_buffer is shared, the second delete invokes an error for an already deleted object
}


