#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector2.h"

namespace efiilj
{

	/// <summary>
	/// Class to represent a 2-dimensional matrix.
	/// </summary>
	class matrix2
	{
	private:

	public:

		union
		{
			struct { float a, b, c, d; };
			vector2 arr_[2];
		};

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a matrix from the specified a, b, c, d values (from top-left to bottom-right).
		/// </summary>
		/// <param name="a">Top left</param>
		/// <param name="b">Top right</param>
		/// <param name="c">Bottom left</param>
		/// <param name="d">Bottom right</param>
		matrix2(float a, float b, float c, float d)
		{
			this->a = a;
			this->b = b;
			this->c = c;
			this->d = d;
		}

		/// <summary>
		/// Creates a new identity Matrix2.
		/// </summary>
		matrix2()
			: matrix2(0, 1, 0, 1)
		{
			clear();
		}

		/// <summary>
		/// Constructs a copy of the specified Matrix2.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		matrix2(const matrix2& copy)
			: matrix2(copy.a, copy.b, copy.c, copy.d)
		{
			*this = copy;
		}

		/* === ACCESSORS === */

		//float& a;
		//float& b;
		//float& c;
		//float& d;

		//const float& a const { return this->arr_[0].x(); }
		//void a(const float& a) { this->arr_[0].x(a); }

		//const float& b const { return this->arr_[0].y(); }
		//void b(const float& b) { this->arr_[0].y(b); }

		//const float& c const { return this->arr_[1].x(); }
		//void c(const float& c) { this->arr_[1].x(c); }

		//const float& d const { return this->arr_[1].y(); }
		//void d(const float& d) { this->arr_[1].y(d); }

		/* === OPERATORS === */

		/// <summary>
		/// Sets the matrix to equal the specified matrix.
		/// </summary>
		/// <param name="other">The matrix which values are to be copied</param>
		/// <returns>A reference to the current matrix, after modification</returns>
		matrix2& operator = (const matrix2& other)
		{
			this->a = other.a;
			this->b = other.b;
			this->c = other.c;
			this->d = other.d;

			return *this;
		}

		/// <summary>
		/// Performs a matrix multiplication with another Matrix2.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix2 resulting from the operation</returns>
		matrix2 operator * (const matrix2& other) const
		{
			matrix2 mat;

			mat(0, 0) = vector2::dot(row(0), other.col(0));
			mat(0, 1) = vector2::dot(row(1), other.col(0));

			mat(1, 0) = vector2::dot(row(0), other.col(1));
			mat(1, 1) = vector2::dot(row(1), other.col(1));

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix2 resulting from the operation</returns>
		matrix2 operator * (const float& other) const
		{
			matrix2 mat = *this;

			for (int i = 0; i < 4; i++)
			{
				mat(i) *= other;
			}

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a Vector2.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector2 resulting from the operation</returns>
		vector2 operator * (const vector2& other) const
		{
			vector2 vect;
			vect.x = other.dot(row(0));
			vect.y = other.dot(row(1));
			return vect;
		}

		/// <summary>
		/// Performs a matrix division with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix2 resulting from the operation</returns>
		matrix2 operator / (const float& other) const
		{
			matrix2 mat = *this;

			return mat * (1 / other);
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Multiplies this matrix with a vector and returns the result.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>A reference to the Vector2 resulting from the operation</returns>
		vector2& operator *= (vector2& other) const
		{
			other = (*this) * other;
			return other;
		}

		/* === ACCESSORS === */

		/// <summary>
		/// Returns the (non-const) value at the n:th position in the matrix, from top-left to bottom-right.
		/// </summary>
		/// <param name="n">The matrix index to query</param>
		/// <returns>The value at matrix index n</returns>
		float& operator () (int n)
		{
			return arr_[n / 2][n % 2];
		}

		/// <summary>
		/// Returns the (non-const)value at matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-poisition, row</param>
		/// <returns>The value at matrix position x, y</returns>
		float& operator () (int x, int y)
		{
			return arr_[y][x];
		}

		/// <summary>
		/// Returns the (const) value at the n:th matrix index, from top-left to bottom-right.
		/// </summary>
		/// <param name="n">The matrix index to access</param>
		/// <returns>The value at matrix index n</returns>
		float at(int n) const
		{
			return at(n / 2, n % 2);
		}

		/// <summary>
		/// Returns the (const) value at matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-poisition, row</param>
		/// <returns>The value at matrix position x, y</returns>
		float at(int x, int y) const
		{
			return arr_[y].at(x);
		}

		/// <summary>
		/// Returns the row at specified y-position as a 3D-vector.
		/// </summary>
		/// <param name="y">The row index to return</param>
		/// <returns>The row at the specified y-position</returns>
		vector2 row(int y) const
		{
			return vector2(at(0, y), at(1, y));
		}

		/// <summary>
		/// Returns the column at specified x-position as a 3D-vector.
		/// </summary>
		/// <param name="x">The column index to return</param>
		/// <returns>The column at the specified x-position</returns>
		vector2 col(int x) const
		{
			return vector2(at(x, 0), at(x, 1));
		}

		/* === MATRIX FUNCTIONS === */

		/// <summary>
		/// Clears the matrix, optionally creating an identity matrix.
		/// </summary>
		/// <param name="identity">Whether to create an empty matrix, or an identity matrix</param>
		void clear(bool identity = true)
		{
			for (int i = 0; i < 4; i++)
			{
				(*this)(i) = identity && (i % 3 == 0);
			}
		}

		/// <summary>
		/// Returns the matrix determinant.
		/// </summary>
		/// <returns>The determinant of the matrix</returns>
		float determinant() const
		{
			return a * d - b * c;
		}

		/// <summary>
		/// Returns a transposed copy of the matrix (shifted along the diagonal).
		/// </summary>
		/// <returns>Transposed copy of the current matrix</returns>
		matrix2 transpose() const
		{
			return matrix2(this->a, this->c, this->b, this->d);
		}

		/// <summary>
		/// Returns the inverse of the matrix, or an identity if none exists.
		/// </summary>
		/// <returns>The inverse of the current matrix</returns>
		matrix2 inverse() const
		{
			float det = determinant();

			if (det == 0)
				return matrix2();

			return matrix2(d, -b, -c, a) / det;
		}


		/* === FACTORY FUNCTIONS === */

		/// <summary>
		/// Returns a rotation matrix for the specified rotation in 2D-space.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <returns>A rotation matrix for the 2D rotation</returns>
		static matrix2 getRotationMatrix(const float& rad)
		{
			return matrix2(cos(rad), -sin(rad), sin(rad), cos(rad));
		}

		/* UTILITIES */

		/// <summary>
		/// Returns a formatted multi-line string representation of the matrix.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the matrix</returns>
		std::string to_string()
		{
			std::stringstream ss;
			ss << a << ", " << b << ";\n" << c << ", " << d << ";\n";
			return ss.str();
		}

		~matrix2() { }

		friend vector2;
	};
}
