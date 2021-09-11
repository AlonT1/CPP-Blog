/*
timing - timing how much time an execution occurs - used for benchmarking
or to execute statements at a certain time
how much time passes between statement executions
helps in diagnosing which code is faster

chrono is a platform independent way for time measuring - its good and very high resolution
platform spcific, for example win32 api (query performance(?))

for benchmarking using timer, always run it in release mode! in release mode
the compiler performs optimizations! for example in debug, a summing for loop of 100000 iterations
in debug mode is looped 100000 times, but in release the sum is done in compilation mode
*/

#include <iostream>
#include <chrono>
#include <thread>
#include<vector>

class Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end; // this is the return type of now() (hover over now())
	//can't use auto since auto must be initialized at decleration
	Timer()
	{
		start = std::chrono::high_resolution_clock::now(); //auto because long return type
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		std::cout << "Timer took" << duration.count() * 1000.0f << std::endl;
	}
};


class Timer2
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
public:
	Timer2()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer2()
	{
		end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
	}
};

/*
using stack object life paradigm to automate timing.
Timer timer logs the start time in the ctor and
when Timer timer object goes out of scope, the destructor will run and log the end time.
this is not useful inside main(), since when main dies, the program terminates.
*/
void function()
{
	Timer timer;
	for (size_t i = 0; i < 100; i++)
	{
		std::cout << "hello\n"; //faster than std::endl since sstd::endl flushes the buffer
	}

}

int main()
{
	std::vector<int> x;
	using namespace std::literals::chrono_literals; //used for "s" in 1s in sleep_for()
	auto start = std::chrono::high_resolution_clock::now(); //auto because long return type
	//gives us the current time
	std::this_thread::sleep_for(1s); //pause execution in this thread for 1s
	auto end = std::chrono::high_resolution_clock::now(); //auto because long return type
	std::chrono::duration<float> duration = end - start;
	std::cout << duration.count() << std::endl;

	//alternative: // using a struct - scoped timer!
	function();


	//shortest version:
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration<double>(end - start).count();
}
