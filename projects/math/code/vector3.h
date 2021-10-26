#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector2.h"

#define EPSILON 0.00001f

namespace efiilj
{

	/// <summary>
	/// Class to represent a 3-dimensional point vector.
	/// </summary>
	class vector3
	{
	private:

	public:

		union
		{
			struct { float x, y, z; };	
			float arr_[3];
		};

		/* === CONSTRUCTORS === */


		/// <summary>
		/// Creates a new 3D vector with the specified values.
		/// </summary>
		/// <param name="x">Top value</param>
		/// <param name="y">Middle value</param>
		/// <param name="z">Bottom value</param>
		vector3(float x, float y = 0.0f, float z = 0.0f)
			: x(arr_[0]), y(arr_[1]), z(arr_[2])
		{ 
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/// <summary>
		/// Inserts the specified Vector2 into the top of a new Vector3.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		vector3(const vector2& copy, float z = 0.0f)
			: vector3(copy.x, copy.y, z)
		{ }

		/// <summary>
		/// Constructs a copy of the specified vector.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		vector3(const vector3& copy)
			: vector3(copy.x, copy.y, copy.z)
		{ }

		vector3()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		vector3(vector3&& move) noexcept
		{
			*this = std::move(move);
		}

		/* === OPERATORS === */

		void operator = (const vector3& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		void operator = (const vector3&& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		/// <summary>
		/// Adds two Vector3 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		vector3 operator + (const vector3& other) const
		{
			vector3 vect;
			vect.x = this->x + other.x;
			vect.y = this->y + other.y;
			vect.z = this->z + other.z;
			return vect;
		}

		/// <summary>
		/// Subtracts a Vector3 from this, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		vector3 operator - (const vector3& other) const
		{
			vector3 vect;
			vect.x = this->x - other.x;
			vect.y = this->y - other.y;
			vect.z = this->z - other.z;
			return vect;
		}

		vector3 operator - () const
		{
			return *this * -1.0f;
		}

		/// <summary>
		/// Multiplies the individual values of two Vector3 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		vector3 operator * (const vector3& other) const
		{
			vector3 vect;
			vect.x = this->x * other.x;
			vect.y = this->y * other.y;
			vect.z = this->z * other.z;
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector3 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		vector3 operator * (const float& other) const
		{
			vector3 vect;
			vect.x = this->x * other;
			vect.y = this->y * other;
			vect.z = this->z * other;
			return vect;
		}

		/// <summary>
		/// Divides the individual values of this Vector3 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		vector3 operator / (const float& other) const
		{
			return *this * (1.0f / other);
		}

		/// <summary>
		/// Tests two Vector3 instances for equality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if equal, false otherwise</returns>
		bool operator == (const vector3& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		/// <summary>
		/// Tests two Vector3 instances for inequality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if not equal, false otherwise</returns>
		bool operator != (const vector3& other) const
		{
			return x != other.x || y != other.y || z != other.z;
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Adds another Vector3 to this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		vector3& operator += (const vector3& other)
		{
			(*this) = (*this) + other;
			return *this;
		}

		/// <summary>
		/// Subtracts another Vector3 from this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		vector3& operator -= (const vector3& other)
		{
			(*this) = (*this) - other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector3 with another, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		vector3& operator *= (const vector3& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector3 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		vector3& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Divides the individual values of this Vector3 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		vector3& operator /= (const float& other)
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

			return arr_[index];
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

			return arr_[y];
		}

		/* === VECTOR FUNCTIONS === */

		/// <summary>
		/// Calculates the distance between two vectors (points).
		/// </summary>
		/// <param name="x">The first point</param>
		/// <param name="y">The second point</param>
		/// <returns>The distance between points as a float</returns>
		static float dist(const vector3& x, const vector3& y)
		{
			return (x - y).length();
		}

		/// <summary>
		/// Calculates the dot product between two vectors.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The dot product as a float</returns>
		static float dot(const vector3& a, const vector3& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		}

		/// <summary>
		/// Calculates the cross product between two vectors.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The cross product as a float</returns>
		static vector3 cross(const vector3& a, const vector3& b)
		{
			vector3 vect;
			vect.x = a.y * b.z - a.z * b.y;
			vect.y = a.z * b.x - a.x * b.z;
			vect.z = a.x * b.y - a.y * b.x;
			return vect;
		}

		static vector3 min(const vector3& a, const vector3& b)
		{
			return vector3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
		}

		static vector3 max(const vector3& a, const vector3& b)
		{
			return vector3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
		}

		/// <summary>
		/// Calculates the dot product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate dot product with</param>
		/// <returns>The dot product as a float</returns>
		float dot(const vector3& other) const
		{
			return vector3::dot(*this, other);
		}

		/// <summary>
		/// Calculates the cross product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate cross product with</param>
		/// <returns>The cross product as a float</returns>
		vector3 cross(const vector3& other) const
		{
			return vector3::cross(*this, other);
		}

		/// <summary>
		/// Returns the length of this point from the origin (0, 0, 0).
		/// </summary>
		/// <returns>The vector length as a float</returns>
		float length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		float magnitude() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		float square_magnitude() const
		{
			return x * x + y * y + z * z;
		}

		/// <summary>
		/// Normalizes the vector, turning it into a unit vector.
		/// </summary>
		/// <returns>The resulting unit vector</returns>
		vector3 norm() const
		{
			vector3 v = (*this);
			float len = v.length();

			return len > 0 ? v / len : v;
		}

		bool is_zero(float epsilon = EPSILON) const
		{
			return fabs(x) < epsilon 
				&& fabs(y) < epsilon 
				&& fabs(z) < epsilon;
		}

		/* === FACTORY FUNCTIONS === */

		/// <summary>
		/// Calculates the reflection of a vector against a specified normal.
		/// </summary>
		/// <param name="normal">The normal to calculate reflection against</param>
		/// <returns>The resulting reflection vector</returns>
		vector3 getReflection(const vector3& normal) const
		{
			vector3 unit = normal * (1 / normal.length());
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
			ss << x << ", " << y << ", " << z;
			return ss.str();
		}
		
		~vector3() { }
	};
	
	extern vector3 operator * (float lhs, const vector3& rhs);
}

#undef EPSILON
