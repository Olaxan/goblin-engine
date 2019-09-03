#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

namespace Assignment
{
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

		Vector2 operator / (const float& other) const
		{
			Vector2 vect = *this;

			return vect * (1 / other);
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

		Vector2& operator /= (const float& other)
		{
			*this = (*this) / other;
			return *this;
		}

		//Vector2& operator *= (const Matrix2& other)
		//{
		//	other *= (*this);
		//	return *this;
		//}

		bool operator == (const Vector2& other) const
		{
			return x() == other.x() && y() == other.y();
		}

		bool operator != (const Vector2& other) const
		{
			return x() != other.x() || y() != other.y();
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
		/*Vector2 getTransformed(const Matrix2& trans) const
		{
			return Vector2(trans * (*this));
		}*/

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
}