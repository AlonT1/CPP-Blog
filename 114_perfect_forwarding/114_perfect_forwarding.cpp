#include <iostream>
#include <string>
#include <utility>

void func1(std::string&);

void* operator new (size_t size)
{
	std::cout << size << '\n';
	return malloc(size);
}

//https://en.cppreference.com/w/cpp/language/reference
//https://techmunching.com/universal-reference-and-perfect-forwarding/
//https://en.cppreference.com/w/cpp/language/reference#Forwarding_references
//https://en.cppreference.com/w/cpp/utility/forward

/*
perfect forwading -

when a function template "X" accepts an argument "p" via parameter "T&& p"
(forward(univsersal) reference) and performs a call to function Y" using Y(std::forward<T>(p))
p is said to be perfectly forwarded, which means two good things:

1. Deduced type = forwarded type (rvalue ref / lvalue ref):
universal ref parameter "T&& p" will "converge" to a type that depends on the value category of argument p
and the parameter with this converged type (either an lvalue ref or rvalue ref)
is promised to be forwarded (passed) "as-is" (same type) to another function
which should accept argument p either via universal ref / lvalue ref / rvalue ref parameter.

2. no redundant copies
because universal ref paramaeter "T&& p" can "converge" to either an lvalue ref
or rvalue ref, and this parameter will be forwarded to another function which accepts
either an lvalue ref /rvalue ref - no redundant copies will be made
The forwarding


Since perfect forwarding only works with universal (forward) references which
can be dedcued to either an lvalue ref or rvalue ref, I'll present both of these
scenarios:
*/



/************perfect forwarding with a universal reference that is deduced to lvalue ref**********/

/*
dispatcher1's universal ref "T&& str" parameter accepts "str" lvalue expression from the caller,
therefore T&& will converge to lvalue ref via the following steps (more info in 113b_universal_forward_reference):
1. Step 1 (T deduction): T&& str  =>  int& && str (T deduced to int& because "str" is an lvalue expression)
2. Step 2 (ref collapsing): std::string& str; (final type)  (according to the rule T& && => T&)

the call will finally look like this: dispatcher1<std::string&>(std::string&)
where "<std::string&>" means that the compiler deduced that "str" is an lvalue expression
and (std::string&) shows the final type that the universal ref T&& has converged to as shown above.

so now we have the following parameter in dispatcher1: std::string& str - analysis:
1. decl-specifier-seq (type specifier): std::string - the base type
2. init-declarator-seq (declarators): &str, where "str" is the identifier and "&"
is the modifier/compound type (because now str is reference built upon the underlying std::string).

std::forward<T>(str)  <-- here "str" is an lvalue expression, and T was deduced to std::string&
as explained above, so we have the folowing call: "std::forward<std::string&>(str)"

std::forward explained:
std::forward's job is to take in a reference to the expression we want to pass to another function
and ensure that the type of this expression (either an rvalue ref or lvalue ref), is the type
that will eventually be passed (forwarded) to the function.

std::forward has two overloads: the first takes in an lvalue ref and the 2nd an rvalue ref.
This coincides with the fact that universal references converge to either an lvalue ref or rvalue ref.

overload 1:
template<class T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept
{
  return static_cast<T&&>(t);
}

overload 2:
template<class T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept
{
  return static_cast<T&&>(t);
}


when the call "std::forward<std::string&>(str)" is met by the compiler it has to decide which
one of the two overloads to stencil out:

because the argument "str" is an lvalue expression,
and overload1's parameter is an lvalue ref "t", overload 1 will be chosen, as opposed
to overload 2 that houses an rvalue ref (can bind to primary categories xvalues or prvalues,
both are from the non-primary rvalue family category - invoked with statements succh as:
"std::forward<T>(std::move(x));").

next, after the overload is chosen the compiler will stencil out forward function like this:

std::string& (std::string& t) noexcept
{
	return static_cast<std::string&>(t)
}

explanation:

1. in principle "func(std::forward<T>(str))" is IDENTICAL to "func(std::static_cast<T&&>(str))"
in both cases we take the lvalue expression "str" and ensure that func will receive the same underlying type
of str which can be either lvalue ref or rvalue ref (instead of func(str) which passes an lvalue, losing the
underlying type). The reason for the existence of std::forward is because it is more explicit,
just like std::move(str) which is equivalent to std::static_cast<T&&>(str)

2. the parameter t: the type trait (template struct the transforms a type),
"std::remove_reference<T>" =>  "std:remove_reference<std::string&>"
equivalent to "std::string" (ref removed) and now this std::string is joined
with the "&" declarator - forming std::string&. b
the removal of the reference from T is crucial becuase we want to ensure that
the parameter of std::forward function is an lvalue ref (to us from copying the lvalue expression "str"),
therefore, we remove the ref from std::string + "add" it with &, forming an lvalue ref.

3. because T is std::string& (as seen in the the deduced call to std::forward),
all T&&'s are equivalent to: "std::string& &&" which collapse to std::string& which it
is the exact type that we want to preserve and pass to another function.

4. finally we cast "t" to std::string& - important because the expression "t" is lvalue expression,
and because we want to forward it as an lvalue ref(the same type that the universal ref converged to
when the argument came into the forward function)
we must cast it back into std::string& and return it as std::string&
*/


template <typename T>
void dispatcher1(T&& str)
{
	func1(std::forward<T>(str)); //str is lvalue expression
}


//func now recieves "str" as std::string&. it is our responsibilty to ensure that
//the function that expects a perfectly forwarded argument (func 1)  will have the same type
//(rvalue ref or lvalue ref) that the incoming arguemnt from std::forward provides.
void func1(std::string& str)
{
	std::cout << str;
}


int main()
{
	std::string str{ "abc" };
	dispatcher1(str); //equivalent to "dispatcher<std::string&>(str)" std::string& because "str" expression is an lvalue
}