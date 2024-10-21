#include <iostream>
#include <string> // needed to print std::string. the operator (?) overload that allows to push (<<) strings into cout is in this file

//All string literals have an implicit null - terminator, irrespective of the content of the string.
//So, the string literal "abc\0" contains the implicit null-terminator, in addition to the explicit one.

/*
std::string is heap allocated.
string - a group (Array) of characters
char is useful for pointer arithmetic and memory allocation 1kb = 1024 chars (1 byte per char)
UTF8 (8 bits) -> 2^8 characters (256), there are way more than 256 in all languages combined
therefore -> UTF16 - 2^16 possibilities - primitive data byte char
in cpp without libraries char is 1 byte - for english.

*/

void print_string_worse(std::string string) //passing a copy! copy is made on the stack - string copy is slow. make sure to pass by const ref
{
    string += "h"; //won't change original string
    std::cout << string << std::endl;
}


void print_string_better(const std::string& string) //const - string is locked, refrence - we modify the original data in the memory location
{
    //string += "h";  // not possible
    std::cout << string << std::endl;
}


int main()
{
    /**the two following examples are possible in c where string literals are not required to use const like in cpp*/
    //char* x = "hello";  // allowed in cpp but raises warning! 
    //x="shalom";
    /********************************************************/
    //"string"; //< -a string literal(const char[] array in c) null termination implicitly added ('\0' or 0 it's ascii equivalent)
    //null termination signifies where the string is terminated, and is added implicitly -> "string" == "string/0".
    //adding it explicitly will cause double null termination
    //functions such that measure the length of a string will print that the length of "string" is 6 (characters), but
    //there are actually 7

    /*
     string literals are immutable(c/cpp) and constant(cpp).	

     Immutable (c/cpp) - both in c & cpp strings are immutable - the content at the memory cannot be modified!
     (as opposed to const which is all about access control during development)
     modifying an immutable object causes: 
     1. new memory allocation 
     2. the old content ("hello") is copied to the new allocated memory with the changes ("hellw")
     3. pointer assigned to new block

     Constant (only cpp)- a cv-type qualifier (const or volatile) (c don't have a const) const-ness is used for
     access control which can be modified via mutable keyword: const-ness of functions can be overriden by adding "mutable" keyword
     to members, allowing const functions to modify class members, in addition we can perform a const cast to remove constness,
     as opposed to immutability, where there isn't a way to change the data at any stage of life. Thus the concept of immutability
     can be simulated by a const - but it isn't guarateed. the concept of immutability is seen in cpp through string literals, as shown above.
     in the example above x is assigned with new value  this is technically possible
     (only in c where const isn't required) but considered as undefined behaviour - the c standard doesn't define 
     what should happen in this case. some compilers may produce valid code for this.. not recommended.
     msvc (vs compiler) allows this (?) clang doesn't.
     string literals are stored in a read only memory stored in const segment in assembly code. (they are stored in the const segment, 
     not because of the "const" keyword, but because strings are designed to be stored there). "const" is concept for the compiler,
     it doesn't have any effect on the final binary code.
     The characters are embedded into the binary itself - not allowed to edit.
     reassigning x MAY WORK IN DEBUG - NOT IN RELEASE!!
     
     the correct cpp style of assigning a string literal to a variable:*/
     const char* str = "string"; //str stores the address of the first char in the string ('s')

     /*prints the whole string (probably std::cout operator<< is overloaded in such way that
     char* argument prints the all string up until '\0'*/
     std::cout << str; 

     std::cout << *str; // prints 's', directly accessing char 's'
     /*where strings are stored?
     char s[] on the stack.
     char* s - read only data segment in memory (.rodata), in "text" section of memory.
     both are stored in const segment of assembly language this version is immutable 
     (stored in read-only memory), any change to this string actually starts steps 1,2,3
     the notes of immutable


     why string literals are constant in cpp?
     1. allows merging, reuse, of string literals - if one program uses the same string literal in several places, the compiler can merge them, saving memory
     char *foo = "long string"; char *bar = "string";
     In a case like this, it's possible for bar to be foo+5 (if I'd counted correctly).
    */

    const char* test = "Shalo\0m"; //prints until shalo. this how it looks in memory  Shalo.m. - . is for \0. once for manual \0 and one for the implicit trailing
    // \0 is considered one character (\ escaping character)
 
    //strings can exist without a null-terminated character, if we copy using memcpy the 5 out 7 characters of string into an 5 char array,
    //and ignore the null terminated charater, we can string print it using a for loop with (i=0;i<5;i++)
    std::cout << test << std::endl;
    std::cout << strlen(test)<< std::endl; //5 ("shalo") since it works until closest null terminator
    //"shalom" -> there are 7 characters here - the 7th is the implicit null termination character!

    const char* s = "hello"; //c style way to define string (with const, c doens't have const). char pointer
    //string begins at the memory address and ends in \0 (or 0, same in ascii) null termination signifies end of string(added implicitly).this is how std::cout knows when to stop
    //this is not heap allocated - if don't use "new" we don't use "delete".  char - 1 byte per character (8 bits - utf8)

    const char* s2 = u8"hello"; //c style way to define string (with const, c doens't have const). char pointer. u8 enforce utf8 (implicit?)

    char name2[4] = { 'A','L','o',0 }; // as opposed to the string literal (const char*) "Alo" 
    //that adds the null termination character implicitly,
    //char arrays require adding a null termination  (0 or '\0') expliciltly. without it printing name2 yields jibbrish after 'o
    //(the end of string is not known to the compiler)

    char str4[3] = "ma"; // we have to accomodate an extra char for '\0' thus str[3] and not str[2]
    char str5[] = "ma"; // sizeof(str5) prints 3, extra character for null terminated charactor
    //L signifies that literal is made up of wide characters 2 bytes per character - up to the compiler 2 windows 4 mac
    // regular char is 1 byte (up to 256 characters) , for ASCII.
    // wchar_t is used for Unicode (thousands of characters)
    const wchar_t* name5 = L"makore";

    //cpp11 introduced
    const char16_t* name10 = u"makore"; // 2 byte per character (16 bits (1byte=bits)
    const char32_t* name11 = U"makore"; // 4 byte per character (32 bits)

    std::cout << name2 << std::endl;
    //c functions
    strlen(name2);
    //strcpy

    //c++
    //template specialization of string class - array of chars with additional functions
    std::string name3 = "shalom";
    std::cout << name3 << std::endl;

    //std::string name4 = "shalom" + "hello";  //not possible - adding two const char arrays.
    //"text" <- in c this is a const char array, they can't be added!!! not a string!
    //alternatives:
    name3 += " hello"; //possible! += is overloaded in class string to make this possible

    std::string name4 = std::string("shalom") + "hello!"; //
    std::cout << name4 << std::endl;

    
    /*
    int is the basic signed integer type, and is guaranteed to be at least 16 bits wide[1].
    std::size_t  the size of the WORD depends on the os/cpu
    */

    bool contains = name4.find("om") != std::string::npos;
    //if find doesn't finds the substring it returns std::string::npos (a constant value: -1 of type unsigned size_t - the size of 
    //of a Word, depends on the architecture) because it is unsigned -1 in two's complement means the largest size_t possible)
    //SO WE BASICALLY SAY if the returned value of find is different than npos (not found), it means that contains is true
    std::cout << "ew" << contains << std::endl;

    //cpp14
    // adding s after " - operator function that returns a set of string (implemented in string_literals), allows + concat
    using namespace std::string_literals;
    std::string name13 = "Cherno"s + "hello";
    std::u16string name14 = u"Cherno"s + u"hello"; 
    std::u32string name15 = U"Cherno"s + U"hello";

    //new lines 

    const char* test3 = "Line1\n"
        "line2\n"
        "line3\n";

    const char* test4 = R"(Line1
Line2
Line3
Line4)"; //R means to ignore escape characters (multiline strings)

    std::cout << test3 << std::endl;

    std::string name16 = "hello" + std::string("bye"); // string concat. "hello" + "bye" yields error ("bye" is const char*)
    std::cout << name16 << std::endl;


    /* the following for loop on a std::string in reverse is problematic*/
    std::string string = "hello";
   /* for (size_t length{ string.length() - 1 }; length >= 0; --length)
        std::cout << string[length];*/

    /* the problem arises because string.length() returns a size_t (unsigned __int64) and
    at the last loop, when length==0 and we print string[0], the final decrement "--length"
    occurs - and because length is unsigned an overflow occurs and the length wraps around to its
    max positive value. this causes the final check "length >=0" to fail and hangs us in an
    indefinite loop.

    solution:
    for(size_t length { string.length() }; length > 0 ;)
        std::cout << string[--length]; // decrements to 0, prints, and then ends in "length > 0" check

    alternative - simply cast to an int:
    for (int length = static_cast<int>(string.size()) ; length >= 0; --length )
        std::cout << string[length];
    */

}
