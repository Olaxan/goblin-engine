#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector4.h"
#include "matrix3.h"

namespace Assignment
{

	class Matrix4
	{
	private:
		Vector4 _arr[4];

	public:
		Matrix4()
		{
			clear();
		}

		Matrix4(const Matrix2& copy)
		{
			clear();

			(*this)(0, 0) = copy.a();
			(*this)(1, 0) = copy.b();
			(*this)(0, 1) = copy.c();
			(*this)(1, 1) = copy.d();
		}

		Matrix4(const Matrix3& copy)
		{
			clear();

			(*this)(0, 0) = copy.at(0, 0);
			(*this)(1, 0) = copy.at(1, 0);
			(*this)(2, 0) = copy.at(2, 0);

			(*this)(0, 1) = copy.at(0, 1);
			(*this)(1, 1) = copy.at(1, 1);
			(*this)(2, 1) = copy.at(2, 1);

			(*this)(0, 2) = copy.at(0, 2);
			(*this)(1, 2) = copy.at(1, 2);
			(*this)(2, 2) = copy.at(2, 2);
		}

		Matrix4(const Matrix4& copy)
		{
			*this = copy;
		}

		Matrix4(Vector4& a, Vector4& b, Vector4& c, Vector4& d)
		{
			_arr[0] = a;
			_arr[1] = b;
			_arr[2] = c;
			_arr[3] = d;
		}

		Matrix4 operator * (const Matrix4& other) const
		{
			Matrix4 mat;

			mat(0, 0) = Vector4::dot4(row(0), other.col(0));
			mat(0, 1) = Vector4::dot4(row(1), other.col(0));
			mat(0, 2) = Vector4::dot4(row(2), other.col(0));
			mat(0, 3) = Vector4::dot4(row(3), other.col(0));

			mat(1, 0) = Vector4::dot4(row(0), other.col(1));
			mat(1, 1) = Vector4::dot4(row(1), other.col(1));
			mat(1, 2) = Vector4::dot4(row(2), other.col(1));
			mat(1, 3) = Vector4::dot4(row(3), other.col(1));

			mat(2, 0) = Vector4::dot4(row(0), other.col(2));
			mat(2, 1) = Vector4::dot4(row(1), other.col(2));
			mat(2, 2) = Vector4::dot4(row(2), other.col(2));
			mat(2, 3) = Vector4::dot4(row(3), other.col(2));

			mat(3, 0) = Vector4::dot4(row(0), other.col(3));
			mat(3, 1) = Vector4::dot4(row(1), other.col(3));
			mat(3, 2) = Vector4::dot4(row(2), other.col(3));
			mat(3, 3) = Vector4::dot4(row(3), other.col(3));

			return mat;
		}

		Matrix4 operator * (const float& other) const
		{
			Matrix4 mat = *this;

			for (int i = 0; i < 16; i++)
			{
				mat(i) *= other;
			}

			return mat;
		}

		Matrix4 operator / (const float& other) const
		{
			Matrix4 mat = *this;

			return mat * (1 / other);
		}

		Matrix4& operator = (const Matrix4& other)
		{
			(*this)(0, 0) = other.at(0, 0);
			(*this)(0, 1) = other.at(0, 1);
			(*this)(0, 2) = other.at(0, 2);
			(*this)(0, 3) = other.at(0, 3);

			(*this)(1, 0) = other.at(1, 0);
			(*this)(1, 1) = other.at(1, 1);
			(*this)(1, 2) = other.at(1, 2);
			(*this)(1, 3) = other.at(1, 3);

			(*this)(2, 0) = other.at(2, 0);
			(*this)(2, 1) = other.at(2, 1);
			(*this)(2, 2) = other.at(2, 2);
			(*this)(2, 3) = other.at(2, 3);

			(*this)(3, 0) = other.at(3, 0);
			(*this)(3, 1) = other.at(3, 1);
			(*this)(3, 2) = other.at(3, 2);
			(*this)(3, 3) = other.at(3, 3);

			return *this;
		}

		Vector4& operator *= (Vector4& other) const
		{
			other = (*this) * other;
			return other;
		}

		Vector4 operator * (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(other.dot4(row(0)));
			vect.y(other.dot4(row(1)));
			vect.z(other.dot4(row(2)));
			vect.w(other.dot4(row(3)));
			return vect;
		}

		float& operator () (int i)
		{
			return _arr[i / 4][i % 4];
		}

		float& operator () (int x, int y)
		{
			return _arr[y][x];
		}

		float at(int n) const
		{
			if (n > 16)
				throw new std::out_of_range("Matrix index out of range");

			return at(n % 4, n / 4);
		}

		float at(int x, int y) const
		{
			if (x > 4 || y > 4)
				throw new std::out_of_range("Matrix index out of range");

			return _arr[y].at(x);
		}

		Vector4 row(int y) const
		{
			if (y > 4)
				throw new std::out_of_range("Row index out of range");

			return Vector4(at(0, y), at(1, y), at(2, y), at(3, y));
		}

		Vector4 col(int x) const
		{
			if (x > 4)
				throw new std::out_of_range("Column index out of range");

			return Vector4(at(x, 0), at(x, 1), at(x, 2), at(x, 3));
		}

		void clear(bool identity = true)
		{
			for (int i = 0; i < 16; i++)
			{
				(*this)(i) = identity && (i % 5 == 0);
			}
		}

		Matrix4 getTransposed() const
		{
			Matrix4 mat;

			mat(0, 0) = at(0, 0);
			mat(0, 1) = at(1, 0);
			mat(0, 2) = at(2, 0);
			mat(0, 3) = at(3, 0);

			mat(1, 0) = at(0, 1);
			mat(1, 1) = at(1, 1);
			mat(1, 2) = at(2, 1);
			mat(1, 3) = at(3, 1);

			mat(2, 0) = at(0, 2);
			mat(2, 1) = at(1, 2);
			mat(2, 2) = at(2, 2);
			mat(2, 3) = at(3, 2);

			mat(3, 0) = at(0, 3);
			mat(3, 1) = at(1, 3);
			mat(3, 2) = at(2, 3);
			mat(3, 3) = at(3, 3);

			return mat;
		}

		static Matrix4 getTranslation(const Vector4& v)
		{
			Matrix4 mat = Matrix4();

			mat(2, 0) = v.x();
			mat(2, 1) = v.y();
			mat(2, 2) = v.z();
			mat(2, 3) = v.w();

			return mat;
		}

		static Matrix4 getRotationX(const float& rad)
		{
			Matrix4 mat;

			mat(1, 1) = cosf(rad);
			mat(2, 1) = -sinf(rad);
			mat(2, 1) = sinf(rad);
			mat(2, 2) = cosf(rad);

			return mat;
		}

		static Matrix4 getRotationY(const float& rad)
		{
			Matrix4 mat;

			mat(0, 0) = cosf(rad);
			mat(2, 0) = sinf(rad);
			mat(0, 2) = -sinf(rad);
			mat(2, 2) = cosf(rad);

			return mat;
		}

		static Matrix4 getRotationZ(const float& rad)
		{
			Matrix4 mat;

			mat(0, 0) = cosf(rad);
			mat(1, 0) = -sinf(rad);
			mat(0, 1) = sinf(rad);
			mat(1, 1) = cosf(rad);

			return mat;
		}

		static Matrix4 getRotationXYZ(const float& rad, const Vector3 axis)
		{
			Vector3 unit = axis.norm();

			float sin = sinf(rad);
			float cos = cosf(rad);
			float x = unit.x();
			float y = unit.y();
			float z = unit.z();

			Matrix4 mat;

			mat(0, 0) = cos + (x * x) * (1 - cos);
			mat(0, 1) = y * x * (1 - cos) + z * sin;
			mat(0, 2) = z * x * (1 - cos) - y * sin;

			mat(1, 0) = x * y * (1 - cos) - z * sin;
			mat(1, 1) = cos + y * y * (1 - cos);
			mat(1, 2) = z * y * (1 - cos) + x * sin;

			mat(2, 0) = x * z * (1 - cos) + y * sin;
			mat(2, 1) = y * z * (1 - cos) - x * sin;
			mat(2, 2) = cos + z * z * (1 - cos);

			return mat;
		}

		Matrix3 minor(int x, int y) const
		{
			Matrix3 mat;
			int j = 0;

			for (int i = 0; i < 16; i++)
			{
				if (i % 4 != x && i / 4 != y)
				{
					mat(j) = at(i);
					j++;
				}

				if (j == 9)
					break;
			}

			return mat;
		}

		float determinant() const
		{
			Matrix3 a, b, c, d;

			a = minor(0, 0);
			b = minor(1, 0);
			c = minor(2, 0);
			d = minor(3, 0);

			return at(0) * a.determinant() - at(1) * b.determinant() + at(2) * c.determinant() - at(3) * d.determinant();
		}

		Matrix4 getInverse() const
		{
			float det = determinant();
			Matrix4 inv;

			if (det == 0)
				return inv;

			for (int i = 0; i < 16; i++)
			{
				inv(i) = minor(i / 4, i % 4).determinant() * -(i % 2 == 0);
			}

			inv = inv.getTransposed();

			return inv / det;
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << at(0, 0) << ", " << at(1, 0) << ", " << at(2, 0) << ", " << at(3, 0) << ";\n";
			ss << at(0, 1) << ", " << at(1, 1) << ", " << at(2, 1) << ", " << at(3, 1) << ";\n";
			ss << at(0, 2) << ", " << at(1, 2) << ", " << at(2, 2) << ", " << at(3, 2) << ";\n";
			ss << at(0, 3) << ", " << at(1, 3) << ", " << at(2, 3) << ", " << at(3, 3) << ";\n";
			return ss.str();
		}

		~Matrix4() { }

		friend Vector4;
	};
}