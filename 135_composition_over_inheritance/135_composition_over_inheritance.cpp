#include <iostream>

namespace Inheritance // entity inheritance (who they are)
{
	class Animal
	{
	public:
		void walk() { std::cout << "Animal Walking\n"; }
	};

	class Dog : public Animal
	{
	public:
		void bark() { std::cout << "Dog barking\n"; }
	};

	class Robot
	{
	public:
		void drive() { std::cout << "Robot Driving\n"; }
	};

	class CleaningRobot : public Robot
	{
	public:
		void clean() { std::cout << "Robot cleaning\n"; }
	};

	// now we want a CleaningRobotDog that has all 4 methods method above - problem!
}

namespace Composition // behaviour composition (what they do)
{
	namespace Behaviours
	{
		void(*walk)() = []() {std::cout << "walk\n"; };
		void(*bark)() = []() {std::cout << "bark\n"; };
		void(*drive)() = []() {std::cout << "drive\n"; };
		void(*clean)() = []() {std::cout << "clean\n"; };
		//"std::function<void(void)> walk" is equivalent to "void(*walk)()"
	}

	class CleaningRobotDog // composed of behaviours
	{
	public:
		void(*activity_walk)() = Behaviours::walk;
		void(*activity_bark)() = Behaviours::bark;
		void(*activity_drive)() = Behaviours::drive;
		void(*activity_clean)() = Behaviours::clean;
	};

}

int main()
{
	Composition::CleaningRobotDog cleaning_robot_dog;
	cleaning_robot_dog.activity_bark();
}
