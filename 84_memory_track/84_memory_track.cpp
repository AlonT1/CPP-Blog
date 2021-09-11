
#include <iostream>

struct MemoryAllocations
{
    int total_size_allocated = 0;
    int total_size_deallocated = 0;
};

static MemoryAllocations mem_allocations;

void* operator new (size_t size)
{
    std::cout << "sized allocated: " << size << std::endl;
    mem_allocations.total_size_allocated++;
    return malloc(size);
}

void operator delete (void* ptr, size_t size)
{
    std::cout << "freeing: " << size << std::endl;
    free(ptr);
    mem_allocations.total_size_deallocated++;
}

int main()
{
    std::string s = "shalom";
    std::unique_ptr<std::string> ptr = std::make_unique<std::string>(s);
}