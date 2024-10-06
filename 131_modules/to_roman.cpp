// to_roman.cpp – Implementation of the to_roman() function
// note that instead of two seperate .cpp file that implement from_roman and two_roman
// we can also have a single .cpp that implements both functions from the module interface, 
// or even implement the two functions directly in the module itself.


module roman; // implicitly inherits all imports (string, etc.. )from the module interface file "roman.ixx"!
//and imports all the forwrad declarations so we cand define them here.

// note that we always import/export modules (.ixx) and not implementation (.cpp)
std::string to_roman(unsigned int i)
{
	if (i > 3999) return {}; // 3999, or MMMCMXCIX, is the largest standard Roman numeral
	static const std::string ms[]{ "","M","MM","MMM" };

	static const std::string cds[]{ "","C","CC","CCC","CD","D","DC","DCC","DCCC","CM" };
	static const std::string xls[]{ "","X","XX","XXX","XL","L","LX","LXX","LXXX","XC" };
	static const std::string ivs[]{ "","I","II","III","IV","V","VI","VII","VIII","IX" };
	return ms[i / 1000] + cds[(i % 1000) / 100] + xls[(i % 100) / 10] + ivs[i % 10];
}