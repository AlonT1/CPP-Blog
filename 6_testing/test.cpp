#include <iostream>
#include <bit>
#include <numbers>
#include <format>

//https://www.modernescpp.com/index.php/std-format-in-c-20
//https://en.cppreference.com/w/cpp/io/c/fprintf

int main()																	   
{
	std::cout << std::format("{:^11X}", '|');

	//std::cout << (std::endian::native == std::endian::little) ? true : false;
	//std::cout << std::format("value < 0: {}\n", std::is_lt(ordering));
	for (double radius{ 2.5 }; radius <= 20.0; radius += 2.5)
	{
		std::cout << std::format("radius = {:2.3f}, area = {:7.2f}\n", radius, std::numbers::pi * radius * radius);
	}


	const double pi = std::numbers::pi;
	std::cout << std::format("Default: {:.2}, fixed: {:.2f}, scientific: {:.2e}, "
		"general: {:.2g}\n", pi, pi, pi, pi);
	//std::cout << std::format("Default: {}, binary: {:b}, hex.: {:x}\n", 314, 314, 314);
	//std::cout << std::format("Default: {}, decimal: {:d}, hex.: {:x}\n", 'c', 'c', 'c');
	//std::cout << std::format("Alternative hex.: {:#x}, binary: {:#b}, HEX.: {:#X}\n",
	//	314, 314, 314);
	//std::cout << std::format("Forced sign: {:+}, space sign: {: }\n", 314, 314);
	//std::cout << std::format("All together: {:*<+10.4f}, {:+#09x}\n", pi, 314);

	// carrots is a ptr to 1st of 3 double ptr(s) where each pints to 1st of 4 double(s)
	// (notice that 3 is dictated by the initialization)

	


}