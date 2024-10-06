#include <iostream>
// a->b == (*a).b
// -> member access through pointer to an object (if we have a pointer to an object
//we can access the object's members via "->")
// . member access through object (if an object as allocated on the stack, we can access
//it's members directly via ".")
// *a -> indirection
class Log
{
public:
    enum class Level // MUST be defined above its usage!!! //enums are without namespaces! no Log::Level::... but Log::Info 
    {
        ERROR = 0, WARNING = 1, INFO = 2
    };
private:
    Level m_log_level = Level::INFO;
public:
    void set_level(Level level)
    {
        
        m_log_level = level;
    }

    void error(const char* message)
    {
        std::cout << "[ERROR]" << message << std::endl;
    }

    void warn(const char* message)
    {
        std::cout << "[WARNING]" << message << std::endl;
    }

    void info(const char* message)
    {
        std::cout << "[INFO]" << message << std::endl;
    }
};

int main()
{
    Log log;
    log.set_level(Log::Level::ERROR);
    log.info("hello");
    std::cout << "Hello World!\n";
}
