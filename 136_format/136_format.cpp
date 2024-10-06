#include <iostream>
#include <format>
#include <numbers>
#include <vector>

int main()
{
	const double pi = std::numbers::pi; //cpp20
	std::cout << std::format("Default: {:2.4}, fixed: {:.2f}, scientific: {:.2e}, "
		"general: {:.2g}\n", pi, pi, pi, pi);
	//prints:
	//Default: 3.1, fixed : 3.14, scientific : 3.14e+00, general : 3.1







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
}