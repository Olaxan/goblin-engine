#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector2.h"

namespace efiilj
{

	/// <summary>
	/// Class to represent a 3-dimensional point vector.
	/// </summary>
	class Vector3
	{
	private:
		float _arr[3];

	public:

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a new 3D vector with the specified values.
		/// </summary>
		/// <param name="x">Top value</param>
		/// <param name="y">Middle value</param>
		/// <param name="z">Bottom value</param>
		Vector3(float x, float y, float z)
		{
			this->x(x);
			this->y(y);
			this->z(z);
		}

		/// <summary>
		/// Creates a homogeneous 2D vector with the specified w value.
		/// </summary>
		/// <param name="w">The scaling value for the vector, default 1 (for normal cartesian coordinate systems)</param>
		Vector3(float w = 1) : Vector3(0, 0, w) { }

		/// <summary>
		/// Inserts the specified Vector2 into the top of a new Vector3.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		Vector3(const Vector2& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
			this->z(0);
		}

		/// <summary>
		/// Constructs a copy of the specified vector.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		Vector3(const Vector3& copy)
		{
			*this = copy;
		}

		/* === ACCESSORS === */

		const float& x() const { return this->_arr[0]; }
		void x(const float& x) { this->_arr[0] = x; }

		const float& y() const { return _arr[1]; }
		void y(const float& y) { this->_arr[1] = y; }

		const float& z() const { return _arr[2]; }
		void z(const float& z) { this->_arr[2] = z; }

		/* === OPERATORS === */

		/// <summary>
		/// Adds two Vector3 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		Vector3 operator + (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(this->x() + other.x());
			vect.y(this->y() + other.y());
			vect.z(this->z() + other.z());
			return vect;
		}

		/// <summary>
		/// Subtracts a Vector3 from this, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		Vector3 operator - (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(this->x() - other.x());
			vect.y(this->y() - other.y());
			vect.z(this->z() - other.z());
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of two Vector3 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		Vector3 operator * (const Vector3& other) const
		{
			Vector3 vect;
			vect.x(this->x() * other.x());
			vect.y(this->y() * other.y());
			vect.z(this->z() * other.z());
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector3 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		Vector3 operator * (const float& other) const
		{
			Vector3 vect;
			vect.x(this->x() * other);
			vect.y(this->y() * other);
			vect.z(this->z() * other);
			return vect;
		}

		/// <summary>
		/// Divides the individual values of this Vector3 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		Vector3 operator / (const float& other) const
		{
			Vector3 v = *this;

			return v * (1 / other);
		}

		/// <summary>
		/// Tests two Vector3 instances for equality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if equal, false otherwise</returns>
		bool operator == (const Vector3& other) const
		{
			return x() == other.x() && y() == other.y() && z() == other.z();
		}

		/// <summary>
		/// Tests two Vector3 instances for inequality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if not equal, false otherwise</returns>
		bool operator != (const Vector3& other) const
		{
			return x() != other.x() || y() != other.y() || z() != other.z();
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Adds another Vector3 to this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		Vector3& operator += (const Vector3& other)
		{
			(*this) = (*this) + other;
			return *this;
		}

		/// <summary>
		/// Subtracts another Vector3 from this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		Vector3& operator -= (const Vector3& other)
		{
			(*this) = (*this) - other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector3 with another, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		Vector3& operator *= (const Vector3& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector3 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		Vector3& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Divides the individual values of this Vector3 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		Vector3& operator /= (const float& other)
		{
			*this = (*this) / other;
			return *this;
		}

		/* === ACCESSORS === */

		/// <summary>
		/// Accesses a value at the specified index in the vector, from top to bottom.
		/// </summary>
		/// <param name="index">The index to access</param>
		/// <returns>The value at the specified position in the vector</returns>
		float& operator [] (const int& index)
		{
			if (index > 2)
				throw new std::out_of_range("Vector index out of range");

			return _arr[index];
		}

		/// <summary>
		/// Returns the (const) value at the n:th vector index, from top to bottom.
		/// </summary>
		/// <param name="n">The vector index to access</param>
		/// <returns>The value at the specified position in the vector</returns>
		const float& at(int y) const
		{
			if (y > 2)
				throw new std::out_of_range("Vector index out of range");

			return _arr[y];
		}

		/* === VECTOR FUNCTIONS === */

		/// <summary>
		/// Calculates the distance between two vectors (points).
		/// </summary>
		/// <param name="x">The first point</param>
		/// <param name="y">The second point</param>
		/// <returns>The distance between points as a float</returns>
		static float dist(const Vector3& x, const Vector3& y)
		{
			return (x - y).length();
		}

		/// <summary>
		/// Calculates the dot product between two vectors.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The dot product as a float</returns>
		static float dot(const Vector3& x, const Vector3& y)
		{
			return x.dot(y);
		}

		/// <summary>
		/// Calculates the cross product between two vectors.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The cross product as a float</returns>
		static Vector3 cross(const Vector3& x, const Vector3& y)
		{
			return x.cross(y);
		}

		/// <summary>
		/// Calculates the dot product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate dot product with</param>
		/// <returns>The dot product as a float</returns>
		float dot(const Vector3& other) const
		{
			return (this->x() * other.x() + this->y() * other.y() + this->z() * other.z());
		}

		/// <summary>
		/// Calculates the cross product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate cross product with</param>
		/// <returns>The cross product as a float</returns>
		Vector3 cross(const Vector3& other) const
		{
			Vector3 vect;

			vect.x(y() * other.z() - z() * other.y());
			vect.y(z() * other.x() - x() * other.z());
			vect.z(x() * other.y() - y() * other.x());

			return vect;
		}

		/// <summary>
		/// Returns the length of this point from the origin (0, 0, 0).
		/// </summary>
		/// <returns>The vector length as a float</returns>
		float length() const
		{
			return sqrt(powf(this->x(), 2) + powf(this->y(), 2) + powf(this->z(), 2));
		}

		/// <summary>
		/// Normalizes the vector, turning it into a unit vector.
		/// </summary>
		/// <returns>The resulting unit vector</returns>
		Vector3 norm() const
		{
			Vector3 v = (*this);
			float len = v.length();

			return len > 0 ? v * (1 / v.length()) : v;
		}

		/* === FACTORY FUNCTIONS === */

		/// <summary>
		/// Calculates the reflection of a vector against a specified normal.
		/// </summary>
		/// <param name="normal">The normal to calculate reflection against</param>
		/// <returns>The resulting reflection vector</returns>
		Vector3 getReflection(const Vector3& normal) const
		{
			Vector3 unit = normal * (1 / normal.length());
			return (*this) - unit * 2 * dot(unit);
		}

		/* === UTILITIES === */

		/// <summary>
		/// Returns a formatted multi-line string representation of the vector.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the vector</returns>
		std::string to_string() const
		{
			return std::to_string(this->x()) + ";\n" + std::to_string(this->y()) + ";\n" + std::to_string(this->z()) + ";\n";
		}

		~Vector3() { }
	};
}