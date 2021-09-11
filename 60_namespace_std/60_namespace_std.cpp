/*
using namespace std advantages:
easily identify what belongs to the standard library - BEING EXPLICIT IS GOOD
no "using namespace" in headers!!!!!!!!!! because they are get included in translation units:
because of the "using namespace"  in the header file - all of the content of the namespace will be dumped in the code. 
this is what we are trying to avoid by explicilty writing namespace::x
using namespace can cause naming conflicts
"using namespace" should be as scoped as possible (inside if, function...)
*/
#include <iostream>
#include <vector>

namespace test
{
    void cout(const std::string& text)
    {
        std::cout << text << std::endl;
    }
}

using namespace std;
using namespace test;
int main()
{
    std::vector<int> a; // clear that this belongs to std!
    vector<float> w; 
    cout("ken"); // cout is ambiguous - in std namespace & in test!
    //cout will be activate in the namespace that matches the best the argument
    //in the function through implicit coversion - the argument is const char array
    //and not std::string
}
