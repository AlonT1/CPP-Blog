/*
operator - symbol used instead of a function to perform something (*, !, &, <<, +, - new, delete, ())
overloading - giving a new meaning to..
allowing to define or change an action of an operator
Note - with function overloading we are "overloading functions" that already exist
When overloading operators, the operator function does not necessarily exist in our custom class,
so overloading here means more  "IMPLEMENTING" the operator, not overloading
an existing function such as "operator+".
PERATORS ARE FUNCTIONS instead of the name "add" the name can be "operator+".
use of operator overloading should be minimal and make perfect sense (vector addition)!
*/

#include <iostream>

class Vector2
{
private:
    int m_z;
public:
    float x, y;

    Vector2 (float x, float y) : x(x), y(y), m_z(-1) {}
    
    //these 2 methods are kept as an api, to allow the user to make use of them
    //if he likes. In addition they are used inside the operator overload functions
    Vector2 add(const Vector2& other) const
    {
        return Vector2(this.x + other.x, this.y + other.y); // creating a new object
    }

    Vector2 multiply(const Vector2& other) const 
    {
        return Vector2(this.x * other.x, this.y * other.y);
    }


    /***************operator overloads

    Assume the following statement:
    vec1+vec2 equivalent to vec1.opeator+(vec2), more specifically
    vec1.operator+(&vec1, vec2), where vec1 is the the lhs instance we work on.
    in a binary operator such as +, THE LHS OPERAND (vec1) IS passed to the member function
    as IMPLICITLY "THIS", and the rhs operand (vec2) is the "other" parameter, equivalent to:
    Vector2 operator+ (Entity* const this, const Vector2& other)
    BINARY OPERATORS MUST TAKE IN 2 PARAMETERS by definition (either 1 implicit ("this") and 2nd is explicit
    or 2 explicit). Unary operators take in 1 parameters.

    Const at the end is not mandatory, but recommended as operator+ should not modify class members
    */
    Vector2 operator+ (const Vector2& other) const  //equivalent to Vector2 operator+ (Vector2* const this, const vector2& other)
    //"operator" is a key word used to customize c++ operators for operands of user-defined types
    {
        /*
        thanks to copy elision, this object is constructed right in the memory
        address of the variable that we assign to (its memory location is pushed onto the stack).
        even though we've called the add function, there are no extra "copies" done.
        without copy elision, returning back from add would require:
        1.copy constructing vector2 from add() to the hidden parameter 
        */
        return add(other); //  return Vector2(this.x + other.x, this.y + other.y); // creating a new object
    }

    Vector2 operator* (const Vector2& other) const // equivalent to Vector2 operator* (Entity* const this, const Vector2& other) const
    //last const promises the the body will not change class variable members (unless they are marked as mutable)
    {
        return multiply(other);
    }

    // vec1==vec2 equivalent to vec1.opeator==(vec2) - vec1 (lhs operator) is "this", vec2 (rhs operator) is other
    bool operator== (const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!= (const Vector2& other) const
    {
        //return !(*this == other); //compare the actual data in the memory location of "this" with "other" object.
        //this==other, means compare the memory address with "other". alternative:

        return !operator==(other);  //calling the operator== overload function explicitly (other) is the arguemnt.
        //possible but not recommended to nest operator overload inside one another
        //the lhs (the left operand) will implicitly be the "this", the lhs instance of the class.

        //return x!=other.x || y!=other.y also possible
    }

    //copy assginment overload
    Vector2& operator= (const Vector2& other) 
    {
        if (this != &other) //ensuring we don't self-compare
        {
            x = other.x;
            x = other.y;
        }
        return *this; // if (this==other) return *this
        /*
        Important note: a=b is equivalent to a.operator=(b)  in this case we actually don't need to return
        anything becuase the "assignment" is performed in the above block,
        but when dealing with statements as a=b=c (chaining) which is equivalent to a.operator=(b.operator=(c))
        we have to return a value -> b.operator=(c) returns a ref to a Vector2 which later evaluated
        in a.operator=(the returned value)

        in addition, we return *this since we want to return the instance, an lvalue reference to the
        living instance to avoid creating new data (lvalue can be assigned with an lvalue).
        */
    }

    //unary operators take in 1 parameter, equivalent to: Vector2 operator++(Vector2* const this)
    //and since "*this" is passed implicitly to member functions, the parameter list appears empty.
    //the only reason we return Vector2 is to support chaining. we can return the object itself because
    //because we modify the object itself and we don't create any new object!
    Vector2& operator++ ()
    {
        x++;
        y++;
        return *this;
    }

    //FRIEND OVERLOADED OPERATER (as opposed to MEMBER OVERLOADED OPERATOR displayed above)
    /************
    Why do we use a friend function to overload operator<< ?

    1. operator<< (belongs to ostream, std::cout is type osstream) and is binary function which
    takes in 2 parameters - the lhs operand (ostream) and a template generic rhs operand.
    in this case the Vector instance) - this is just how it is defined.
    therefore this function must be implemeneted outside of the class (free function)
    because otherwise the first paramter will implicitly be "*this" which will mount to 3 parameters, and to overload
    the function we must use the same number of parameters. their name can differ (we override signatures
    and signatures can be written without names, the types must match. in case of operator<< the first
    param type is ostream and the 2nd param is generic (supports our custom object)
    Thus, we can't overload operator<< via a class function, because "this"
    will be implicitly passed, including std::cout and the rhs parameter (3 total parameters and not the required 2).

    2. Unlike operator+ and operator* which are functions that did not exist in our custom class, operator<<
    does exist in iostream. In fact its signature is:
    std::ostream& operator<< (std::ostream& os, const T& obj); // note the generic 2nd parameter
    So "operator overloading" with operator+ means - IMPLEMENTING the + opeartion,
    But with operator<< we REALLY overload the operator (function) since its already defined in ostream.
    So the engineers had 2 choices to enable us to overload<<:
    1. either our class should inherit from ostream and override operator<< as a member function
    2. make operator<< as a free member function.
    By going with (2) its much easier to overload the function (no need for inheritance), and if operator<<
    needs to access one of the class members (i.e act like a member function) we can simply friend it.

    
    The only reason to "friend" this function is to give it access to the private
    fields of "other". otherwise, the function can be non-friend "normal"
    */
    
    friend std::ostream& operator<< (std::ostream& stream, const Vector2& other);
};

/*
similar to toString in Java
Notes:
1. std::cout << "hello" is equivalent to std::cout.operator<<(std::cout, vector) 
2. std::cout is the lhs operand, and an object of class ostream where operator<< is defined.
3. "Vector2& other" is the rhs operator, "operator<<" is the operator
4. returning std::ostream& - we don't want to create a new ostream ->
std::ostream cannot be copied in cpp, thus it can and only be refrenced!
5. ostream is an ouput stream, used to hold output for data consumers and it is write
only (we write to it and a consumer (monitor, printer) reads from it).
6. returning std::ostream& helps in concatinating strings << << (chaining).
7. Although "stream" is a local variable, it is bound to a non local 
object ostream and we return the same ostream we got into the function,
the point is that stream does not die in the scope of the function, and it is ok
to return it back since ostream lives outside the function.
*/
std::ostream& operator<< (std::ostream& stream, const Vector2& other)
{
    //other.m_z is private, but accessible thanks to "friending" this outer function
    return stream << other.x << ", " << other.y;
}

/*
Notes about friend functions:

1.  friend functions such as "friend std::ostream& operator<< (std::ostream& stream, const Vector2& other);
    can be implemented with the declaration inside the class itself!
    the behaviour is exactly the same as declaring friend inside.
    the class and implementing it outside as seen here, but when calling this function, a "this" reference won't be passed.

2.  Although friend functions can be declared and even defined inside the class they are friends with,
    friend functions DO NOT belong to the class (they are free as normal functions),
    therefore the definition inside the class is for
    organizational purposes only, . This evident by the lack of need to write the namespace
    of the class when defining the friend function, for example:
    in a header file, inside a Vector2 class we declare:
    friend std::ostream& operator<< (std::ostream& stream, const Vector2& other);
    in a cpp file, the definition would not contain Vector2:: scope access, because the function
    is not in the namespace of the class, and do not belong to it (as opposed to member functions),
    therefore the definition would be:
    std::ostream& operator<< (std::ostream& stream, const Vector2& other)
    {
    }
***************************************
3.  friend functions grant access to the private members of the friended class that is passed
    through the arguments (we can access the private members of "Vector2 other" parameter
    because the the is declared inside the Vector2 class body with a friend keyword).
    because of this friend functions break "data hiding" (private is visible), and should
    be used only when necessary.
***************************************
4.  there is no hidden *this parameters in friend functions, all parameters are explicit

5. friend function cannot be declared virtual - only member functions can be virtual
*/


/*
The following is a non-friend function ("free" normal function) that do not belong the class
when the function is class member the lhs operand is the implicitly  "*this" pointer as the 1st parameter (operand).
when the function is free we must provide the lhs operand explicitly, there is no "*this"!
*/
Vector2 operator- (const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

/*
What if in addition to the free operator- function above, we also have a member function:

Vector2 operator- (const Vector2& rhs) const 
{
    return Vector2(x - rhs.x, y - rhs.y);
}

Can both exist together? the answer is yes
Vector2 result1 = v1 - v2;  // Calls the member function    
Vector2 result2 = operator-(v1, v2);  // Calls the non-member function

The C++ standard defines that when both member and non-member functions can be called,
the member function takes precedence for 
operations where the left operand is an object of the class.

*/




/*
further details at the function call in main()
*/
Vector2& operator-- (Vector2& val) 
{
    val.x--;
    val.y--;
    return val;
}

/*
operator[] must be overloaded as a member function
(because member functions imply that we read/write the instance itself, 
rather then intersted in creating a new instance (fit for normal/friend functions))

Assume a class CustomString that supports:

char& operator[](unsigned int index)
{
    return m_buffer[index];
}
and str is of type CustomString:
str[2]='a';

"str[2]": [] is the operator (function),  2 index is the operand.
[] returns an lvalue reference to the MEMORY LOCATION
of m_buffer[2] - in other words, we get back m_buffer[2]
as if it was a modifiable variable, thus we can assign 'a'
to that memory location.


see 45 copy_ctor for further details
*/


int main()
{
    Vector2 pos(4.0f, 4.0f);
    Vector2 speed(0.5f, 1.5f);
    Vector2 powerup(1.1f, 1.1f);
    Vector2 res1 = pos.add(speed); //legit
    
    Vector2 res2 = pos.add(speed.multiply(powerup)); //pos+(speed*powerup) convoluted...

    //better to use operator overloading

    Vector2 res3 = speed * powerup;
    /*
     operator* is a binary operator (two operands, two argments).
    "speed" is lhs operand (1st operand), "powerup" is rhs operand(2nd operand).


    overload with a Member/Friend/Normal function?
    *********************************************************************
    tl;dr tl;dr Member/Friend/Normal function? - These are just guidelines!!!!
    which function signature to use? (the compiler will generate the appropriate function
    call according to the signature, explained in "speed * powerup" example above)

    1. Want to manipulate lhs? overload by member function  +=, -=, /=,  *=, [], =, ()
    2. want to create a new data (instance / overload an operator from an inaccessible class(operator<<)?
    overload by normal function:  +,-,/,* , <<
    3. same reasons in 2, but also grant access to private members of parameters
    and also have the possibility to define the overload inside the class*? overload by friend function

    *friend functions can be defined inside the class for organizational purposes
    by they aren't actually a part of the class , explained in notes about friend
    
    Notes:
    1.  Another reason to use free functions (normal /friend):
        The inability to add a member overload directly to the class, e.g:
        ostream supports operator<<: we cannot access ostream class and add a new member
        function to it that accepts  our custom object, therefore we must overload operator<<
        outside via friend/normal function.
        operator<< actually takes in ostream and a generic parameter, therefore
        when overloading operator<< we specify ostream as the first paramaeter in the overload
        and a concrete type as the second parameter. operator<< is a non-member function, 
        its "sits" outside the class, therefore we can override it per class.

    2.  Normal (outside function) vs Friend:
        Use normal /friend functions for overloading operators for the reason above (creating a new
        instance via 2 explicit parameters), and consider the following points:

            a. Encapsulation - a r/w of private members of the parameters is not necessary
            or alternativly we can get a hold of the private members through getters - better encapsulation,
            then friend functions who have direct access to private members

            b. Organizational - Use Friend functions when the r/w of private members of the parameters IS necessary and/or 
            we want that the definition of the overload to be inside the class for organizational
            purposes (although friend functions are not part of the class, explained in
            notes about friends).
            

    3.  Also note that although operators such as += are fit for member overload,
        they CAN be overloaded as a free functions (normal/friend) as well,
        but following the conventions is recommended. both are equivalent:
        Entity& operator+= (const Entity& other)
        {
            m_x += other.m_x;
            return *this;
        }

        friend Entity& operator+= (Entity& e1, Entity& e2)
        {
            e1.m_x += e2.m_x;
            return e1;
        }

        5. +, - and ! are unary operators (e.g: -x)

    */


    Vector2 res4 = pos + (speed * powerup); //better to use operator overloading
    std::cout << res4.x << std::endl;

    res4--; 
    /*
    with unary operators
    1. declaring an overloading member function:
    Vector2& operator--(); (equivalent to Vector2& operator(Vector2* this))
    when performing "res4--"; the compiler will generate the following function call:
    operator--(&res4); //res4 is supplied implicitly 

    2. declaring an overlding normal/friend function:
    this solution is not optimal for this scenario because operator-- operates
    on the instance itself, thus a member function fits better as explained above in:
    overload with a Member/Friend/Normal function?
    Vector2 operator--(Vector2& vec); //we explicitly note the parameter
    when performing "res4--"; the compiler will generate the following function call:
    operator--(&res4); //res4 is supplied implicitly 

    Conclusion: in both cases, the supplied parameter is passed implicitly
    and it is the instance itself
    */

    //equivalent to res4.operator--() Vector2& operator--(&res4)

    //operator overload for cout
    bool res5 = res3 == res4;
    std::cout << res5 << std::endl; //equivalent to: std::cout.operator<<(res5), equivalent to: V

    bool res6 = pos != speed;
    std::cout << res6 << std::endl;
}

/*
General notes:
1. if two overloads are identical except that they differ in their return type,
they are still considered identical overloads! different return types are not sufficient
for the compiler to determine which method to be executed at run time.
Entity& operator++()
Entity operator++()
Are IDENTICAL even though the return types are different!!!!!
only different argument lists matter in function overloading.
This also applies to template functions.
This is why in order to differentiate between postfix and prefix operators,
the overload of postfix has a dummy int parameter:
Entity& operator++() //prefix operator
Entity operator++(int) //postfix operator
both are explained in 16b_precedence

2.

operator[][] overload is not possible, instead we can overload() operator
to achieve operations such as my_2d_array(2,3)  to get a specific index.

int& Matrix::operator () (int x, int y)
{
    return matrix[x][y];
}

3. operator overload for pod's (plain-old-data):
There is no int::operator+. Whether the compiler chooses to compile a + b directly to assembly (likely) 
or replace it with some internal function like int __add_ints(int, int) (unlikely) is an implementation detail.


4. Unary negation: "operator-": Note that you should not return *this. 
The unary - operator needs to create a brand new Vector value, not change the
thing it is applied to, therfore the overload of this operator should be free/friend


*/
