#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector3.h"
#include "matrix2.h"

namespace efiilj
{

	/// <summary>
	/// Class to represent a 3-dimensional matrix.
	/// </summary>
	class matrix3
	{
	private:

	public:

		union
		{
			struct { float a, b, c, d, e, f, g, h, i; };
			vector3 arr_[3];
		};

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a new identity Matrix3.
		/// </summary>
		matrix3()
		{
			clear();
		}

		/// <summary>
		/// Inserts the specified Matrix2 into the top-left corner of an 3x3 identity matrix.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		matrix3(const matrix2& copy)
		{
			clear();

			(*this)(0, 0) = copy.a;
			(*this)(1, 0) = copy.b;
			(*this)(0, 1) = copy.c;
			(*this)(1, 1) = copy.d;
		}

		/// <summary>
		/// Constructs a copy of the specified Matrix3.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		matrix3(const matrix3& copy)
		{
			*this = copy;
		}

		/// <summary>
		/// Creates a Matrix3 from the specified row vectors, from top to bottom.
		/// </summary>
		/// <param name="x">First row</param>
		/// <param name="y">Second row</param>
		/// <param name="z">Third row</param>
		matrix3(const vector3& x, const vector3& y, const vector3& z)
		{
			arr_[0] = x;
			arr_[1] = y;
			arr_[2] = z;
		}

		/* === OPERATORS === */

		/// <summary>
		/// Sets the matrix to equal the specified matrix.
		/// </summary>
		/// <param name="other">The matrix which values are to be copied</param>
		/// <returns>A reference to the current matrix, after modification</returns>
		matrix3& operator = (const matrix3& other)
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

		/// <summary>
		/// Performs a matrix multiplication with another Matrix3.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix3 resulting from the operation</returns>
		matrix3 operator * (const matrix3& other) const
		{
			matrix3 mat;

			mat(0, 0) = vector3::dot(row(0), other.col(0));
			mat(0, 1) = vector3::dot(row(1), other.col(0));
			mat(0, 2) = vector3::dot(row(2), other.col(0));

			mat(1, 0) = vector3::dot(row(0), other.col(1));
			mat(1, 1) = vector3::dot(row(1), other.col(1));
			mat(1, 2) = vector3::dot(row(2), other.col(1));

			mat(2, 0) = vector3::dot(row(0), other.col(2));
			mat(2, 1) = vector3::dot(row(1), other.col(2));
			mat(2, 2) = vector3::dot(row(2), other.col(2));

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix3 resulting from the operation</returns>
		matrix3 operator * (const float& other) const
		{
			matrix3 mat = *this;

			for (int i = 0; i < 9; i++)
			{
				mat(i) *= other;
			}

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a Vector3.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector3 resulting from the operation</returns>
		vector3 operator * (const vector3& other) const
		{
			vector3 vect;
			vect.x = other.dot(row(0));
			vect.y = other.dot(row(1));
			vect.z = other.dot(row(2));
			return vect;
		}

		/// <summary>
		/// Performs a matrix division with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix3 resulting from the operation</returns>
		matrix3 operator / (const float& other) const
		{
			const matrix3 mat = *this;

			return mat * (1 / other);
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Multiplies this matrix with a vector and returns the result.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>A reference to the Vector3 resulting from the operation</returns>
		vector3& operator *= (vector3& other) const
		{
			other = (*this) * other;
			return other;
		}

		/* === ACCESSORS === */

		/// <summary>
		/// Returns the (non-const) value at the n:th position in the matrix, from top-left to bottom-right.
		/// </summary>
		/// <param name="i">The matrix index to query</param>
		/// <returns>The value at matrix index n</returns>
		float& operator () (const int i)
		{
			return arr_[i / 3][i % 3];
		}

		/// <summary>
		/// Returns the (non-const) value at matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-position, row</param>
		/// <returns>The value at matrix position x, y</returns>
		float& operator () (const int x, const int y)
		{
			return arr_[y][x];
		}

		/// <summary>
		/// Returns the (const) value at the n:th matrix index, from top-left to bottom-right.
		/// </summary>
		/// <param name="n">The matrix index to access</param>
		/// <returns>The value at matrix index n</returns>
		const float& at(const int n) const
		{
			if (n >= 9)
				throw std::out_of_range("Matrix index out of range");

			return at(n % 3, n / 3);
		}

		/// <summary>
		/// Returns the (const) value at the matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-position, row</param>
		/// <returns>The value at matrix position x, y</returns>
		const float& at(const int x, const int y) const
		{
			if (x >= 3 || y >= 3)
				throw std::out_of_range("Matrix index out of range");

			return arr_[y].at(x);
		}

		/// <summary>
		/// Returns the row at specified y-position as a 3D-vector.
		/// </summary>
		/// <param name="y">The row index to return</param>
		/// <returns>The row at the specified y-position</returns>
		vector3 row(int y) const
		{
			if (y > 3)
				throw std::out_of_range("Row index out of range");

			return vector3(at(0, y), at(1, y), at(2, y));
		}

		/// <summary>
		/// Returns the column at specified x-position as a 3D-vector.
		/// </summary>
		/// <param name="x">The column index to return</param>
		/// <returns>The column at the specified x-position</returns>
		vector3 col(int x) const
		{
			if (x > 3)
				throw std::out_of_range("Column index out of range");

			return vector3(at(x, 0), at(x, 1), at(x, 2));
		}

		/* === MATRIX FUNCTIONS === */

		/// <summary>
		/// Clears the matrix, optionally creating an identity matrix.
		/// </summary>
		/// <param name="identity">Whether to create an empty matrix, or an identity matrix</param>
		void clear(bool identity = true)
		{
			for (int i = 0; i < 9; i++)
			{
				(*this)(i) = identity && (i % 4 == 0);
			}
		}

		/// <summary>
		/// Returns the minor matrix of the specified cell, excluding its row and column.
		/// </summary>
		/// <param name="x">The x-coordinate of the cell</param>
		/// <param name="y">The y-coordinate of the cell</param>
		/// <returns>A 2D matrix excluding the specified row and column</returns>
		matrix2 minor(int x, int y) const
		{
			matrix2 mat;
			int j = 0;

			for (int i = 0; i < 9; i++)
			{
				if (i % 3 != x && i / 3 != y)
				{
					mat(j) = at(i);
					j++;
				}

				if (j == 4)
					break;
			}

			return mat;
		}

		/// <summary>
		/// Returns the matrix determinant.
		/// </summary>
		/// <returns>The determinant of the matrix</returns>
		float determinant() const
		{
			matrix2 a, b, c;

			a = minor(0, 0);
			b = minor(1, 0);
			c = minor(2, 0);

			return at(0) * a.determinant() - at(1) * b.determinant() + at(2) * c.determinant();
		}

		/// <summary>
		/// Returns a transposed copy of the matrix (shifted along the diagonal).
		/// </summary>
		/// <returns>Transposed copy of the current matrix</returns>
		matrix3 transpose() const
		{
			matrix3 mat;

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

		/// <summary>
		/// Returns the inverse of the matrix, or an identity if none exists.
		/// </summary>
		/// <returns>The inverse of the current matrix</returns>
		matrix3 inverse() const
		{
			matrix3 inv;
			float det = determinant();

			if (det == 0)
				return inv;

			for (int i = 0; i < 9; i++)
			{
				inv(i) = minor(i % 3, i / 3).determinant();

				if (i % 2 != 0)
					inv(i) *= -1;
			}

			inv = inv.transpose();

			return inv / det;
		}

		/* === FACTORY FUNCTIONS === */

		/// <summary>
		/// Constructs a translation matrix from the specified 4D-vector.
		/// </summary>
		/// <param name="v">The vector to use for translation</param>
		/// <returns>A new translation matrix</returns>
		static matrix3 getTranslation(const vector3& v)
		{
			matrix3 mat = matrix3();
			mat(2, 0) = v.x;
			mat(2, 1) = v.y;
			mat(2, 2) = v.z;
			return mat;
		}

		/// <summary>
		/// Returns a rotation matrix for the specified rotation in 2D-space.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <returns>A rotation matrix for the 2D rotation</returns>
		static matrix3 getRotationMatrix(const float& rad)
		{
			matrix2 mat = matrix2::getRotationMatrix(rad);
			return matrix3(mat);
		}

		/* === UTILITIES === */

		/// <summary>
		/// Returns a formatted multi-line string representation of the matrix.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the matrix</returns>
		std::string to_string()
		{
			std::stringstream ss;
			ss << at(0, 0) << ", " << at(1, 0) << ", " << at(2, 0) << ";\n";
			ss << at(0, 1) << ", " << at(1, 1) << ", " << at(2, 1) << ";\n";
			ss << at(0, 2) << ", " << at(1, 2) << ", " << at(2, 2) << ";\n";
			return ss.str();
		}

		~matrix3() { }

		friend vector3;
	};
}

