#include <iostream>

int main()
{
	/************Goto*/
	//causes spahgetti code
	int i{0};
try_again: //label statement
	std::cout << "still no 3\n";
	i++;

	if (i < 3)
		goto try_again;
	std::cout << "3\n";


/*
	{
	goto later;
	int a = 100;
later:
	printf("%d", a); // although the declaration of a is implicitly moved up to the top of the block (explained below)
	//the initialization doesn't therefore we get an error "skip the initialization of a", but the declaration IS
	//recognized, even though the compiler hasn't "touched" "int a = 100" statement before printf
	}

	note: in c99, declarations could be written everywhere as opposed to previous versions of
	c where declerations where only allowed at the top. In javascript, hoisting
	is peformed to variables declared with var (but not declared with "let"),
	meaning "x=5" can be used before "var x" statement,
	because the compiler would move var x to the top.
	THE MAIN POINT IS that the decleration is also moved in c99 and above,
	but the initialization doesn't!


//"almost" equivalent to :
	int b; 
	goto later1;
	b = 100; 
later1:
	printf("%d", b); //b will print  inderetminate value because b was assigned after the goto



*/

	/************Switch*/
	//if-else evaluates each if statements until reaching the true statement - O(N) time, N - num of statements, O(1) space.
	//switch uses jump table (array), jumps straight to the case, O(1) time, O(N) (?) space.
	//swtich statements can also be view as goto's (go to "case 1"...).
	int x{ 1 };
	switch (x)
	{
		int y{ 4 }; //decleration + definitition+ initialization is not allowed in switch scope, 
		//it seems that the compiler views y in the same manner in the example above, where a was declared
		//below the goto command, when it should have been appeared before it. the same case may be applied here,
		//y is declared inside the switch and the before the cases, (=between the goto and the label statement).
		//because y is initialized, the compiler cannot move y to the top of the block, as opposed to z.

		int z; // z declared (z identifier exists)+defined(given memory) is inside switch scope,
		//and is available to all cases, moved to the the top of the block
		
		case 1: // label statement - can contain only statements.
		//in c declerations are not considered statements , contrary to cpp where declerations are subset of statements
		//therefore declaration such as "int x" is not allowed in c, but allowed in cpp
			int u; //allowed in cpp, not in c. u is in the block of the switch and available to all (move to the top of the block)
			{
				int b{ 4343 }; //initialization is possible 
				std::cout << b << "\n";
			}
			std::cout << "1\n";
			break;
		case 5: //cases 5 and 2 will lead to the same case
		case 2:
			//int p{ 105 }; //error due to "jump over initialization"
			std::cout << "2\n";
			break;
		case 3:				 
			std::cout << "3\n"; //prints this
			z = 2;
			std::cout << z << "\n";
			u = 4;
			std::cout << u << "\n"; // was declared in case 1
		case 4:
			int w;
			std::cout << "4\n"; // also prints because "case 3" doesnt have a break or return, because of "fallthrough"
		default:				//evaluated when no "case" is chosen
			std::cout << "default\n";
	}

} 

/*
According to cppreference, C++ includes following types of statements:

expression statements;
compound statements;
selection statements;
iteration statements;
jump statements;
declaration statements;
try blocks;
atomic and synchronized blocks


While C considers following types of statements:
compound statements
expression statements
selection statements
iteration statements
jump statements
As you can notice, declarations are not considered statements in C, while it is not this case in C++.

*/