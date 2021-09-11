//https://www.fluentcpp.com/2018/04/27/tag-dispatching/

#include <iostream>

/*
Tag dispatching is a way to differentiate between function overloads by providing an additional parameter
to the definition of the overload. This parameter is the tag (usually an empty struct) and we call
the function with a specific tag, the specific overload will execute.
(Tags can be also thought as flags). tags in stl are usually suffixed with _t

*/
//forward_itrerator_tag, causes the dispatch of this overloaded function,
//forward iterators can only move forwards and support ++
template <typename Iterator, typename Distance>
void advance_impl(Iterator& it, Distance n, forward_iterator_tag) 
{
    while (--n >= 0)
        ++it;
}


//random_iterator_tag, causes the dispatch of this overloaded function,
//forward random_iterator_tag can supports +=
template <typename Iterator, typename Distance>
void advance_impl(Iterator& it, Distance n, random_iterator_tag)
{
    it += n;
}


/**********Another example of tag_dispatching:
because constructors don't have a name, and are only differentiated by parameters,
we can provide a tag to the constructor that specifies information about the type, 
or about the behaviour of the construction. This will help us to choose the appropriate
ctor
*/
class MyClass
{
public:
    //the tags. should be empty - they are merely a flag
    static struct ConstructThisWay {} constructThisWay;
    //"static struct ConstructThatWay {}" is a declaration of a struct and
    //constructThatWay is an instiation of the struct
    //equivalent to "ConstructThatWay constructThatWay;"
    static struct ConstructThatWay {} constructThatWay;

    //the ctors. each ctor will be dispatched according to the tag (flag) we provide
    MyClass(ConstructThisWay) {};
    //parameter name isn't necessary since we won't refer to the tag, we only use
    //it to dispatch a ctor.
    MyClass(ConstructThatWay) {};

    //an alternative to a tag in a form of struct - a atg in a form of enum
    static enum class Tag1 {};
    static enum class Tag2 {};

    MyClass(Tag1) {};
    MyClass(Tag2) {};
};


int main()
{
    //dispatch the ctor according to the struct tag (==flag)
    MyClass x(MyClass::constructThatWay);

    //dispatch the ctor according to the enum tag (==flag)
    MyClass y(MyClass::Tag1);
}

/**********Another example of tag_dispatching:
emplace has a special ctor that can be activated with the tag: std::piecewise_constructor -
see 109 std_map for additional details
*/