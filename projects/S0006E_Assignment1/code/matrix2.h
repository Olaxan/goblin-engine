#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector2.h"

namespace Assignment
{

	class Matrix2
	{
	private:
		Vector2 _arr[2];
	public:
		Matrix2(const Matrix2& copy)
		{
			*this = copy;
		}

		Matrix2(float a = 0, float b = 0, float c = 0, float d = 0)
		{
			this->a(a);
			this->b(b);
			this->c(c);
			this->d(d);
		}

		Matrix2& operator = (const Matrix2& other)
		{
			this->a(other.a());
			this->b(other.b());
			this->c(other.c());
			this->d(other.d());

			return *this;
		}

		Matrix2 operator * (const Matrix2& other) const
		{
			Matrix2 mat;

			mat(0, 0) = Vector2::dot(row(0), other.col(0));
			mat(0, 1) = Vector2::dot(row(1), other.col(0));

			mat(1, 0) = Vector2::dot(row(0), other.col(1));
			mat(1, 1) = Vector2::dot(row(1), other.col(1));

			return mat;
		}

		Matrix2 operator * (const float& other) const
		{
			Matrix2 mat = *this;

			for (int i = 0; i < 4; i++)
			{
				mat(i) *= other;
			}

			return mat;
		}

		Vector2 operator * (const Vector2& other) const
		{
			Vector2 vect;
			vect.x(other.dot(row(0)));
			vect.y(other.dot(row(1)));
			return vect;
		}

		Matrix2 operator / (const float& other) const
		{
			Matrix2 mat = *this;

			return mat * (1 / other);
		}

		Vector2& operator *= (Vector2& other) const
		{
			other = (*this) * other;
			return other;
		}

		float& operator () (int n)
		{
			return _arr[n / 2][n % 2];
		}

		float& operator () (int x, int y)
		{
			return _arr[y][x];
		}

		float at(int n) const
		{
			return at(n / 2, n % 2);
		}

		float at(int x, int y) const
		{
			return _arr[y].at(x);
		}

		Vector2 row(int y) const
		{
			return Vector2(at(0, y), at(1, y));
		}

		Vector2 col(int x) const
		{
			return Vector2(at(x, 0), at(x, 1));
		}

		void clear(bool identity = true)
		{
			for (int i = 0; i < 4; i++)
			{
				_arr[i] = identity && (i % 3 == 0);
			}
		}

		const float& a() const { return this->_arr[0].x(); }
		void a(const float& a) { this->_arr[0].x(a); }

		const float& b() const { return this->_arr[0].y(); }
		void b(const float& b) { this->_arr[0].y(b); }

		const float& c() const { return this->_arr[1].x(); }
		void c(const float& c) { this->_arr[1].x(c); }

		const float& d() const { return this->_arr[1].y(); }
		void d(const float& d) { this->_arr[1].y(d); }

		Matrix2 getTransposed() const
		{
			return Matrix2(this->a(), this->c(), this->b(), this->d());
		}

		static Matrix2 getRotationMatrix(const float& rad)
		{
			return Matrix2(cos(rad), -sin(rad), sin(rad), cos(rad));
		}

		float determinant() const
		{
			return a() * d() - b() * c();
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << a() << ", " << b() << ";\n" << c() << ", " << d() << ";\n";
			return ss.str();
		}

		~Matrix2() { }

		friend Vector2;
	};
}