#include <iostream>

/*tl;dr
there are 2 approaches for having a template function inside a template class:
assuming class C and foo is a friend of C:

approach 1:
each specialization C<x> is friend with all templated versions of foo

approach 2 (the best):
each specialization class<x> is friends exclusively with foo<x>
*/



/*******************************Approach 1*******************************/

template <typename T> 
class A
{
private:
    T m_a;
public:
    A(const T& a) : m_a{ a } {} //inside the template body A is equivalent to A<T>

    //this states that each specialization of A is friends with ALL(!) 
    //templated versions (specializations) of foo.
    template <typename U>
    friend void foo();
};

template <typename U>
void foo()
{
    std::cout << "foo\n";
}



A<int> a1(5);


template <>
void foo<double>()
{
    std::cout << a1.m_a << '\n';
}

/*******************************Approach 2*******************************/

/* this forward declaration is needed because we use goo in the body of class B
before it is actually defined. alterntivaly, we can simply put the definition 
of goo, right here and save us the forward declaration.*/

template <typename T>
void goo(const T& t);

template <typename T>
class B
{
private:
    T m_a;
public:
    B(const T& a) : m_a{ a } {}

    /* here we tell the compiler that each speicalization B<x>, will be friended exclusively
    with goo<x>, as opposed to approach1, where everybody is a friend with everyone.
    note that that we use <T> after goo (sometimes written as <>), which means
    that goo is a friend only with the same specific specialization of B,  as opposed to
    approach1 which stated that A is friends with ALL template versions of foo.*/
    friend void goo<T>(const T& t);
};

template <typename T>
void goo(const T& t)
{
    std::cout << "goo\n";
}



int main()
{   
    /***************************Approach 1************************************
    each class specialization is friends with all template specialization of the function

    Reminder: friend functions are able to access private members of a class that
    they are declared friends with, through an instance of that class.

    In this approach we specialize A<double>, and manually specialize foo<double>:
    this means that when the code for A<double> is generated, the compiler won't
    generate code for foo<double> because we've done it manually.
    since foo is a friend of A, foo<double> should have exclusive access to private
    members of A<double>, and only A<double>!

    But the example above contradicts this:
    we specialize "A<int> a1(5);" and when invoke foo<double>, it is
    able to access the private member m_a of a1 which is of type A<int>!
    so we have foo<double> that is able to access private members of A<int>, when
    both of them should not have any connections. A<int> should be friends with
    foo<int>, and A<double> friends with foo<double>.

    the compiled code looks something like:

    template<>
    class A<int> // same body for class A<double>
    {
        private:
            int m_a

        template <>
        void foo<int>()
        {
            std::cout << "foo\n";
        }
  
        template <>
        void foo<double>()
        {
            std::cout << a1.m_a << '\n';
        }
    }

    since foo<double> is a friend of A<int>, foo<double> has access to private
    members of A<int>

    Why this happens?
    in this approach if we instantiate A<double>, A<int>, A<char>, then
    foo<double>, foo<int>, foo<char> all become friends with each and one of
    A<double>, A<int>, A<char>. for example: foo<double>, foo<int>, foo<char> are
    all friends with A<int>, meaning that foo<double> has direct access to
    private members of A<int>, when it should have access only to private members
    of A<double>

    */
    A<double> a2(5.5);
    foo<double>();


    /***************************Approach 2***********************************
    each specialization class<x> is friends exclusively with func<x>

    each specialization of B<x> will be friend exclusively with the
    specialization of goo<x>, as opposed to approach1, where everybody
    is a friend with everyone. See class B for further info.
    
    
    
    */
    B<int> t(5);
    goo<int>(5); //equivalent to goo(5) due to type deduction
    B<double> w(5.5);
    goo<double>(5.5); //equivalent to double(5) due to type deduction

    /*
    class B is compiled into the following code after the statements above:

    class B<int>
    {
        .......
        template<>
        friend void goo<int>(const int& t)
        {
            std::operator<<(std::cout, "goo\n");
        }
    };

    class B<double>
    {
        .......
        template<>
        friend void goo<double>(const double& t)
        {
            std::operator<<(std::cout, "goo\n");
        }
    };

    B<int> is friends only with goo<int> and B<double> is friends only with goo<double>
    
    */

}