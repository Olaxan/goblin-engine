#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector3.h"

namespace Assignment
{
	class Vector4
	{
	private:
		float _arr[4];

	public:
		Vector4(float x, float y, float z, float w)
		{
			this->x(x);
			this->y(y);
			this->z(z);
			this->w(w);
		}

		Vector4(float w = 1) : Vector4(0, 0, 0, w) { }

		Vector4(const Vector2& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
			this->z(0);
			this->w(1);
		}

		Vector4(const Vector3& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
			this->z(copy.z());
			this->w(1);
		}

		Vector4(const Vector4& copy)
		{
			*this = copy;
		}

		Vector4 operator + (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(this->x() + other.x());
			vect.y(this->y() + other.y());
			vect.z(this->z() + other.z());
			return vect;
		}

		Vector4 operator - (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(this->x() - other.x());
			vect.y(this->y() - other.y());
			vect.z(this->z() - other.z());
			return vect;
		}

		Vector4 operator * (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(this->x() * other.x());
			vect.y(this->y() * other.y());
			vect.z(this->z() * other.z());
			return vect;
		}

		Vector4 operator * (const float& other) const
		{
			Vector4 vect;
			vect.x(this->x() * other);
			vect.y(this->y() * other);
			vect.z(this->z() * other);
			return vect;
		}

		Vector4 operator / (const float& other) const
		{
			Vector4 v = *this;

			return v * (1 / other);
		}

		Vector4& operator += (const Vector4& other)
		{
			(*this) = (*this) + other;
			return *this;
		}

		Vector4& operator -= (const Vector4& other)
		{
			(*this) = (*this) - other;
			return *this;
		}

		Vector4& operator *= (const Vector4& other)
		{
			*this = (*this) * other;
			return *this;
		}

		Vector4& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		Vector4& operator /= (const float& other)
		{
			*this = (*this) / other;
			return *this;
		}

		//Vector4& operator *= (const Matrix4& other)
		//{
		//	(*this) = other * (*this);
		//	return *this;
		//}

		bool operator == (const Vector4& other) const
		{
			return x() == other.x() && y() == other.y() && z() == other.z() && w() == other.w();
		}

		bool operator != (const Vector4& other) const
		{
			return x() != other.x() || y() != other.y() || z() != other.z() || w() != other.w();
		}

		float& operator [] (const int& index)
		{
			if (index > 3)
				throw new std::out_of_range("Vector index out of range");

			return _arr[index];
		}

		float at(int y) const
		{
			if (y > 3)
				throw new std::out_of_range("Vector index out of range");

			return _arr[y];
		}

		const float& x() const { return this->_arr[0]; }
		void x(const float& x) { this->_arr[0] = x; }

		const float& y() const { return _arr[1]; }
		void y(const float& y) { this->_arr[1] = y; }

		const float& z() const { return _arr[2]; }
		void z(const float& z) { this->_arr[2] = z; }

		const float& w() const { return _arr[3]; }
		void w(const float& w) { this->_arr[3] = w; }

		static float dist(const Vector4& x, const Vector4& y)
		{
			return (x - y).length();
		}

		static float dot(const Vector4& x, const Vector4& y)
		{
			return x.dot(y);
		}

		static float dot4(const Vector4& x, const Vector4& y)
		{
			return x.dot4(y);
		}

		static Vector4 cross(const Vector4& x, const Vector4& y)
		{
			return x.cross(y);
		}

		float dot(const Vector4& other) const
		{
			return (this->x() * other.x() + this->y() * other.y() + this->z() * other.z());
		}

		float dot4(const Vector4& other) const
		{
			return dot(other) + w() * other.w();
		}

		Vector4 cross(const Vector4& other) const
		{
			Vector4 vect;
			vect.x(y() * other.z() - z() * other.y());
			vect.y(z() * other.x() - x() * other.z());
			vect.z(x() * other.y() - y() * other.x());
			return vect;
		}

		float length() const
		{
			return sqrt(powf(this->x(), 2) + powf(this->y(), 2) + powf(this->z(), 2));
		}

		Vector4 norm() const
		{
			Vector4 v = (*this);
			return v * (1 / v.length());
		}

		/*Vector4 getTransformed(const Matrix4& trans) const
		{
			return Vector4(trans * (*this));
		}*/

		Vector4 getReflection(const Vector4& normal) const
		{
			Vector4 unit = normal * (1 / normal.length());
			return (*this) - unit * 2 * dot(unit);
		}

		std::string to_string() const
		{
			std::stringstream ss;
			ss << x() << ";\n" << y() << ";\n" << z() << ";\n" << w() << ";\n";
			return ss.str();
		}

		~Vector4() { }
	};
}