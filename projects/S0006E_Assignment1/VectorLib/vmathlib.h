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

	template <>
	struct hash<Assignment::Vector3>
	{
		size_t operator()(const Assignment::Vector3& v) const
		{
			// http://stackoverflow.com/a/1646913/126995
			size_t res = 17;
			res = res * 31 + hash<float>()(v.x());
			res = res * 31 + hash<float>()(v.y());
			res = res * 31 + hash<float>()(v.z());
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

	class Matrix3;
	class Vector3
	{
	private:
		float _arr[3];

	public:
		Vector3(float x = 0, float y = 0, float z = 0)
		{
			this->x(x);
			this->y(y);
			this->z(z);
		}

		Vector3(bool homogenous) : Vector3(0, 0, homogenous) { }

		Vector3(const Vector2& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
			this->z(0);
		}

		Vector3(const Vector3& copy)
		{
			*this = copy;
		}

		Vector3& operator += (const Vector3& other)
		{
			(*this) = (*this) + other;
			return *this;
		}

		Vector3& operator -= (const Vector3& other)
		{
			(*this) = (*this) - other;
			return *this;
		}

		Vector3& operator *= (const Vector3& other)
		{
			*this = (*this) * other;
			return *this;
		}

		Vector3& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		Vector3 operator + (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(this->x() + other.x());
			vect.y(this->y() + other.y());
			vect.z(this->z() + other.z());
			return vect;
		}

		Vector3 operator - (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(this->x() - other.x());
			vect.y(this->y() - other.y());
			vect.z(this->z() - other.z());
			return vect;
		}

		Vector3 operator * (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(this->x() * other.x());
			vect.y(this->y() * other.y());
			vect.z(this->z() * other.z());
			return vect;
		}

		Vector3 operator * (const float& other) const
		{
			Vector3 vect;
			vect.x(this->x() * other);
			vect.y(this->y() * other);
			vect.z(this->z() * other);
			return vect;
		}

		Vector3& operator *= (const Matrix3& other)
		{
			(*this) = other * (*this);
			return *this;
		}

		bool operator == (const Vector3& other) const
		{
			return (std::hash<Vector3>()(*this) == std::hash<Vector3>()(other));
		}

		bool operator != (const Vector3& other) const
		{
			return (std::hash<Vector3>()(*this) != std::hash<Vector3>()(other));
		}

		float& operator [] (const int& index)
		{
			if (index > 2)
				throw new std::out_of_range("Vector index out of range");

			return _arr[index];
		}

		float at(int y) const
		{
			if (y > 2)
				throw new std::out_of_range("Vector index out of range");

			return _arr[y];
		}

		const float& x() const { return this->_arr[0]; }
		void x(const float& x) { this->_arr[0] = x; }

		const float& y() const { return _arr[1]; }
		void y(const float& y) { this->_arr[1] = y; }

		const float& z() const { return _arr[2]; }
		void z(const float& z) { this->_arr[2] = z; }

		static float dist(const Vector3& x, const Vector3& y)
		{
			return (x - y).length();
		}

		static float dot(const Vector3& x, const Vector3& y)
		{
			return x.dot(y);
		}

		float dot(const Vector3& other) const
		{
			return (this->x() * other.x() + this->y() * other.y() + this->z() * other.z());
		}

		float length() const
		{
			return sqrt(powf(this->x(), 2) + powf(this->y(), 2) + powf(this->z(), 2));
		}

		Vector3 norm() const
		{
			Vector3 v = (*this);
			return v * (1 / v.length());
		}

		Vector3 getTransformed(const Matrix3& trans) const
		{
			return Vector3(trans * (*this));
		}

		Vector3 getReflection(const Vector3& normal) const
		{
			Vector3 unit = normal * (1 / normal.length());
			return (*this) - unit * 2 * dot(unit);
		}

		std::string to_string() const
		{
			return std::to_string(this->x()) + ";\n" + std::to_string(this->y()) + ";\n" + std::to_string(this->z()) + ";\n";
		}

		~Vector3() { }
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

			mat(0, 0) = Vector3::dot(row(0), other.col(0));;
			mat(0, 1) = Vector3::dot(row(1), other.col(0));;

			mat(1, 0) = Vector3::dot(row(0), other.col(1));;
			mat(1, 1) = Vector3::dot(row(1), other.col(1));;

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

		std::string to_string()
		{
			std::stringstream ss;
			ss << a() << ", " << b() << ";\n" << c() << ", " << d() << ";\n";
			return ss.str();
		}

		~Matrix2() { }

		friend Vector2;
	};

	class Matrix3
	{
	private:
		Vector3 _arr[3];

	public:
		Matrix3()
		{
			clear();
		}

		Matrix3(const Matrix2& copy)
		{
			clear();

			(*this)(0, 0) = copy.a();
			(*this)(1, 0) = copy.b();
			(*this)(0, 1) = copy.c();
			(*this)(1, 1) = copy.d();
		}

		Matrix3(const Matrix3& copy)
		{
			*this = copy;
		}

		Matrix3(Vector3& x, Vector3& y, Vector3& z)
		{
			_arr[0] = x;
			_arr[1] = y;
			_arr[2] = z;
		}

		Matrix3 operator * (const Matrix3& other) const
		{
			Matrix3 mat;

			mat(0, 0) = Vector3::dot(row(0), other.col(0));
			mat(0, 1) = Vector3::dot(row(1), other.col(0));
			mat(0, 2) = Vector3::dot(row(2), other.col(0));

			mat(1, 0) = Vector3::dot(row(0), other.col(1));
			mat(1, 1) = Vector3::dot(row(1), other.col(1));
			mat(1, 2) = Vector3::dot(row(2), other.col(1));

			mat(2, 0) = Vector3::dot(row(0), other.col(2));
			mat(2, 1) = Vector3::dot(row(1), other.col(2));
			mat(2, 2) = Vector3::dot(row(2), other.col(2));

			return mat;
		}

		Matrix3& operator = (const Matrix3& other)
		{
			(*this)(0, 0) = other.at(0, 0);
			(*this)(0, 1) = other.at(0, 1);
			(*this)(0, 2) = other.at(0, 2);

			(*this)(1, 0) = other.at(1, 0);
			(*this)(1, 1) = other.at(1, 1);
			(*this)(1, 2) = other.at(1, 2);

			(*this)(2, 0) = other.at(2, 0);
			(*this)(2, 1) = other.at(2, 1);
			(*this)(2, 2) = other.at(2, 2);

			return *this;
		}

		Vector3& operator *= (Vector3& other) const
		{
			other = (*this) * other;
			return other;
		}

		Vector3 operator * (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(other.dot(row(0)));
			vect.y(other.dot(row(1)));
			vect.z(other.dot(row(2)));
			return vect;
		}

		float& operator () (int i)
		{
			return _arr[i / 3][i % 3];
		}

		float& operator () (int x, int y)
		{
			return _arr[y][x];
		}

		float at(int n) const
		{
			return at(n / 3, n % 3);
		}

		float at(int x, int y) const
		{
			return _arr[y].at(x);
		}

		Vector3 row(int y) const
		{
			return Vector3(at(0, y), at(1, y), at(2, y));
		}

		Vector3 col(int x) const
		{
			return Vector3(at(x, 0), at(x, 1), at(x, 2));
		}

		void clear(bool identity = true)
		{
			for (int i = 0; i < 9; i++)
			{
				(*this)(i) = identity && (i % 4 == 0);
			}
		}

		Matrix3 getTransposed() const
		{
			Matrix3 mat;

			mat(0, 0) = at(0, 0);
			mat(0, 1) = at(1, 0);
			mat(0, 2) = at(2, 0);

			mat(1, 0) = at(0, 1);
			mat(1, 1) = at(1, 1);
			mat(1, 2) = at(2, 1);

			mat(2, 0) = at(0, 2);
			mat(2, 1) = at(1, 2);
			mat(2, 2) = at(2, 2);

			return mat;
		}

		static Matrix3 getTranslation(const Vector3& v)
		{
			Matrix3 mat = Matrix3();
			mat(2, 0) = v.x();
			mat(2, 1) = v.y();
			mat(2, 2) = v.z();
			return mat;
		}

		static Matrix3 getRotationMatrix(const float& rad)
		{
			Matrix2 mat = Matrix2::getRotationMatrix(rad);
			return Matrix3(mat);
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << at(0, 0) << ", " << at(1, 0) << ", " << at(2, 0) << ";\n";
			ss << at(0, 1) << ", " << at(1, 1) << ", " << at(2, 1) << ";\n";
			ss << at(0, 2) << ", " << at(1, 2) << ", " << at(2, 2) << ";\n";
			return ss.str();
		}

		~Matrix3() { }

		friend Vector3;
	};



}

