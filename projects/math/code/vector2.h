#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

namespace efiilj
{

	/// <summary>
	/// Class to represent a 2-dimensional point vector.
	/// </summary>
	class vector2
	{
	private:
	public:

		union
		{
			struct { float u, v; };
			struct { float x, y; };
			float arr_[2]{};
		};

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a new 2D vector with the specified values.
		/// </summary>
		/// <param name="x">Top value</param>
		/// <param name="y">Bottom value</param>
		explicit vector2(const float x = 0, const float y = 0)
		{
			this->x = x;
			this->y = y;
		}

		/// <summary>
		/// Constructs a copy of the specified Vector2.
		/// </summary>
		/// <param name="copy">The vector of which to create a copy</param>
		vector2(const vector2& copy)
			: vector2(copy.x, copy.y)
		{ }

		vector2(vector2&& move) noexcept
		{
			*this = std::move(move);
		}

		/* === OPERATORS === */

		void operator = (const vector2& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
		}

		void operator = (const vector2&& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
		}

		/// <summary>
		/// Adds two Vector2 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector2 resulting from the operation</returns>
		vector2 operator + (const vector2& other) const
		{
			vector2 vect;
			vect.x = this->x + other.x;
			vect.y = this->y + other.y;
			return vect;
		}

		/// <summary>
		/// Subtracts a Vector2 from this, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector2 resulting from the operation</returns>
		vector2 operator - (const vector2& other) const
		{
			vector2 vect;
			vect.x = this->x - other.x;
			vect.y = this->y - other.y;
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of two Vector2 instances together, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector2 resulting from the operation</returns>
		vector2 operator * (const vector2& other) const
		{
			vector2 vect;
			vect.x = this->x * other.x;
			vect.y = this->y * other.y;
			return vect;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector2 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector2 resulting from the operation</returns>
		vector2 operator * (const float& other) const
		{
			vector2 vect;
			vect.x = this->x * other;
			vect.y = this->y * other;
			return vect;
		}

		/// <summary>
		/// Divides the individual values of this Vector2 with a scalar, and returns the result.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector2 resulting from the operation</returns>
		vector2 operator / (const float& other) const
		{
			vector2 vect = *this;

			return vect * (1 / other);
		}

		/// <summary>
		/// Tests two Vector2 instances for equality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if equal, false otherwise</returns>
		bool operator == (const vector2& other) const
		{
			return x == other.x && y == other.y;
		}

		/// <summary>
		/// Tests two Vector2 instances for inequality.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>True if not equal, false otherwise</returns>
		bool operator != (const vector2& other) const
		{
			return x != other.x || y != other.y;
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Adds another Vector2 to this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector2 resulting from the operation</returns>
		vector2& operator += (const vector2& other)
		{
			this->x = this->x + other.x;
			this->y = this->y + other.y;
			return *this;
		}

		/// <summary>
		/// Subtracts another Vector2 from this, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector2 resulting from the operation</returns>
		vector2& operator -= (const vector2& other)
		{
			this->x = this->x - other.x;
			this->y = this->y - other.y;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector2 with another, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector2 resulting from the operation</returns>
		vector2& operator *= (const vector2& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Multiplies the individual values of this Vector2 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector2 resulting from the operation</returns>
		vector2& operator *= (const float& other)
		{
			*this = (*this) * other;
			return *this;
		}

		/// <summary>
		/// Divides the individual values of this Vector2 with a scalar, and returns the result as a reference.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>A reference to the Vector2 resulting from the operation</returns>
		vector2& operator /= (const float& other)
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
			if (index >= 2)
				throw std::out_of_range("Vector index out of range");

			return arr_[index];
		}

		/// <summary>
		/// Returns the (const) value at the n:th vector index, from top to bottom.
		/// </summary>
		/// <param name="n">The vector index to access</param>
		/// <returns>The value at the specified position in the vector</returns>
		float at(int y) const
		{
			if (y >= 2)
				throw std::out_of_range("Vector index out of range");

			return arr_[y];
		}

		/* === VECTOR FUNCTIONS === */

		/// <summary>
		/// Calculates the distance between two vectors (points).
		/// </summary>
		/// <param name="x">The first point</param>
		/// <param name="y">The second point</param>
		/// <returns>The distance between points as a float</returns>
		static float dist(const vector2& x, const vector2& y)
		{
			return (x - y).length();
		}

		/// <summary>
		/// Calculates the dot product between two vectors.
		/// </summary>
		/// <param name="x">The first vector</param>
		/// <param name="y">The second vector</param>
		/// <returns>The dot product as a float</returns>
		static float dot(const vector2& x, const vector2& y)
		{
			return x.dot(y);
		}

		static bool is_near(const vector2& a, const vector2& b, float treshold = 0.01f)
		{
			return dist(a, b) < treshold;
		}

		/// <summary>
		/// Calculates the dot product between this vector and another.
		/// </summary>
		/// <param name="other">The other vector to calculate dot product with</param>
		/// <returns>The dot product as a float</returns>
		float dot(const vector2& other) const
		{
			return (this->x * other.x + this->y * other.y);
		}

		/// <summary>
		/// Returns the length of this point from the origin (0, 0).
		/// </summary>
		/// <returns>The vector length as a float</returns>
		float length() const
		{
			return sqrt(powf(this->x, 2) + powf(this->y, 2));
		}

		/// <summary>
		/// Normalizes the vector, turning it into a unit vector.
		/// </summary>
		/// <returns>The resulting unit vector</returns>
		vector2 norm() const
		{
			vector2 v = (*this);
			float len = v.length();

			return len > 0 ? v * (1 / v.length()) : v;
		}

		/* === FACTORY FUNCTIONS === */

		/// <summary>
		/// Calculates the reflection of a vector against a specified normal.
		/// </summary>
		/// <param name="normal">The normal to calculate reflection against</param>
		/// <returns>The resulting reflection vector</returns>
		vector2 getReflection(const vector2& normal) const
		{
			return (*this) - (normal - (*this)) * normal * 2;
		}

		/* === UTILITIES === */

		/// <summary>
		/// Returns a formatted multi-line string representation of the vector.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the vector</returns>
		std::string to_string() const
		{
			return std::to_string(this->x) + ";\n" + std::to_string(this->y) + ";\n";
		}

		~vector2() { }
	};
}
