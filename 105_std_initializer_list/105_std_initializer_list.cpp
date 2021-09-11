
#include <iostream>
#include <initializer_list>

template <typename T, int size>
class Array
{
public:
	T arr[size];

	Array()
	{
		for (int i{ 0 }; i < size; ++i)
			arr[i] = 0;
	}

	//std::initializer_list is a prvalue object, therefore we can accept it either by
	//value (causes a copy) or by a const lvalue ref which can accept prvalues
	//(explained in 85_lr_values)
	Array(const std::initializer_list<T>& list) //alternative: template parameter pack (see 108_if_constexpr)
	{
		int index{ 0 };
		for (T val : list)
			arr[index++] = val;
	}

	Array& operator=(const std::initializer_list<T>& list)
	{
		init_list(list);
		return *this;
	}

	friend std::ostream& operator<< (std::ostream& os, const Array& array)
	{
		for (T val : array.arr)
		{
			os << val << " ";
		}
		os << '\n';
		return os;
	}
};


int main()
{
	int array[]{ 1,2,3,4,5 }; //initializer list
	//can we have something similar in a user defined class? YES!

	Array<int, 6> my_array1{1,2,3,4,5};
	
	/*
	One caveat: Initializer lists will always favor a matching initializer_list
	constructor over other potentially matching constructors. IntArray array { 5 };
	would match to IntArray(std::initializer_list<int>), not IntArray(int).
	*/
}