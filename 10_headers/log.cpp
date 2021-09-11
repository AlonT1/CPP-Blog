#include "log.h"

#include <iostream>

void init_log()
{
	log("dwdw");
}

void log(const char* msg)
{
	std::cout << msg << std::endl;
}
