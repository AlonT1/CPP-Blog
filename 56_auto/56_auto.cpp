/*
we can type auto and the compiler can automatically know the type of the variable
the concept of auto is . automatically deduces the type of the variable
auto is vague... may change type.
use it when the type is really long
*/

#include <iostream>
#include <vector>
#include <unordered_map>

std::string get_name() 
{
    return "Shalom";
}

class Device {};

class DeviceManager
{
private:
    std::unordered_map<std::string, std::vector<Device*>> m_devices;
public:
    const std::unordered_map<std::string, std::vector<Device*>>& get_devices() const
    {
        return m_devices;
    }
};

int main()
{
    //auto x; auto must be initialized at decleration!
    int a = 5;
    auto b = a; // compiler knows b is a string

    auto name = get_name(); // if we need to change std::String to char*, auto will still work

    std::vector<std::string> strings;
    strings.push_back("apple");
    strings.push_back("orange");

    for (int i = 0; i < 2; i++)
        std::cout << strings.at(i) << std::endl;

    //for loop explained:
    //initialization: we initialize a const iterator it with strings.begin() - 
    //returns a pointer to the beginning of strings vector
    //condition: as long as "it" iterator isn't equal to strings.end() which is the address
    //that exist 1 step AFTER the last element of the array. The iterator traverses the
    //vector and the only way to be sure that we've reached the end of the vector
    //is when we are done with the last element and reached the "non-existing element" after
    //the last element.
    //update: increase the memory address of "it" iterator, essentialy pointing the
    //iterator to the next element
    for (std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); it++) // returns iterator
        //std::vector<std::string>::const_iterator is a huge type! can be replaced by auto
        std::cout << *it << std::endl;

    DeviceManager dm;
    const std::unordered_map<std::string, std::vector<Device*>>& devices = dm.get_devices();
    //very long type -> alternatives:

    //1. "using" alternative
    using DeviceMap = std::unordered_map<std::string, std::vector<Device*>>;
    const DeviceMap& devices = dm.get_devices();

    //2. auto
    const auto& DevMap = dm.get_devices();

}
