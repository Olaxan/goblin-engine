#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector2.h"

namespace Assignment
{
	class Vector3
	{
	private:
		float _arr[3];

	public:
		Vector3(float x, float y, float z)
		{
			this->x(x);
			this->y(y);
			this->z(z);
		}

		Vector3(bool homogenous = false) : Vector3(0, 0, homogenous) { }

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

		Vector3 operator / (const float& other) const
		{
			Vector3 v = *this;

			return v * (1 / other);
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

		Vector3& operator /= (const float& other)
		{
			*this = (*this) / other;
			return *this;
		}

		//Vector3& operator *= (const Matrix3& other)
		//{
		//	(*this) = other * (*this);
		//	return *this;
		//}

		bool operator == (const Vector3& other) const
		{
			return x() == other.x() && y() == other.y() && z() == other.z();
		}

		bool operator != (const Vector3& other) const
		{
			return x() != other.x() || y() != other.y() || z() != other.z();
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

		static Vector3 cross(const Vector3& x, const Vector3& y)
		{
			return x.cross(y);
		}

		Vector3 cross(const Vector3& other) const
		{
			Vector3 vect;

			vect.x(y() * other.z() - z() * other.y());
			vect.y(z() * other.x() - x() * other.z());
			vect.z(x() * other.y() - y() * other.x());

			return vect;
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

		/*Vector3 getTransformed(const Matrix3& trans) const
		{
			return Vector3(trans * (*this));
		}*/

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
}