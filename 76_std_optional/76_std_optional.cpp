#include <iostream>
#include<fstream>
#include <string>
#include <optional>

/*
std optional - helps dealing in data that may or may not be there
optional wraps around the return type - therefore std::optional can house 2 parameters:
1. a parameter that has the type of the template parameter, for example std::optional<std::string>
can store "std::string:
2. empty optional (std::optional<std::string>{} or std::nullopt or {} that is interpreted as 
a null string because the template parameter is std::string)
*/
std::optional<std::string> read_file_as_string(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (stream.is_open())
	{
		for (std::string str; getline(stream, str);)
		{
			stream.close();
			return str;
		}
	}
	return std::nullopt; //alternatives above
}

int main()
{
	//check if the file read was successful
	
	/*technique 1 - string length
	std::string data = read_file_as_string("text.txt");
	if (!data.length())
	{
		//code 
	}
	*/
	
	/*technique 2 - ref bool
	bool is_file_opened_successfully;
	std::string data = read_file_as_string("text.txt", is_file_opened_successfully);
	if (is_file_opened_successfully)
	{

	}*/

	//technique 3 - std::optional
	std::optional<std::string> data = read_file_as_string("text.txt");
	std::string str = data.value_or("test"); // gets the data stored in "data"
	//if data is {} (empty std::optional, or nullstring) then the value in the arguemnt
	//is assigned to str ("test" assigned to str). otherwise, the non-null, existing
	//value is assigned to str (for example if a file is not empty, its content are
	//assigned to str, but if it's empty, "test" will be assigned to str)
	
	//alternative - data.value()
	if (data.has_value()) // if not {} (empty std::optional). alternative -> if(data)
	{
		std::cout << "file found\n";
	}
	else // not read successfuly
	{
		std::cout << "no file found\n";
	}



}
