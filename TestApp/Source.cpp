#include "..\projects\S0006E_Assignment1\code\matrix4.h"

#include <iostream>


int main()
{
	Assignment::Matrix4 id;
	Assignment::Matrix4 inc;
	Assignment::Matrix3 minor;

	for (int i = 0; i < 16; i++)
	{
		inc(i) = i;
	}

	std::cout << id.to_string() << "\n";
	std::cout << inc.to_string() << "\n";
	std::cout << inc.minor(3, 3).to_string() << "\n";

	std::cout << "Det: " << inc.determinant() << "\n\n";

	std::cout << inc.getInverse().to_string() << "\n";

	return 0;
}

