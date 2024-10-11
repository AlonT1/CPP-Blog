#include <iostream>
// see 18_classes first
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
