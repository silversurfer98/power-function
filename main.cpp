#include<iostream>
#include"power.h"

// refer exeptions, this is very primitive
// does not handle NaN and inf ot 0's

// this can calculate ln(x) in float precision
// this can calculate exp(x) in float precision
// call appropriate function in order to get values

// AGAIN THIS IS VERY PRIMITIVE OMALE BE CAREFUL


template <typename t>
void print(const char* a, t x)
{
	std::cout << "\n the " << a << " : " << x << "\n\n";
}


int main()
{
	float x, y;
	std::cout << "\n\n***** to find power *******\n\n";
	std::cout << " Enter a : ";
	std::cin >> x;
	std::cout << " Enter b : ";
	std::cin >> y;
	std::cout << "\n\n The ans to " << x << " raised to power of " << y << " is : " << mypower(x, y) << "\n\n";
	return 0;
}