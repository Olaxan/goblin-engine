#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector3.h"

namespace efiilj
{

	/// <summary>
	/// Class to represent a 4-dimensional homogeneous vector.
	/// </summary>
	class Vector4
	{
	private:
		float _arr[4];

	public:

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a new 4D vector with the specified values.
		/// </summary>
		/// <param name="x">First value</param>
		/// <param name="y">Second value</param>
		/// <param name="z">Third value</param>
		/// <param name="w">Fourth value</param>
		Vector4(float x, float y, float z, float w)
		{
			this->x(x);
			this->y(y);
			this->z(z);
			this->w(w);
		}

		/// <summary>
		/// Creates a homogeneous 3D vector with the specified w value.
		/// </summary>
		/// <param name="w">The scaling value for the vector, default 1 (for normal cartesian coordinate systems)</param>
		Vector4(float w = 1) : Vector4(0, 0, 0, w) { }

		/// <summary>
		/// Inserts the specified Vector2 into the top of a new Vector4.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		Vector4(const Vector2& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
			this->z(0);
			this->w(0);
		}

		/// <summary>
		/// Inserts the specified Vector3 into the top of a new Vector4.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		Vector4(const Vector3& copy)
		{
			this->x(copy.x());
			this->y(copy.y());
			this->z(copy.z());
			this->w(1);
		}

		/// <summary>
		/// Constructs a copy of the specified vector.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		Vector4(const Vector4& copy)
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

		const float& w() const { return _arr[3]; }
		void w(const float& w) { this->_arr[3] = w; }

		/* === OPERATORS === */

		/// <summary>
		/// Adds two Vector4 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector4 resulting from the operation</returns>
		Vector4 operator + (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(this->x() + other.x());
			vect.y(this->y() + other.y());
			vect.z(this->z() + other.z());
			return vect;
		}

		/// <summary>
		/// Subtracts a Vector4 from this, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector4 resulting from the operation</returns>
		Vector4 operator - (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(this->x() - other.x());
			vect.y(this->y() - other.y());
			vect.z(this->z() - other.z());
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of two Vector4 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector4 resulting from the operation</returns>
		Vector4 operator * (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(this->x() * other.x());
			vect.y(this->y() * other.y());
			vect.z(this->z() * other.z());
			vect.w(this->w() * other.w());
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector4 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector4 resulting from the operation</returns>
		Vector4 operator * (const float& other) const
		{
			Vector4 vect;
			vect.x(this->x() * other);
			vect.y(this->y() * other);
			vect.z(this->z() * other);
			vect.w(this->w() * other);
			return vect;
		}

		/// <summary>
		/// Divides the individual values of this Vector4 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector4 resulting from the operation</returns>
		Vector4 operator / (const float& other) const
		{
			Vector4 v = *this;

			return v * (1 / other);
		}

		/// <summary>
		/// Tests two Vector4 instances for equality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if equal, false otherwise</returns>
		bool operator == (const Vector4& other) const
		{
			return x() == other.x() && y() == other.y() && z() == other.z() && w() == other.w();
		}

		/// <summary>
		/// Tests two Vector4 instances for inequality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if not equal, false otherwise</returns>
		bool operator != (const Vector4& other) const
		{
			return x() != other.x() || y() != other.y() || z() != other.z() || w() != other.w();
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Adds another Vector4 to this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		Vector4& operator += (const Vector4& other)
		{
			(*this) = (*this) + other;
			return *this;
		}

		/// <summary>
		/// Subtracts another Vector4 from this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		Vector4& operator -= (const Vector4& other)
		{
			(*this) = (*this) - other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector4 with another, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		Vector4& operator *= (const Vector4& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector4 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		Vector4& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Divides the individual values of this Vector4 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		Vector4& operator /= (const float& other)
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
			if (index > 3)
				throw new std::out_of_range("Vector index out of range");

			return _arr[index];
		}

		/// <summary>
		/// Returns the (const) value at the n:th vector index, from top to bottom.
		/// </summary>
		/// <param name="n">The vector index to access</param>
		/// <returns>The value at the specified position in the vector</returns>
		float at(int y) const
		{
			if (y > 3)
				throw new std::out_of_range("Vector index out of range");

			return _arr[y];
		}

		/* === VECTOR FUNCTIONS === */

		/// <summary>
		/// Calculates the 3D distance between two vectors (points).
		/// The fourth coordinate (w) is not taken into account.
		/// </summary>
		/// <param name="x">The first point</param>
		/// <param name="y">The second point</param>
		/// <returns>The distance between points as a float</returns>
		static float dist(const Vector4& x, const Vector4& y)
		{
			return (x - y).length();
		}

		/// <summary>
		/// Calculates the 3D dot product between two vectors.
		/// The fourth coordinate (w) is not taken into account.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The dot product as a float</returns>
		static float dot(const Vector4& x, const Vector4& y)
		{
			return x.dot(y);
		}

		/// <summary>
		/// Calculates the 4D dot product between two vectors.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The dot product as a float</returns>
		static float dot4(const Vector4& x, const Vector4& y)
		{
			return x.dot4(y);
		}

		/// <summary>
		/// Calculates the 3D cross product between two vectors.
		/// The fourth coordinate (w) is not taken into account.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The cross product as a float</returns>
		static Vector4 cross(const Vector4& x, const Vector4& y)
		{
			return x.cross(y);
		}

		/// <summary>
		/// Calculates the 3D dot product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate dot product with</param>
		/// <returns>The dot product as a float</returns>
		float dot(const Vector4& other) const
		{
			return (this->x() * other.x() + this->y() * other.y() + this->z() * other.z());
		}

		/// <summary>
		/// Calculates the 4D dot product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate dot product with</param>
		/// <returns>The dot product as a float</returns>
		float dot4(const Vector4& other) const
		{
			return dot(other) + w() * other.w();
		}

		/// <summary>
		/// Calculates the 3D cross product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate cross product with</param>
		/// <returns>The cross product as a float</returns>
		Vector4 cross(const Vector4& other) const
		{
			Vector4 vect;
			vect.x(y() * other.z() - z() * other.y());
			vect.y(z() * other.x() - x() * other.z());
			vect.z(x() * other.y() - y() * other.x());
			vect.w(0);
			return vect;
		}

		/// <summary>
		/// Returns the 3D length of this point from the origin (0, 0, 0).
		/// The fourth coordinate (w) is not taken into account.
		/// </summary>
		/// <returns>The vector length as a float</returns>
		float length() const
		{
			return sqrt(powf(this->x(), 2) + powf(this->y(), 2) + powf(this->z(), 2));
		}

		/// <summary>
		/// Normalizes the vector, turning it into a unit vector.
		/// </summary>
		/// <returns>The resulting unit vector, or returns as-received if the vector length is already zero</returns>
		Vector4 norm() const
		{
			Vector4 v = (*this);
			float len = v.length();

			return len > 0 ? v * (1 / v.length()) : v;
		}

		/* === FACTORY FUNCTIONS === */

		/// <summary>
		/// Calculates the reflection of a vector against a specified normal.
		/// The fourth coordinate (w) is not taken into account.
		/// </summary>
		/// <param name="normal">The normal to calculate reflection against</param>
		/// <returns>The resulting reflection vector</returns>
		Vector4 getReflection(const Vector4& normal) const
		{
			Vector4 unit = normal * (1 / normal.length());
			return (*this) - unit * 2 * dot(unit);
		}

		/* === UTILITIES === */

		/// <summary>
		/// Returns a formatted multi-line string representation of the vector.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the vector</returns>
		std::string to_string() const
		{
			std::stringstream ss;
			ss << x() << ";\n" << y() << ";\n" << z() << ";\n" << w() << ";\n";
			return ss.str();
		}

		~Vector4() { }
	};
}