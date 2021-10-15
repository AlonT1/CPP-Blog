/*
//other wise, allocating and not releasing a resource such as a file is a resource leak, just like a memory leak
//(e.g: "new"ing an array inside a function without performing a "delete" on the pointer that holds the array(resource)
//before leaving the scope of the function).

https://stackoverflow.com/questions/33874548/c-what-is-the-need-of-both-buffer-and-stream
https://www.codeproject.com/Questions/342008/how-the-cout-statement-works-in-cplusplus-and-how
https://superuser.com/questions/86999/why-cant-i-name-a-folder-or-file-con-in-windows
https://stackoverflow.com/questions/10518608/buffered-and-unbuffered-stream

Streams connect between our program and another entity (file, keyboard, display...).
the definition of streams is a sequence of bytes accessible sequentially, but in cpp
context streams mean a full mechanism that handle input and output between our program and an entity.
they're abstractions, we don't know EXACTLY how they work.
Some streams use buffers - the buffer (also called std::streambuf or "input sequence")
is managed by the stream. the stream wraps around the streambuf.
Between the stream and stream buffer a process of serialization / deserialization occurs:
the stream performs r/w operations on the stream buffer that holds raw bytes.
when writing (serializing), the stream converts integers, strings, etc. to raw bytes 
(possibly with additional info about the types?) and sends them to the buffer
when reading (deserializing), the stream reads the raw bytes from the buffer converts them to the actual
types they are meant to represent.
https://stackoverflow.com/questions/8116541/what-exactly-is-streambuf-how-do-i-use-it

i/o stream objects std::basic_istream (or simply istream - cin is object of class istream) and std::basic_ostream
(or simply ostream, cout is object of class ostream) all implement a streambuf (std::basic_streambuf)
https://en.cppreference.com/w/cpp/io/basic_streambuf
cout and cin global objects control output and input to the stream buffer (i'm assuming
both stream objects cout and cin have different stream buffers. 

***Serialization: turning an object in memory (class, string, int...) into a stream of bytes that can be stored on disk as a file
an object is stored in memory in pure bytes, for example: 0x61 0x62. the compiler knows that this are two integers,
but as pure bytes they can be chars, doubles, etc. therefore during serialization into a file the process ensures
to encode those bytes with additional information (for example the type of the objects).
Deserialization: the exact opposite - taking a serialized file and turning it into an object in memory, compatible
to be manipulated by the compiler
https://www.tutorialspoint.com/java/java_serialization.htm


Why a buffer is needed?
	Working with files is expensive performance wise. Therefore some streams have a buffer
	(temp memory on ram - faster then hard disk), where the streams can copy the file to the buffer and perform
	r/w operations on it there, and when all the changes are done, copy the changes back to the file (flushing the buffer).
	https://stackoverflow.com/questions/8116541/what-exactly-is-streambuf-how-do-i-use-it):


	diagram: Program<-----STREAM Mechanism(optionally buffered)------>Entity

	If there was no buffer, the program would write (get flushed) byte-by-byte to the file itself - this is cpu
	heavy but the information reaches the destination / from the destination fast without a buffer
	as mediator.
	the buffer decouples the program and the file - making r/w operations everything faster.
	"instead of copying character 1 by 1, we memorize all sentence and copy it to our brain
	(the buffer), the writing down is flushing."
	we can make cout unbuffered with the following statement:
	std::cout.setf(std::ios::unitbuf);


	example: taken from https://stackoverflow.com/questions/10518608/buffered-and-unbuffered-stream )
	assuming that std::cout is connected to the terminal (console)
	(cout and cin CAN be redireceted to files as described here
	https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files):

	std::cout << 1 << "hello" << ' ' << "world" << '\n';
	a buffereless cout would cause 4 writes to a file named "con" (CONsole) used for input/output to the console.
	Instead, std::cout is an object of ostream (buffered) class - so we write
	to the buffer of the stream very fast and from there the buffer can be flushed.
	meaning it will be emptied out to the console, and displayed there.
	The buffer can be flushed in the following scenarios:

	1. the buffer is full:
	This is considered optimal - the buffer can be imagined as a truck carrying
	load. it is more efficient to maximally utilize the carrying capcity of the truck (buffer)
	and make a single delivery (flush) to the destination (console/file)
	then to load up part-by-part (byte-by-byte) and make a delivery (flush) per part (byte).

	2. use std::flush manipulator manually (e.g std::cout << "hay" << std::flush;)
	std::flush flushes the whole buffer. This is performance-heavy.

	3. we use std::endl (adds a newline char '\n' and std::flush). better to use only '\n'
	to create a new line

	4. any input from std::cin, output to std::cerr (character error stream), or program termination forces all
	standarad streams to be flushed, including cout (a call to std::cout.flush(),
	(specifically, when the global instance of std::ios_base::Init is destroyed
	https://stackoverflow.com/questions/25668601/structure-of-cout-buffer-and-endl-in-c).
	for example:
	std::cout << "hay";
	std::cin >> x;  //the buffer of cout is flushed here to the console! (because we've used cin)
	https://en.cppreference.com/w/cpp/io/manip/flush


	5.  a new line character '\n' also causes std::cout buffer to flush:
	(according to https://stackoverflow.com/questions/42430701/does-new-line-character-also-flush-the-buffer):
	The 8 cpp stream objects (cin, cout, cerr, clog, wcin, wcout, wcerr, wclog)
	are synchronized with c's streams (stdin, stdout, stderr).
	https://en.cppreference.com/w/cpp/io/ios_base/sync_with_stdio
	Synchronization means that alternating between c and cpp streams produces consisted results, e.g:

	std::cout << "he";
	fprintf(stdout, "ll"); // printing to stdout (default file descriptor of the operating system)
	std::cout << "o";

	prints hello as expected.

	The problem is that the default buffering mode of stdout is _IOLBF (when stdout is connected
	to interactive stream, e.g: console). _IOLBF means line buffering - 
	The buffer is flushed when a new-line character ('\n') is written, when the buffer is full, or when input (std::cin) is requested
	(https://www.ibm.com/docs/en/i/7.3?topic=functions-setvbuf-control-buffering).
	Because std::cout is synchronized with the line-buffered stdout, this means that a newline charater inserted into std::cout
	or stdout WILL ALSO CAUSE THE BUFFER associated with both of them TO FLUSH.
	("The object cout controls output to a stream buffer associated with the object stdout." as stated in the standard:
	https://stackoverflow.com/questions/26975876/is-stdcout-buffered).
	Note that non-interactive streams (e.g: file streams) have a buffering mode of _IOFBF (full buffering mode).
	cppreference goes even further by stating this means that synchronized cpp streams are in practice unbuffered
	- they are line-buffered - but this causes each I/O operation on the stream to be immediately flushed and repetitive
	flushing is costly (https://en.cppreference.com/w/cpp/io/ios_base/sync_with_stdio).

	Note the cin also flushes the buffer when we enter a newline character (pressing enter).
	(we type into the buffer and when we press enter its content is dumped (inserted to some variable, for example).

	The advatange of this synchronization is that we can freely mix C++ and C I/O with predictible results.
	The minus is that line-buffering is also applied to cpp streams WHICH CAN BE FULLY BUFFERED
	(that is a newline character won't cause the buffer to flush). in addition,
	line buffering would require inspection of each character by the stream buffer which effectively inhibits bulk operations
	 on characters and thereby causing a substantial slow-down.


	How to make cpp streams fully buffered (and not just line bufferred):
	when the program always uses cpp streams and doesn't use c streams we can utilize this fact
	and disable synchronization with c streams via:

	std::ios_base::sync_with_stdio(false);

	a good implementation of this statement will disable line-buffering, enable full-buffering (_IOFBF buffering mode)
	for the streams (including cout) and result in substantial speed-up:
	https://www.modernescpp.com/index.php/c-core-guidelines-improved-performance-with-iostreams finds
	a performance improvement of about 70% in the non-synchronised variant when using GCC.

	we can apply the same to c streams via:
	std::setvbuf(stdout, nullptr, _IOFBF, BUFSIZ).
	https://stackoverflow.com/questions/796865/can-i-stop-stdcout-flushing-on-n/796894#796894


But what if we want to perform a single read of the file, or getting an input from hardware
without memorizing it, do we need a buffer? no -we can use unbuffered streams which are
faster because they don't temporarily store data in a buffer for possible r/w operations.


The following streams are buffered - data is streamed from the program into a data consumer
or the data is streamed from a data producer into the program:

	Input streams ("in" to our program) -
	diagram: Program<-----STREAM Mechanism(optionally buffered)<------Data Producer

	data producer, such as a keyboard, a file, or a network
	inputs data into our program via stream. istream class (also called basic_istream -
	for basic input stream operations). is the primary class used when dealing with input streams. With input streams,
	the extraction operator (>>) is used to remove values from the stream (the buffer). 
	istream is read only, we read data from the data producer for our usage.
	cin is object of type istream.


	Output streams ("out" from our program)-
	diagram: Program----->STREAM Mechanism(optionally buffered)------>Data Consumer

	used to stream output from a particular data consumer, such as a monitor, a file, or a printer
	into a buffer. The ostream (also called basic_ostream - for basic output stream operations)
	class is the primary class used when dealing with output streams. With output streams, 
	the insertion operator (<<) is used to put values in the stream. ostream is write only,
	we write to it so the data can be used by a data consumer.
	cout is object of type ostream.

	The iostream class can handle both input and output, allowing bidirectional I/O.

Standard stream is a pre-connected stream provided to a computer program by its environment. the following
are examples of standarad streams:

	1. std::cin object is an object of type (inherits from) istream and is preconnected to the keybaord,
	therefore when we type on the keyboard (data producer) we dump data into the iostream and this data is extractable via cin
	which is the interface through which we interact with iostream.
	we can only read from cin, thus we can only extract from it >> (extraction operator).


	2. std::cout class object to the standrad output (monitor), cout is an object of class (inherits from) ostream,
	and is connected to the monitor. we can only write to it, thus we can only insert to it with << (insertion operator).
	WE PRODUCE DATA AND THEN PROVIDE IT TO A DATA CONSUMER via cout which in turn dumps the data to the ostream 
	and displays the dumped data on the monitor.

	both cin and cout can be rerouted to interact with another resource.

Observation - with istream we can read data and then: 1.use it 2. output it to a data consumer
			  with ostream we can only output data to a data consumer
*/

#include <iostream>
#include <iomanip> //setw
#include <string> //std::string and std::getline that accepts std::string
#include <sstream> //for string streams 
#include <algorithm>
#include <string_view>
#include <map>
#include <fstream> // file streams


//prints the state flags of a stream (can be string streams, i/ostreams),
//they all inherit from ios (which inherits from ios_base) 
void print_state(const std::ios& stream)
{
	std::cout << " good()=" << stream.good();
	std::cout << " eof()=" << stream.eof();
	std::cout << " fail()=" << stream.fail();
	std::cout << " bad()=" << stream.bad();
}


bool is_valid_name(std::string_view name)
{
	//character or space ("John Smith")
	return std::all_of(name.cbegin(), name.cend(), [](char c)
		{ return std::isalpha(c) || std::isspace(c);});
}

bool is_valid_telephone(std::string_view telephone_number, std::string_view pattern)
{
	if (telephone_number.length() != pattern.length())
		return false;

	return std::equal(telephone_number.begin(), telephone_number.end(),
		pattern.begin(), pattern.end(), [](char tel_char, char pattern_char) -> bool
		{
			if (pattern_char != '#' && pattern_char == tel_char)
				return true;
			else
				return std::isdigit(tel_char);
		});
}

int main()
{
#if 0
	char buf[10];

	//there are two ways for manipulating streams: flags and manipulators:
	//1. manipulators are inserted or extracted via the corresponding operators
	//setw - MANIPULATOR - an object that is used to modify a stream when applied with << or >>
	//setw limits the number of characteres read in from a stream.
	//endl is also a maniplator - prints newline (eol - end of line) character and flushes any buffered output.
	//Manipulators are smarter then flags (shown below).
	//2. flags are accessible via setf function (e.g: std::cout.setf(the flag's name).
	std::cin >> std::setw(10) >> buf; //reads 9 characters + the 10th will always be terminator
#endif

#if 0
	/*
	tl;dr:
	1. we input "abcdef" into istream via cin
	2. cin operator (char) is an overload that extracts single character ("a") from istream into ch
	3. std::cout << ch inserts ch to iostream via cout and cout displays the data on screen
	4. return to the while loop to extract the next character "b" from istream.

	how this loop works:
	1. std::cin is a standard stream, (sequence of bytes accessable sequentially)
	pre-connected to the keyboard, cin itself is an object of type istream, MEANING
	WE WRITE TO ISTREAM VIA CIN WHICH COMMUNICATES WITH THE KEYBOARD

	2. while(std::cin >> ch)
	a. we type characters on the keyboard, e.g: "abcdef", this data lives in the istream
	To extract this data we use the pre-connected stream to keyboard "cin" of type istream.

	b. std::cin >> ch, is actually   basic_istream& std::cin.operator>>(ch) (returns istream for chaining)
	because the function overload of operator>> accepts a char - the overloaded function knows to extract
	only single character from the istream into ch.
	for example: "a" is extracted first into ch, leaving istream with "bcdef" - remember that this is a
	stream (bytes sequence accessible sequentially in a first in first out manner).

	c. std::cout << ch - ch is inserted into iostream via cout and cout displays this data on
	screen.

	READING SEQUENTIALLY!!!! AKA STREAM ABCDEF
	*/
	char ch;
	//the problem is that std::cin skips whitespace (" " space), use std::cin.get(ch) instead
	while (std::cin.get(ch))
		std::cout << ch;
	return 0;
#endif 


#if 0
	char a, b, c;
	std::cin >> a >> b >> c;
	//as explained above, due to synchronization with c's stdin, cin is line buffered - 
	//entering 23\n54\n56\n  (\n is the actual press of "enter" key) \n after 23 causes a flush of  to be flushed from the
	//buffer into a, b,c to be assigned with
	//corresponding numbers. In other words, EXTRACTION OPERATOR extracts until newline character (space)
	std::cout << a << " " << b << " " << c << '\n';
#endif 


#if 0
	char string_buffer[11];
	std::cin.get(string_buffer, 11); //overload of cin.get(), specify max chars, only reads
	//10 chars, the 11th is null-terminator whether we enter the 11th char or not.
	//if we enter more then 11 characters, the remaining ones will be left in the buffer.
	std::cout << string_buffer;

	std::cin.get(string_buffer, 11);//won't fire - because cin.get() does not read newline character:
	//when we typed "abcde\n" into istream in the 1st cin.get() (\n - newline added to the buffer by pressing enter),
	// the 1st cin.get() consumed only "abcde" from istream, "\n" was left in the istream buffer
	//thus the 2nd cin.get encoutered an unempty istream that contained "\n" and stopped, thinking
	//that we we typed something for the 2nd std::cin.get to actuall get.
	//use std::getline instead! it consumes "\n"!

	

#endif 


//input with istream
#if 0
	//string version of getline
	std::string std_string;
	std::getline(std::cin, std_string); //get line reads entire line up to '\n'.
	//if we read from a file, after reading the line, the control goes to the next line in 
	//the file. getline also returns a boolean value if read operation was successful,
	//else false, makingit perfect for a while loop 
#endif

#if 0
	char first_char = std::cin.get(); // entering "John Smith" to istream buffer, get() only retrieves 1st char "J"
	std::cin.ignore(256, ' '); //ignore until white space, istream buffer now contains "Smith"
	char second_char = std::cin.get(); //get() retrieves "S"
	std::cout << first_char << second_char;
#endif

	//output with ostream and ios
#if 0
	std::cout.setf(std::ios::showpos); //prints + for POSitive nubmers
	std::cout << 25 << '\n';

	//setf only turns flag on, it doesn't turn mutually exclusive flags off, such as
	//dec or hex - only one of them can be on, thus we need to manually off dec and on hex
	std::cout.unsetf(std::ios::dec);
	std::cout.setf(std::ios::hex);
	std::cout << 27 << '\n'; // prints 1b
	//alterantive: turns off all basefield flags except of hex
	//std::cout.setf(std::ios::hex, std::ios::basefield)
	std::cout << std::hex << 27 << '\n'; // manipulators are smarter then flags, they
	//turn on & off appropriate flags
	std::cout << std::boolalpha << true << '\n'; // boolalpha manipulator/flag (std::cout.setf(boolalpha))
	//prints "true" instead of 1
	std::cout.setf(std::ios::dec, std::ios::basefield);
	std::cout << 12345 << '\n';

	//set line width of 10 characters, and left justify -12345 -> prints "12345     " 
	std::cout << std::setw(10) << std::left << -12345 << '\n';

	//internal - left justifies the sign and right-justifies the value -> prints "-    12345"
	std::cout << std::setw(10) << std::internal << -12345 << '\n';
	std::cout.fill('*');
	std::cout << std::setw(10) << 123; //prints "*******123"
	/*
	how manipulators such as std::flash or std::hex work?
	https://stackoverflow.com/questions/14105650/how-does-stdflush-work

	*/
#endif

//string streams
#if 1
	/*string stream classes, they simply provide a buffer to hold string data.
	6 string stream classes: istringstream (can only read from it, derived from istream), ostringstream
	(can only write to it, derived from ostream), stringstream(read and write, derived from iostream)
	the three others are for wide strings: wistringstream, wostringstream, wstringstream

	stringstreams extract until whitespace characters (' ', '\t' , '\n'), just like cin.
	(whitespace characters remained in the buffer).

	In general you should use the object that gives you the minimum permissionsand
	functionality that is required to do the job*/


	/******************************stringstream*********************************
	bidirectional input/output stream - we can input to it and read from it*/


	std::stringstream os; //derived from iostream, read & write
	os.str("shalom "); // alternative: os << "shalom";
	//std::cout << os.str(); //prints shalom

	std::stringstream os1("123 456"); //write to os1
	std::string s1;
	os1 >> s1; // output from os1. s1 recieves 123, extracts until whitespace character (space)
	std::string s2;
	os1 >> s2; // s2 receives 456

	std::string s3(os1.str()); // os1.str() retreives all the the string as opposed to >>
	os1.str(""); // erase the buffer, alternative: os.str(std::string{})
	os1.clear(); // reset error flags that may have been set, and returns stream back to ok state
	//(goodbit is set to 1 and the three other flags are set to 0), explained in stream states next.
	//https://www.geeksforgeeks.org/processing-strings-using-stdistringstream/


	/******************************ostringstream and istringstream*********************************
	istringstream - we can only take input from it ("its a keyboard", input stream, processing data from a data producer)
	ostringstream - we can only write to it ("its a screen", output stream, providing data to a data consumer).
	*although we can only read from ostringstream, we can provide initial value
	to both istringstream and ostringstream.*/

	std::string temp;

	std::istringstream iss("istringstream\n");
	iss >> temp; // we can read data from a data producer! it inputs "in" to our prgoram

	std::ostringstream oss("ostringstream\n");
	oss << "hello\n"; // we can write data to a data consumer! "we can "out" data to the stream
	//now oss looks like:
	//hello
	//gstream
	//hello was inserted from the beginning of the stream
	
	//both objects allow us to get the contents of the stream
	std::cout << iss.str();
	std::cout << oss.str();
#endif

//stream states
#if 0
	//ios_base class contains several state flags that are used to signal various conditions
	//that may occur when using streams (accessible also through ios since ios is derived from ios_base).
	//this flags are stored in a bitmask type in ios_base that is called std::ios_base::iostate:

	//1. goodbit -	Everything is okay, accssible by stream.good() (e.g: std::cin.good() or std::cout.good(),
	//also applies to string streams).
	//2. badbit - Some kind of fatal error occurred (e.g.the program tried to read past the end of a file),
	//acessible by bad()
	//3. eofbit - The stream has reached the end of a file, accessible by eof()
	//4. failbit - Logical error on i/o operation, A non-fatal error occurred
	//(eg.the user entered letters when the program was expecting an integer)
	//accessible by fail()
	int age;
	std::cin >> age;
	print_state(std::cin);
	//entering a number, e.g: 12: good()=1 eof()=0 fail()=0 bad()=0 
	//becuase age is an int and we typed a number, the insertion to age is ok.

	//entering characters, e.g "abc": good()=0 eof()=0 fail()=1 bad()=0
	//logical error because we entered characters into age

	//entering "123abc": good()=1 eof()=0 fail()=0 bad()=0
	//operator>> does the best it can an extracts 123 from the buffer
	print_state(std::cin);
	std::cin.clear(); //Clears all flags and restores the stream to the goodbit state
	//so if failbit is set to 1, it is reset to 0 and goodbit is set to 1

#endif

//string validation - full name - letters and space
#if 0
	std::string full_name; //declared here, so it'll be visible to the while loop
	do
	{
		std::getline(std::cin, full_name);
		std::cout << full_name;
	} while (!is_valid_name(full_name));

#endif

//string validation - telephone number
#if 0
	std::string telephone_number;
	do
	{
		std::getline(std::cin, telephone_number);
	} while (!is_valid_telephone(telephone_number , "(###) ### ####" ));
#endif


//std::cin vs std::cin.get vs std::cin.getline vs std::getline
//tl;dr use std::cin for extracting words (up until and not including a whitespace)
//use std::getline to extract complete lines up until and including '\n'.
// in addition getline offers an option for a custom delimiter!
//cin.get and cin.getline don't work with std::strings as opposed to std::getline and std::cin
#if 0

	char arr[4]; // arr is filled with garbage

	//The cin and cin.get() treats a white space i.e " " , "\t" , "\n"
	//as delimiter character. cin -> extracts everything up until it meets '\n'.
	//although \n remains in the buffer after extraction. cin's seem to ignore previous \n characters
	//cin.get() acts the same but following cin.get()'s ARE affected by redundant '\n' characters:
	//when 2 consequtive cin.get() exists, the latter won't execute because the first cin.get()
	//extracted all the characters except '\n', and the 2nd cin.get() encouters the remaining '\n',
	//and extracts no characters from this buffer (if the buffer contains any character, cin.get()
	//would try to extract information from it, in this case, there is nothing to extract
	//(extraction will look like this: ""
	//empty! '\n' will remain in the buffer until will use ignore.
	//on the contrary 2 consequtive cin's do work because the latter cin seems to ignore the previously entered '\n' 
	//by the first cin

	std::cin.get(arr, 4);
	std::cin.get(arr, 4);

	//also note that cin.get doesn't work with std::string, only char arrays. 
	//for extracting "words" (up until a white space character) -> cin is preferred.

	//std::getline on the contrary extracts everything up and including the delimiter '\n'
	//in other words the complete line including '\n' will be flushed from the buffer.
	//the 3rd parameter is a custom delimiter.
	//std::getline works with std:: strings as opposed to std::cin.getline!! use std::getline!!!!!
	std::string str;
	std::getline(std::cin, str);
	//ignore Extracts (flushes) characters from the input sequence (buffer) and discards them, 
	//until either n (100) characters have been extracted, or until the delimiter
	//('\n' in this case) is met where everything up to and including the delimiter
	//is extracted (and discarded) from the buffer.
	std::cin.ignore(100, '\n'); 
	std::cout << std::cin.gcount();

#endif


//numeric validation
#if 0
	int age{0};
	while (true)
	{
		std::cin >> age;

		//fail bit can occur with illogical input into int age, e.g. typing:
		//'abc' - fail because we're trying to extract chars into int age.
		//NO EXTRACTION TAKES PLACE WHEN FAIL BIT OCCURS
		if (std::cin.fail())
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clear the buffer
			std::cin.clear(); // reset error flags
			continue;
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (std::cin.gcount() > 1)
			continue;

		if (age <= 0)
			continue;

		break; //everything is fine.

		//NOTE: ALTERNATIVE TO ALL THIS MAY BE JUST CHECKING IF INPUT IS A DIGIT

	}
#endif

// Basic file I/O
#if 0
	//cpp has 3 basic file I/O classes:
	//1. ifstream (input file stream - Derived from istream - input stream - read only)
	//2. ofstream (output file stream - Dervied from ostream - output stream - write only)
	//3. fstream (file output stream - Derived from iostream - input & output stream - r&w)

	//reading entire content of a file
	std::ifstream file{ "file.txt" };
	if (!file)
	{
		std::cerr << "file could not be opened";
		return 1;
	}

	std::stringstream ss;
	ss << file.rdbuf();//rdbuf returns a pointer to a stream buffer object, and
	//"stringstream operator<<" recieves a reference to a stream buffer, causing the full content
	//of the "file to be dropped into the stringstream
	//(Spported only by stringstream, not supported by std::string)

	//std::cout << iss.str();

	//alternative
	//std::string input;
	//while (file)
		//file >> input;
	//std::cout << input;


#endif

}
