#include "..\VectorLib\code\matrix4.h"

#include <iostream>

int main()
{
	/// MAT2
	std::cout << " === MAT2 === \n\n";

	efiilj::Matrix2 mat2a, mat2b;

	mat2a = efiilj::Matrix2(0, 1, 2, 3);
	mat2b = efiilj::Matrix2(5, 6, 7, 8);
	auto vec2a = efiilj::Vector2(10, 12);

	std::cout << (mat2a * mat2b).to_string() << "\n";
	std::cout << (mat2a * vec2a).to_string() << "\n";
	std::cout << "Det (a): " << mat2a.determinant() << "\n\n";

	/// MAT3
	std::cout << " === MAT3 === \n\n";

	efiilj::Matrix3 mat3a, mat3b;
	auto f = efiilj::Vector3(1, 2, 3);

	for (int i = 0; i < 9; i++)
	{
		mat3a(i) = i;
	}

	std::cout << mat3a.to_string() << "\n";
	std::cout << mat3a.transpose().to_string() << "\n";
	std::cout << (mat3a * mat3a).to_string() << "\n";

	mat3b = efiilj::Matrix3(efiilj::Vector3(0, 2, 2), efiilj::Vector3(1, -1, 1), efiilj::Vector3(0, 1, 2));

	std::cout << mat3b.to_string() << "\n";
	std::cout << (mat3b * mat3b).to_string() << "\n";
	std::cout << (mat3a * f).to_string() << "\n";
	std::cout << "Det (e): " << mat3b.determinant() << "\n\n";
	std::cout << mat3b.inverse().to_string() << "\n";

	/// MAT4
	std::cout << " === MAT4 === \n\n";

	auto g = efiilj::Matrix4(efiilj::Vector4(1, 1, 1, 0), efiilj::Vector4(0, 3, 1, 2), efiilj::Vector4(2, 3, 1, 0), efiilj::Vector4(1, 0, 2, 1));
	auto h = efiilj::Vector4(1, 2, 3, 4);

	std::cout << g.to_string() << "\n";

	std::cout << (g * g).to_string() << "\n";
	std::cout << (g * h).to_string() << "\n";
	std::cout << g.transpose().to_string() << "\n";
	std::cout << "Det (g): " << g.determinant() << "\n\n";
	std::cout << g.inverse().to_string() << "\n";

	/// ROT MAT
	std::cout << " === ROT MAT === \n\n";

	float pi = 3.14159265359;

	auto rx = efiilj::Matrix4::getRotationX(pi / 3);
	auto ry = efiilj::Matrix4::getRotationY(pi / 3);
	auto rz = efiilj::Matrix4::getRotationZ(pi / 3);
	auto rxyz = efiilj::Matrix4::getRotationXYZ(pi / 3, efiilj::Vector3(1, 2, 3));

	std::cout << rx.to_string() << "\n";
	std::cout << ry.to_string() << "\n";
	std::cout << rz.to_string() << "\n";
	std::cout << rxyz.to_string() << "\n";

	return 0;
}

