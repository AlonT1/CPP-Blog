/*
iterators - 
1.an object pointing to some elemnt of a container. used to iterate over  collection of elements.
2. implements the iterator design pattern
3. Usually used to iterate over containers, especially those without random access ([])
such as sets.
4. Iterators serve an an interface between containers and stl algorithms
which usually take algorithms as arguemnts and manipulate the container with them.


*/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <forward_list>
#include <list>

int main()
{


	
	/*Iterators are divided into 5 categories:
	↑ means inheritance, vor example forward iterators is-a input iterator and 
	and output iterator.
	
	Input Iterator				Output Iterator
		↑							↑
		|___________________________|
					  |
				Forward Iterator (added feature: dereference returns lvalue ref (can assign to it)
					  ↑
			  Bidirectional Iterator (added feature: iterating backwards (all below iterates only forward)
					  ↑
			  Random Access Iteraotr (added feature: random access via operator [])

	STL data structures return iterators pointing to different locations.
	assuming that the following data structure (can be array, linked list, etc..),
	the default iterators are set to the following positins

			preceding-the-first element	, 100	,  101	, 102	, 103	, 104	, 105   past-the last element
							↑			   ↑									   ↑			↑
	iterators names:	rend()			 begin()								rbegin()		end()

	*the names above are actually of the name of functions returning the predefined
	iterators that points to the locations above (example in "Additional Notes" section below)

	begin and end iterators work in conjunction.
	rbegin and rend iterators work in conjunction.
	we cannot mix begin with rend or end with rbegin!

	*STL iterators don't provide bound checking

	**iterators associated with STL containers
	1. Vector: Random - Access
	2. List: Bidirectional
	3. Deque: Random - Access
	4. Map: Bidirectional
	5. Multimap: Bidirectional
	6. Set: Bidirectional
	7. Multiset: Bidirectional

	*/

#if 0
	/***************************1. input iterators*******************************
	definition:
	read-only iterator, iterates only in forward direction.
	because they are read-only they are used by non mutating, single pass algorithms such as find() and find_if()
	which themselves return an input iterator to a found object.
	Input iterators can be assigned with another iterator, constructed, dereferenced, and pre/post incremented.

	in practice:*************************************
	input iterators are initialized with some "data producer" (whom we get input from), and the input
	iterator iterates over this input.
	examples of input iterators:
	1. istream_iterator
	"data producers" (that istream_iterator can iterate on) can be:
	1. cin (istream object preconnected to the keybaord),
	2. istringstream (or stringstream which IS an istringstream)
	3. ifstream (or fstream which IS an istream)
	4....

	we interact with the iterator just like a regular iterator (e.g: *iterator, iterator++,

	example 1: */

	//we can only read from istringstream (input stream) (reading from a data producer)
	//with the exception of giving it (write to it) an initial value with a ctor.
	std::istringstream iss{ "0.1 0.2 0.3 0.4" };
	//istream_iterator is "white space" delimited by default - no way to change that.
	std::istream_iterator<double> isi(iss); //iss is the "data producer", "in" to our program

	//default constructed istream_iterator is the end-of-stream iterator
	//(one element after the end of the stream)
	std::istream_iterator<double> eos; 

	while (isi != eos) 
	{
		std::cout << *isi;
		++isi;
	};
	std::cout << '\n';


	//example 2
	std::vector<int> v;

	//std::copy(InputIt first, InputIt last, OutputIt d_first ):
	//std::copy copies the elements in the range, defined by [first, last ) into d_first
	//1st parameter: we parameterize istream_iterator (input stream iterator) as an int

	//copying elements FROM the input (data producer - keyboard cin) into a vector
	std::copy(
		std::istream_iterator<int>(std::cin), //iterating over cin
		std::istream_iterator<int>(),  // stop when we iterate over a  non int or EOF (Ctrl+Z in Windows.)
		 //each element is copied into the constructed std::back_inserter(v) which insertes into the vector - 
		//behind the scenes: *std::back_inserter(v) = *std::istream<int>(std::cin)
		std::back_inserter(v));
	for (int i : v) std::cout << i;

	/*
	copy behind the scences:
	template <class InIt, class OutIt>
	void std::copy(InIt b, InIt e, OutIt d) {
	while (b != e) {
		*d = *b;
		++b;
		++d;
	}
}
	*/

	/*
	Important Note: after we read a through a stream via a while loop or an iterator,
	we might reach the end of it. therefore, to reread the stream we need
	to clear error flags (specifically eof) - stream.clear() and then rewind stream.seekg(0)
	*/

#endif


#if 0
	/***************************2. output iterators*******************************
	for example - ostream itreators: write sequentially to an output stream (such as cout).
	write only iterators, therefore they can be used by non mutating, 
	single pass algorithms such as find() and find_if()
	Dereferencing and assigning a different value to the iterated value via the iterator is not supported
	(the iterator returns a prvalue).

	in practice:*************************************
	output iterators are initialized with some "data consumer" (whom we provide data to),
	and the output iterators iterates over this output.
	examples of output iterator:
	1. ostream_iterator
	"data consumers" (that ostream_itreator can iterate on) can be:
	1. cout (ostream object preconnected to the screen),
	2. ostringstream (or stringstream which IS an ostringstringstream)
	3. ofstream (or fstream which IS an ofstream)
	4....*/


	std::vector<int> myvector;
	for (int i = 1; i < 10; ++i) myvector.push_back(i * 10);
	//myvector contains {10, 20, 30, 40, 50, 60, 70, 80, 89}

	//ostream_iterator parameterized with int, takes in an ostream stream object such as cout
	//and an optional a delimieter that is written to the stream after each insertion
	std::ostream_iterator<int> out_it(std::cout, ", ");

	//copying elements FROM a vector to a data consumer (screen - cout)

	//iterating through the vector, and copying each element to the output stream iterator
	//who adds ", " after each inserted element
	std::copy(myvector.begin(), myvector.end(), out_it);
	//output: 10, 20, 30, 40, 50, 60, 70, 80, 90, 

#endif


#if 0
	/***************************3. forward iterators******************************
	forward iterator is-a input iterator and output iterator (see inheritance diagram above)
	all can move only in a forward direction. 
	forward iterator adds the abillity to dereference the iterator and this returns an lvalue ref
	to the dereference memory location, allowing us to read (get input) from the iterator and write/assing (output)
	to the iterator. this is in opposed to input and output iterators where the dereference operator returns a prvalue.
	*/
	std::forward_list<int> list {1, 2, 3}; //forward_list == singly linked list
	//forward_list iterator is a forward iterator
	std::forward_list<int>::iterator list_iterator = list.begin();
	//THE ADDED FEATURE OF FORWARD ITERATORS - dereference returns an lvalue ref, allowing for assignmnets:
	// changing the first element to 5 (list_iteartor points to list.begin()
	*list_iterator = 5;
	while (list_iterator != list.end()) std::cout << *(list_iterator++);


#endif

#if 0	
	/***************************4. Bidirectional Iterator******************************
	bidirectional iterators have the same features of forward iteator
	(derefencing returns an lvalue ref) + it adds the abillity to iterate in both
	directions.*/
	std::list<int> list2{ 1,2,3,4 }; //std::list is a doubly linked list
	std::list<int>::iterator list2_iterator = list2.begin();
	while (list2_iterator != list2.end()) std::cout << *(list2_iterator++);

	//THE ADDED FEATURE of bidirectional iterators: iterating forwards AND backwards:
	list2_iterator--; 	// decrementing the iterto the last element (currently it points to the past-the-last element)
	while (list2_iterator != list2.begin()) std::cout << *(list2_iterator--);


#endif

#if 1
	/***************************5. Random Access Iterator*****************************
	random access iterator has the most features. it inherits from forward iterator and
	bidirectional iterator. random access iterator adds the abillity to perform random
	access via [] operator
	*/
	std::vector<int> vector{ 1,2,3,4 };
	std::vector<int>::iterator vector_iterator = vector.begin();
	std::cout << vector_iterator[0]; //random access via operator []


	/*
	//******iterator to the 1st element (100)
	std::vector<int>::iterator itr_begin = arr.begin(); //forward iterator of a vector

	//short while loop iteration
	std::cout << "short whle loop oteration: ";
	//* and ++ opeartor precedence explained in 16b_precedence
	while (itr_begin != arr.end()) std::cout << *(itr_begin++) << ' ';
	std::cout << '\n';*/
#endif 

#if 1
	/***************************Additional Notes******************************/
	std::vector<int> arr{ 100, 101, 102, 103, 104, 105 };

	/* 1. simple iteration via the iterator of a vector (bidirectional iterator)
	
	std::vector<int>::iterator predefined iterator of STL Vector class
	STL iterators don't provide bound checking
	arr.begin() - Returns an iterator pointing to the first element in the sequence
	arr.end() - Returns an iterator pointing to past-the-end element in the sequence, meaning
	not the last element, but the location in memory past it (allows for it!=arr.end() in the for loop,
	continue until we are past the last element, stops when it==arr.end(), after we traversed the whole vector)*/
	std::cout << "for loop with iterator: ";
	for (std::vector<int>::iterator it = arr.begin(); it != arr.end(); it++)
		std::cout << *it << " "; //* syntax works because it is overloaded in the iterator class (iterator is not a pointer)
	std::cout << '\n';


	/* 2. reverse iterator of a vector (a bidirectional iterator)

	rbegin() iterator points to the element before the past-the-end element, in 
	other words the actual last element(105). incrementing this iterator will cause
	it to iterate in reverse until rend() which is an iterator that points to
	preceding-the-first element in the vector (element before 100)*/
	std::vector<int>::reverse_iterator itr_rbegin = arr.rbegin(); //reverse iterator
	while (itr_rbegin != arr.rend()) std::cout << *(itr_rbegin++) << ' ';
	std::cout << '\n';


	//itearators are mandatory for data structures don't have a contiguous indexing systems,
	//such as a tree, unordered sets/map unordered map:
	std::unordered_map<std::string, int> map;
	map["apple"] = 5; // although "apple" is a const char*, it works due to implicit conversion of "apple" to the converting ctor 
	//std::string::string(apple) similarly to std::string x = "apple" (converting ctor's (those who aren't declared "explicit"
	//allow for implicit conversion)
	map["zero"] = 0;

	//indexing are impossible! but iteration via an iterator is!

	//iterator type is long, can use auto instead. "auto" may be too vague, so we can use "using" instead:
	using map_iterator = std::unordered_map<std::string, int>::const_iterator;

	//iterator is of type const_iterator, meaning the values it retrieves cannot be modified
	for (std::unordered_map<std::string, int>::iterator it = map.begin(); it != map.end(); it++)
	{
		//first is the key, second is the value. assigning to auto& ensures
		//we don't create a copy of the iterator
		auto& key = it->first;
		auto& value = it->second;
	}

}
