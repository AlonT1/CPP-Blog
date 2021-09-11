#include <iostream>

int main()
{
	int arr[] = { 20,30,40 }; //array is contiguous
	int* p = arr;
	int q = 0;
	q = *(p++);

	std::cout << *p << "hay" << std::endl;

	/*
	associativity int var = fun()
	 int var, fun are operands, = and () are operators
	 = has the lowest precendce
	precednce of () is higher than =, therefore -> int var = (fun())
	 if precednce of =  was higher than () -> int (var = fun)()
	associativity 10/5*2 -> lr (10/5)*2 correct -> rl 10/(5*2)
	associatvity can only help if there are two or more operators of same precedence - precedence wins!

	below are additional properties of the ++ operator (also applies to ---), nothing to do with precedence...
	in both the following examples, post and prefix have higher precedence than = , so they are perfromed first, assuming x =5


	************++prefix 
		the overload of prefix operator is: Entity& operator++():
		1. increments the value of the object. 
		2. return a reference to the result 
		(returning an lvalue ref of x itself (has memory) which we have just incremented)

		examples of ++x in statements:

			1. t=++x  (assume x=5) -> equivalent to : t.operator=( x.operator++() ):
				a. x.operator++() :x incremented and a reference to 6 is returned 
				b. t.operator=(x) the incremented x from (1) is assigned to t -> t=6 and x=6
				because ++prefix  has the 3rd highest precedence, while "=" is 16th precedence. 
				thus first and foremost x is incremented, a reference to the incremented x location
				is returned (basically, 
	
			2. arr[++x]	([] and ++prefix have the same precedence and left-to-right associativity):
			the statement is expanded to (according to 30a_reading_declarations, start with identifier)
			arr.operator[]( x.operator++() ):
				1. x.operator++() operates on x itself and thus no parameteres are taken in, and prefix operator++
				returns an lvalue ref to the incremented x.
				2. arr.operator[] ( x )  ->  executing subscript operator on arr, with the same x (the same lvalue)
				of x inceremtned in 1. operator returns an lvalue reference of the memory location of index x from ther array arr



	************postfix++
		1. create a copy of the object. 
		2. increment the original object (the object we work on, x itself)
		3. return the copy from 1 as a prvalue (the unincremented, original value is returned)

		the overload of postfix operator is: Entity operator++(int):
			1. postfix returns a copy (prvalue) of unincremnted x, as opposed to an lvalue reference as seen in ++prefix
			2. int is a dummy parameter, helps the compiler to identify between the identical
			function signatures of postfix and prefix (the different return types between
			posfix and prefix signatures are not enough for the compiler the determine which 
			function to run, therefore a dummy int is added to postfix

		examples of x++ in statements:
			1. t=x++ equivalent to : t.operator=( x.operator++(int) ):
				a. x.operator++(int) : performs copy of x that we operate on, increment original x,
				return the copied object -> t = 5, x = 6
				b. t.operator=(x) the unincremented copy of x
				postfix ++ has the 2 highest precedence, while = is 16. only :: operator has higher precedence.

			2. arr[x++] equivalent to: arr.operator[]( x.operator++(int) )
			the statement is expanded to (according to 30a_reading_declarations, start with identifier): 
				a. x.operator++(int): 1. copy of x is created, 2. x itself (on which we operator on) is incremented
				3. the unmodified copy from 1 is returned as a prvalue
				b. arr.operator[](x)  ->  executing subscript operator on arr, with int x parameter
				 which is the prvalue unmodified result returned from 1. operator returns an lvalue reference
				of the memory location of index x from ther array arr


	************Notes:
		1. both in "t=x++" and "arr[x++]"  arr and t are evaluated
		with the unmodified value of x (x itself (memory wise) is incremented but the x in the statement
		is an unmodified prvalue copy of x before the increment)
		x itself IS modified but this modification will show up in  the next
		statement in which x will appear. This is in complete opposite of prefix operator, which 
		when exectued, the increment is seen immediately by other statements such as arr[++x] where
		arr evaluates with the incremented x itself (prefix returns lvalue ref to the incremneted x)

		2. ++prefix is faster than postfix++ because prefix doesn't create extra copies

		3. postfix increment and decrement have higher precedence than
		prefix increment and decrement


	************ "*" operator vs "++" operators:
		postfix++ higher precedence than * and * is rl associative, postfix++  
		is lr associatve - but precdence wins! ->
		*ptr++ => *(ptr++) -> increment data(memory address) of ptr and derefence it

		++prefix equals precedence as * (both rl associativity) ->
		++*ptr => ++(*ptr) -> dereference value at ptr (memory adress) and increment it
		*++ptr => *(++ptr)

		
		note: postfix increment and decrement have higher precedence than
		prefix increment and decrement
		note: in any case use parenthesis and be explicit!
		how to remember? postfix has more steps in it, its heavier thus it has higher precedence
	*/

}

