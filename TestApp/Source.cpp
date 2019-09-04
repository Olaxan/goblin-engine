#include "..\projects\S0006E_Assignment1\code\matrix4.h"

#include <iostream>

int main()
{
	/// MAT2
	std::cout << " === MAT2 === \n\n";

	Assignment::Matrix2 a, b;

	a = Assignment::Matrix2(0, 1, 2, 3);
	b = Assignment::Matrix2(5, 6, 7, 8);
	auto c = Assignment::Vector2(10, 12);

	std::cout << (a * b).to_string() << "\n";
	std::cout << (a * c).to_string() << "\n";
	std::cout << "Det (a): " << a.determinant() << "\n\n";

	/// MAT3
	std::cout << " === MAT3 === \n\n";

	Assignment::Matrix3 d, e;
	auto f = Assignment::Vector3(1, 2, 3);

	for (int i = 0; i < 9; i++)
	{
		d(i) = i;
	}

	std::cout << d.to_string() << "\n";
	std::cout << d.getTransposed().to_string() << "\n";
	std::cout << (d * d).to_string() << "\n";

	e = Assignment::Matrix3(Assignment::Vector3(0, 2, 2), Assignment::Vector3(1, -1, 1), Assignment::Vector3(0, 1, 2));

	std::cout << e.to_string() << "\n";
	std::cout << (e * e).to_string() << "\n";
	std::cout << (d * f).to_string() << "\n";
	std::cout << "Det (e): " << e.determinant() << "\n\n";
	std::cout << e.inverse().to_string() << "\n";

	/// MAT4
	std::cout << " === MAT4 === \n\n";

	auto g = Assignment::Matrix4(Assignment::Vector4(1, 1, 1, 0), Assignment::Vector4(0, 3, 1, 2), Assignment::Vector4(2, 3, 1, 0), Assignment::Vector4(1, 0, 2, 1));
	auto h = Assignment::Vector4(1, 2, 3, 4);

	std::cout << g.to_string() << "\n";

	for (int i = 0; i < 16; i++)
	{
		std::cout << "(" << i << ") det: " << g.minor(i & 4, i / 4).determinant() << "\n\n" << g.minor(i & 4, i / 4).to_string() << "\n";
	}

	std::cout << "\n///////\n";

	std::cout << (g * g).to_string() << "\n";
	std::cout << (g * h).to_string() << "\n";
	std::cout << g.getTransposed().to_string() << "\n";
	std::cout << "Det (g): " << g.determinant() << "\n\n";
	std::cout << g.inverse().to_string() << "\n";

	return 0;
}

