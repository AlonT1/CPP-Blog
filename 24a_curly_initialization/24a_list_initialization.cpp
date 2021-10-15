#include <iostream>
/*
list initialization (performed via {}) warns against narrowing (as opposed to direct initialization -
performed via (), which doesn't warn against narrowing). for example:

	1. char can be assigned to an int, but int cannot be assigned to a char,
	since char may not be capapble of holding the value of the int(overflow)
	int a = { 'a' }; allowed   char a = { 128 } not allowed overflow -> this because char a is
	implicit unsigned char meaning the msb is denotes + / -
	since a char is 8 bits, the 8th bit is for the sign, and the rest of the 7 bits allow
	for a number ranging from 0 - 127, with the 8th bit
	we gain - 127 to 127 number range. this is why char a = { 127 } is allowed!

	2. float(4 bytes) can be assigned to a double(8 bytes), but not the opposite(causes overflow), same principle as in(1)

	3. floating point cannot be converted to an int - > int a = { 4.2f }; error!
	Entity e2(3.8, 4) is ok but Entity1 e2 { 3.8, 4 }; notifies narrowing of of a float(3.8) to an int - nice!

*/

int main()
{
	int x2 = 3.4; // causes narrowing - x2 becomes 3
	//int x3{ 3.4 }; // error -  this one notifies an error of narrowing!! (conversion of floats to int,warns against truncation)
	std::cout << x2 << std::endl;
}
