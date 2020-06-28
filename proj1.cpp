#include "proj1.h"

#include <iostream>

extern void printHelloWorld()
{
	std::cout << "Hello World" << std::endl;
}

int main()
{
	printHelloWorld();

	return 0;
}