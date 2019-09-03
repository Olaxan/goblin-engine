#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

namespace std
{
	template <>
	struct hash<Assignment::Vector2>
	{
		size_t operator()(const Assignment::Vector2& v) const
		{
			// http://stackoverflow.com/a/1646913/126995
			size_t res = 17;
			res = res * 31 + hash<float>()(v.x());
			res = res * 31 + hash<float>()(v.y());
			return res;
		}
	};
}

namespace Assignment
{
	class Matrix2;
	class Vector2
	{
	private:
		float _arr[2];
	public:
		Vector2(const Vector2& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
		}

		Vector2(float x = 0, float y = 0)
		{
			this->x(x);
			this->y(y);
		}

		Vector2& operator += (const Vector2& other)
		{
			this->x(this->x() + other.x());
			this->y(this->y() + other.y());
			return *this;
		}

		Vector2& operator -= (const Vector2& other)
		{
			this->x(this->x() - other.x());
			this->y(this->y() - other.y());
			return *this;
		}

		Vector2& operator *= (const Vector2& other)
		{
			*this = (*this) * other;
			return *this;
		}

		Vector2& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		Vector2 operator + (const Vector2& other) const
		{
			Vector2 vect;
			vect.x(this->x() + other.x());
			vect.y(this->y() + other.y());
			return vect;
		}

		Vector2 operator - (const Vector2& other) const
		{
			Vector2 vect;
			vect.x(this->x() - other.x());
			vect.y(this->y() - other.y());
			return vect;
		}

		Vector2 operator * (const Vector2& other) const
		{
			Vector2 vect;
			vect.x(this->x() * other.x());
			vect.y(this->y() * other.y());
			return vect;
		}

		Vector2 operator * (const float& other) const
		{
			Vector2 vect;
			vect.x(this->x() * other);
			vect.y(this->y() * other);
			return vect;
		}

		Vector2& operator *= (const Matrix2& other)
		{
			other *= (*this);
			return *this;
		}

		bool operator == (const Vector2& other) const
		{
			return (std::hash<Vector2>()(*this) == std::hash<Vector2>()(other));
		}

		bool operator != (const Vector2& other) const
		{
			return (std::hash<Vector2>()(*this) != std::hash<Vector2>()(other));
		}

		float& operator [] (const int& index)
		{
			if (index > 2)
				throw new std::out_of_range("Vector index out of range");

			return _arr[index];
		}

		float at(int y) const
		{
			if (y > 1)
				throw new std::out_of_range("Vector index out of range");

			return _arr[y];
		}

		const float& x() const { return this->_arr[0]; }
		void x(const float& x) { this->_arr[0] = x; }

		const float& y() const { return _arr[1]; }
		void y(const float& y) { this->_arr[1] = y; }

		static float dist(const Vector2& x, const Vector2& y)
		{
			return (x - y).length();
		}

		static float dot(const Vector2& x, const Vector2& y)
		{
			return x.dot(y);
		}

		float dot(const Vector2& other) const
		{
			return (this->x() * other.x() + this->y() * other.y());
		}

		float length() const
		{
			return sqrt(powf(this->x(), 2) + powf(this->y(), 2));
		}

		Vector2 norm() const
		{
			Vector2 v = (*this);
			return v * (1 / v.length());
		}

		/// Performs a matrix transformation on the Vector2 and returns the result.
		Vector2 getTransformed(const Matrix2& trans) const
		{
			return Vector2(trans * (*this));
		}

		/// Calculates the reflection of a vector against a specified normal.
		Vector2 getReflection(const Vector2& normal) const
		{
			return (*this) - (normal - (*this)) * normal * 2;
		}

		std::string to_string() const
		{
			return std::to_string(this->x()) + ";\n" + std::to_string(this->y()) + ";\n";
		}

		~Vector2() { }
	};

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

		Matrix2 operator * (const Matrix2& other) const
		{
			Matrix2 mat;

			mat(0, 0) = Vector2::dot(row(0), other.col(0));
			mat(0, 1) = Vector2::dot(row(1), other.col(0));

			mat(1, 0) = Vector2::dot(row(0), other.col(1));
			mat(1, 1) = Vector2::dot(row(1), other.col(1));

			return mat;
		}

		Matrix2& operator = (const Matrix2& other)
		{
			this->a(other.a());
			this->b(other.b());
			this->c(other.c());
			this->d(other.d());

			return *this;
		}

		Vector2& operator *= (Vector2& other) const
		{
			other = (*this) * other;
			return other;
		}

		Vector2 operator * (const Vector2& other) const
		{
			Vector2 vect;
			vect.x(other.dot(row(0)));
			vect.y(other.dot(row(1)));
			return vect;
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