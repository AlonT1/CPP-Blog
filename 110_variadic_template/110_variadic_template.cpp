#include <iostream>
#include <tuple>

//https://www.ibm.com/docs/en/i/7.3?topic=only-variadic-templates-c11
//https://eli.thegreenplace.net/2014/variadic-templates-in-c/

/*
tl;dr
variadic functions (which can accept infinite variables) are implemented in cpp in
the following ways:

1. C-style variadic functions: awkward, hard-to-remember syntax with type safety
problems and pitfalls

2. variadic template functions: THE PREFERRED way. implemented in a recursive manner, albeit
more optimized (read note at the bottom of the page) and always at compile-time as accustomed with templates,
requiring us to include a "base-case" function.
variadic template functions support fold expressions (cpp17) that relieve the need for recursion
and base function, allowing for more concise syntax when operating on parameter packs.

* both 1 and 2 utilize ellipsis (...) specifier, a parameter pack.

*/



//defines va_list, va_arg, 
//va_start, and va_end, which are macros that we need to use
//to access the parameters that are part of the ellipsis.
#include <cstdarg> //relevant to the pre-variadic example


/****************************C-style variadic functions
count parameter - denotes how many arguments we're about to pass
ellipsis (...) specifier - a parameter pack, allows us to pass a variable number of parameters. must be the last parameter.
*/

double sum(int count, ...)
{
	double sum{ 0 };
	va_list list; //macro programmed to accept a parameter list represented by ...

	// va_start is a macro that allowes us to initialize "list" with the parameters that come 
	//AFTER "count" parameter, for example (4,1,2,3,4), list is initialized with (1,2,3,4)
	//the 1st 4 represents count parameter, tells how many variables we're passing)
	va_start(list, count); //now list equals to "1".  (1,2,3,4) is 

	for (int arg{ 0 }; arg < count; ++arg)
	{
		sum += va_arg(list, int); //va_arg extracts "int" parameters from "list" one by one
		//and updates "list" with the next extracted value
	}

	//cleanup the va_list. if the va_end is not called before returning from
	//the function, the result is undefined(? although returning sum works even without va_end)
	//in the debugger va_end resets lists value
	va_end(list);
	return sum;
}
/*
problems with ellipsis and the technique above:
1. type safety - it is possible to send any type to the elipsis - while the method above works only with 1 type:
 the function above works well with calls such as sum(3,1,2,3) (all ints)
but with the call (3, 1, 2.4, 3) 2.4 is double - this is problematic since the macro
va_arg requires us to inform the type of the expressions represented by the ellipsis.
the argument 2.4  in the ellipsis with an "int" provided to va_arg, would cause the compiler only to read
the first 4 bytes (the size of the int), while 2.4 spans over 8 bytes, leaving the latter
4 bytes unread, thus we're getting an incorrect read.
Conclusion: no type checking exists with ellipsis - we have to trust the caller to provide only ints

2. it is possible to make mistake in the count parameter that denotes how many arguments
are passed to the ellipsis (ellipsis themselves don't store the number of argument contained in it)
other solutions:
	1. -1 sentinal value:
	the function call becomes sum(1, 2, 3, -1);
	the function  becomes "double sum(int first, ...)" instead of "double sum(int count, ...)".
	because "first" is skipped by va_start, we must save it in a variable.
	and then in the iterating for loop we stop the iteration when we arg==-1.
	problems: -1 might be value we want to use, and the caller might forget to provide -1 as an argument.

	2. decoder string:
	the function call becomes sum("iiddi", 1, 2, 3.5, 4.5, 5):
	this string instructs the types i (integer) d (double) and enables
	mutliple types:

			va_start(list, decoder);
			int count = 0;
			while (true)
			{
				char codetype{ decoder[count] };
				switch (codetype)
				{
				default:
				case '\0': 		// Cleanup the va_list when we're done.
					va_end(list);
					return sum / count;
				case 'i':
					sum += va_arg(list, int);
					count++;
					break;
				case 'd':
					sum += va_arg(list, double);
					count++;
					break;
				}
			}

Conclusion:
if we use ellipsis specifier with the technique above - better to not don't mix types.
count parameter is safer the sentinal or decoder parameter.

*/



/****************************variadic function templates since cpp 11
A variadic template is a class or function template that supports an arbitrary number of arguments using templates.
variadic templates provide strong type checking, and are much safer then the technique
above (can easily receive multiple arguments with different types, like any other template function).

The following examples show a recursive and iterative way of using a variadic function:*/

/***************************************** recursive example:
template types and arguments explained:
variadic functions are implemented as recursive functions and they MUST have 2 template types to work:
1. "typename T" - a template parameter type. will represent a function parameter called "current_iterated"
2. "typename... Values" - a template parameter type with elipsis (template pack parameter type) :
	a. (...) a specifier that allows us to pass a variable number of parameters.
	b. Values is the template type parameter pack that holds multiple variables
	should be read: "..." specifier specifies that the template type "Values" serves as a parameter pack that
	can hold multiple variables, or in short:
	values is of template type parameter pack

	Note: "typename... Values" template parameter pack can only exist in the context
	of templates, thats why it is called TEMPLATE type parameter pack.
	They are the key ingredient in what makes a template class/function
	variadic.

2. "const Values&... values" - values is a function parameter of a template type parameter pack,
utilizing the template parameter pack(... on the left of the identifier):
values is an identifier of a function parameter of a template type parameter pack.

3.  However, in "summaraizer(values...)" (... on the right of the identifier) values... is called a parameter pack
	expansion (unpack) and "values" is called "the pattern of the expansion". The expansion causes the
	name of the parameter pack ("values" a.k.a the pattern) to be replaced by each of the elements from
	the pack in the order provided by the caller.

the variadic function "auto summarizer(T& current_iterated, const Values&... pack)" is a recursive function:
In practice, each call of summarizer(values...) extracts the next iterated value from the pack -
The extracted value is assigned to current_iterated parameter - the current iterated value from pack.
and then we continue to call the templated version "summarizer(T current_iterated, const Values&... pack)"
with the remaining pack, where current_iterated is initialized with the current extracted value. This goes on
until the pack is empty and then the call will be equivalent to summarizer(), which will call the
base case summarizer() above



/*************the 1st summarizer() is the base case, the 2nd summarizer() is the varaidaic function**********/
//a recursive sum via variadic template:

auto summarizer() //base case function
{
	return 0;
}

// to support variadic templates, we must have these 2 parameters.
// template parameter pack must be the last parameter
template <typename T, typename... Values> //"typename... Values" == "typename ...Values" == "typename ... Values"
auto summarizer(const T& current_iterated, const Values&... values) //return auto type deduction (read 108_auto_if_constexpr)
{
	return current_iterated + summarizer(values...);
}




/*************another variadic template function example:*******
shows pack expansion in pairs:*/

template<typename T>
bool pair_comparer(T a, T b)
{
	return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args)
{
	return a == b && pair_comparer(args...);
}

/*
calling "pair_comparer(1, 1, 2, 2, 4, 4)" will cause parameter pack (args...) 
to be expanded in pairs, (1,1) will be assigned to (a,b) parameters, in the next
iteration (2,2) will be assigned to (a,b) and so forth.
Notes:
1. pair_comparer calls parameters must be even, because the expansion extracts
values into a, b (in pairs) - error otherwise. Albeit, we can provide a template function to cover
a case of uneven parameters:

template<typename T> bool
pair_comparer(T a) { return false; }

that accepts a single template type - this will cover calls such as
pair_comparer(1, 2, 3): (1,2) are extracted into (a,b), but the argument 3
will be provided to the function above.
*/

/*
performance of variadic templates:

1. templates are created at compile - time, so there is no runtime performance cost.
2. and even during compile-time - NO RECURSION OCCURS: 
even though variadic template functions seem to use recursive calls - no recursion 
occurs. What happens is that a sequence of 
*/


/****************************Iterating over an unpacked template parameter pack*******/
template<typename... Values> // Values is a template type parameter pack
void iterator_loop(Values... values) // values is of type Values template type parameter pack
{
	for (auto x : {values... }) // values... means unpacking parameter pack. {} are mandatory.
		std::cout << x << '\n';
}



/****************************variadic class templates since cpp 11*/

//declaration + definition of a variadic template class with T as a parameter pack
template <typename... T> // T is a template type that is also a parameter pack (as specified by "...")
class Entity
{
};

Entity<int, float, char> t; // class template instantiated with multiple template argument


/*
fold expression (cpp 17) - improvement for variadic functions -  
Reduces (folds) a parameter pack over a binary operator in the context of a template-variadic function.
Simply put, we can easily perform binary operations on the whole ellipsis pack with a single statement.

syntax:
( pack op ... )	(1)  e.g: (pack + ...)  ... on the right means right to left associativity (explained below)
( ... op pack )	(2)
( pack op ... op init )	(3)
( init op ... op pack )	(4) init is a default value, e.g: (0 + ... + pack) (explained below)
Note about the syntax:
for example "Values const&... pack": pack is an identifier to an expression of type Values
and ... is a specifier specifying that pack represents multiple variables, therefore the syntax is a bit weird:
the specifier seems seperated from the identifier : pack + ... - well, guess that's the convention.

op - any of the following 32 binary operators:
+ - * / % ^ & | = < > << >> += -= *= /= %= ^= &= |= <<= >>= == !=
<= >= && || , .* ->*. In a binary fold, both ops must be the same.

pack - (values...)  An ellipsis (...) is used to represent a variable number of parameters to a function

init - a default value - explained below
*/


/*
sum_fold_rl(1, 2, 3, 4) expanded at COMPILE-TIME (read 53_templates) to:

int sum_fold(int const& value1, int const& value2, int const& value3, int const& value4)
{
	return (value 1 + (value2 + (value3 + value4)));
}

the associativity is on the same side as the dot dot dot - right to left associativity
associativity is important for example:  rl associativity(1-(2-(3-4))) = -2
lr associativity(((1-2)-3)-4) = -8

Note:
summarizer(1.2550454, 2, 3, 4, 5, 6, 7, 8); actually produces predictible results (36.2550454)
although types are different and the template type Values must represent a single type.
my assessment is that the values go through promotion (see 69_casting), in short, 
the ints are promoted to the highest priority floating point type, double in this case
because 1.2550454 is a double.
*/

template<typename... Values>
auto sum_fold_rl(const Values&... values) //auto type deduction -  when we don't know the returned type
{
	return (values + ...);
}


/*
sum_fold(1, 2, 3, 4) expanded to
int sum_fold_lr(int const& value1, int const& value2, int const& value3, int const& value4)
{
	return (((value 1 + value2) + value3) + value4);
}

the associativity is on the same side as the dot dot dot - left to right associativity
*/
template<typename... Values>
auto sum_fold_lr(Values const&... values)
{
	return (... + values);
}


/*
0 is an init value, an expression that does not contain an unexpanded parameter pack, essentially
default value 0 is useful when we call sum_fold_default() (with no values),
otherwise if no default value is supplied, and we call sum_fold_default() with no arguments
an error will occur since there is nothing to sum (fold of empty expansion over operator+).
the default_value 0 solves this.
*/
template<typename... Values>
auto sum_fold_default(Values const&... values)
{
	return (values + ... + 0);
}




int main()
{
	std::cout << summarizer(1, 2, 3);
	sum_fold_lr(1, 2, 3, 4);
	iterator_loop(4, 2, 4, 5, 6);
}

//https://www.fluentcpp.com/2021/03/12/cpp-fold-expressions/
//https://www.codingame.com/playgrounds/2205/7-features-of-c17-that-will-simplify-your-code/fold-expressions
//https://docs.microsoft.com/en-us/cpp/cpp/ellipses-and-variadic-templates?view=msvc-160
//https://crascit.com/2015/03/21/practical-uses-for-variadic-templates/
/*
Most implementations that incorporate variadic template functions use recursion of some form,
but it's slightly different from traditional recursion. Traditional recursion involves a 
function calling itself by using the same signature. (It may be overloaded or templated,
but the same signature is chosen each time.) Variadic recursion involves calling a variadic function
template by using differing (almost always decreasing) numbers of arguments, and thereby stamping out a
different signature every time. A "base case" is still required, but the nature of the recursion is different.
*/