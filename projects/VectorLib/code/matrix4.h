#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "vector4.h"
#include "matrix3.h"

#define PI 3.1415926535

namespace efiilj
{

	/// <summary>
	/// Class to represent a 4-dimensional matrix.
	/// </summary>
	class Matrix4
	{
	private:
		Vector4 _arr[4];

	public:

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a new identity Matrix4.
		/// </summary>
		Matrix4(bool identity = true)
		{
			clear(identity);
		}

		/// <summary>
		/// Inserts the specified Matrix2 into the top-left corner of an 4x4 identity matrix.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		Matrix4(const Matrix2& copy)
		{
			clear();

			(*this)(0, 0) = copy.a();
			(*this)(1, 0) = copy.b();
			(*this)(0, 1) = copy.c();
			(*this)(1, 1) = copy.d();
		}

		/// <summary>
		/// Inserts the specified Matrix3 into the top-left corner of an 4x4 identity matrix.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		Matrix4(const Matrix3& copy)
		{
			clear();

			(*this)(0, 0) = copy.at(0, 0);
			(*this)(1, 0) = copy.at(1, 0);
			(*this)(2, 0) = copy.at(2, 0);

			(*this)(0, 1) = copy.at(0, 1);
			(*this)(1, 1) = copy.at(1, 1);
			(*this)(2, 1) = copy.at(2, 1);

			(*this)(0, 2) = copy.at(0, 2);
			(*this)(1, 2) = copy.at(1, 2);
			(*this)(2, 2) = copy.at(2, 2);
		}

		/// <summary>
		/// Constructs a copy of the specified matrix.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		Matrix4(const Matrix4& copy)
		{
			*this = copy;
		}

		/// <summary>
		/// Creates a Matrix4 from the specified row vectors, from top to bottom.
		/// </summary>
		/// <param name="a">First row</param>
		/// <param name="b">Second row</param>
		/// <param name="c">Third row</param>
		/// <param name="d">Fourth row</param>
		Matrix4(Vector4 a, Vector4 b, Vector4 c, Vector4 d)
		{
			_arr[0] = a;
			_arr[1] = b;
			_arr[2] = c;
			_arr[3] = d;
		}

		/* === OPERATORS === */

		/// <summary>
		/// Sets the matrix to equal the specified matrix.
		/// </summary>
		/// <param name="other">The matrix which values are to be copied</param>
		/// <returns>A reference to the current matrix, after modification</returns>
		Matrix4& operator = (const Matrix4& other)
		{
			(*this)(0, 0) = other.at(0, 0);
			(*this)(0, 1) = other.at(0, 1);
			(*this)(0, 2) = other.at(0, 2);
			(*this)(0, 3) = other.at(0, 3);

			(*this)(1, 0) = other.at(1, 0);
			(*this)(1, 1) = other.at(1, 1);
			(*this)(1, 2) = other.at(1, 2);
			(*this)(1, 3) = other.at(1, 3);

			(*this)(2, 0) = other.at(2, 0);
			(*this)(2, 1) = other.at(2, 1);
			(*this)(2, 2) = other.at(2, 2);
			(*this)(2, 3) = other.at(2, 3);

			(*this)(3, 0) = other.at(3, 0);
			(*this)(3, 1) = other.at(3, 1);
			(*this)(3, 2) = other.at(3, 2);
			(*this)(3, 3) = other.at(3, 3);

			return *this;
		}

		/// <summary>
		/// Performs a matrix multiplication with another Matrix4.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix4 resulting from the operation</returns>
		Matrix4 operator * (const Matrix4& other) const
		{
			Matrix4 mat;

			mat(0, 0) = Vector4::dot4(row(0), other.col(0));
			mat(0, 1) = Vector4::dot4(row(1), other.col(0));
			mat(0, 2) = Vector4::dot4(row(2), other.col(0));
			mat(0, 3) = Vector4::dot4(row(3), other.col(0));

			mat(1, 0) = Vector4::dot4(row(0), other.col(1));
			mat(1, 1) = Vector4::dot4(row(1), other.col(1));
			mat(1, 2) = Vector4::dot4(row(2), other.col(1));
			mat(1, 3) = Vector4::dot4(row(3), other.col(1));

			mat(2, 0) = Vector4::dot4(row(0), other.col(2));
			mat(2, 1) = Vector4::dot4(row(1), other.col(2));
			mat(2, 2) = Vector4::dot4(row(2), other.col(2));
			mat(2, 3) = Vector4::dot4(row(3), other.col(2));

			mat(3, 0) = Vector4::dot4(row(0), other.col(3));
			mat(3, 1) = Vector4::dot4(row(1), other.col(3));
			mat(3, 2) = Vector4::dot4(row(2), other.col(3));
			mat(3, 3) = Vector4::dot4(row(3), other.col(3));

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix4 resulting from the operation</returns>
		Matrix4 operator * (const float& other) const
		{
			Matrix4 mat = *this;

			for (int i = 0; i < 16; i++)
			{
				mat(i) *= other;
			}

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a Vector4.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Vector4 resulting from the operation</returns>
		Vector4 operator * (const Vector4& other) const
		{
			Vector4 vect;
			vect.x(other.dot4(row(0)));
			vect.y(other.dot4(row(1)));
			vect.z(other.dot4(row(2)));
			vect.w(other.dot4(row(3)));
			return vect;
		}

		/// <summary>
		/// Performs a matrix division with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix4 resulting from the operation</returns>
		Matrix4 operator / (const float& other) const
		{
			Matrix4 mat = *this;

			return mat * (1 / other);
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Multiplies this matrix with a vector and returns the result.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		Vector4& operator *= (Vector4& other) const
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
		float& operator () (int i)
		{
			return _arr[i / 4][i % 4];
		}

		/// <summary>
		/// Returns the (non-const) value at matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-poisition, row</param>
		/// <returns>The value at matrix position x, y</returns>
		float& operator () (int x, int y)
		{
			return _arr[y][x];
		}

		/// <summary>
		/// Returns the (const) value at the n:th matrix index, from top-left to bottom-right.
		/// </summary>
		/// <param name="n">The matrix index to access</param>
		/// <returns>The value at matrix index n</returns>
		float at(int n) const
		{
			if (n > 16)
				throw new std::out_of_range("Matrix index out of range");

			return at(n % 4, n / 4);
		}

		/// <summary>
		/// Returns the (const) value at the matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-poisition, row</param>
		/// <returns>The value at matrix position x, y</returns>
		float at(int x, int y) const
		{
			if (x > 4 || y > 4)
				throw new std::out_of_range("Matrix index out of range");

			return _arr[y].at(x);
		}

		/// <summary>
		/// Returns the row at specified y-position as a 4D-vector.
		/// </summary>
		/// <param name="y">The row index to return</param>
		/// <returns>The row at the specified y-position</returns>
		Vector4 row(int y) const
		{
			if (y > 4)
				throw new std::out_of_range("Row index out of range");

			return Vector4(at(0, y), at(1, y), at(2, y), at(3, y));
		}

		/// <summary>
		/// Returns the column at specified x-position as a 4D-vector.
		/// </summary>
		/// <param name="x">The column index to return</param>
		/// <returns>The column at the specified x-position</returns>
		Vector4 col(int x) const
		{
			if (x > 4)
				throw new std::out_of_range("Column index out of range");

			return Vector4(at(x, 0), at(x, 1), at(x, 2), at(x, 3));
		}

		/* === MATRIX FUNCTIONS === */

		/// <summary>
		/// Clears the matrix, optionally creating an identity matrix.
		/// </summary>
		/// <param name="identity">Whether to create an empty matrix, or an identity matrix</param>
		void clear(bool identity = true)
		{
			for (int i = 0; i < 16; i++)
			{
				(*this)(i) = identity && (i % 5 == 0);
			}
		}

		/// <summary>
		/// Returns the minor matrix of the specified cell, excluding its row and column.
		/// </summary>
		/// <param name="x">The x-coordinate of the cell</param>
		/// <param name="y">The y-coordinate of the cell</param>
		/// <returns>A 3x3 matrix excluding the specified row and column</returns>
		Matrix3 minor(int x, int y) const
		{
			Matrix3 mat;
			int j = 0;

			for (int i = 0; i < 16; i++)
			{
				if (i % 4 != x && i / 4 != y)
				{
					mat(j) = at(i);
					j++;
				}

				if (j == 9)
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
			Matrix3 a, b, c, d;

			a = minor(0, 0);
			b = minor(1, 0);
			c = minor(2, 0);
			d = minor(3, 0);

			return at(0) * a.determinant() - at(1) * b.determinant() + at(2) * c.determinant() - at(3) * d.determinant();
		}

		/// <summary>
		/// Returns a transposed copy of the matrix (shifted along the diagonal).
		/// </summary>
		/// <returns>Transposed copy of the current matrix</returns>
		Matrix4 transpose() const
		{
			Matrix4 mat;

			mat(0, 0) = at(0, 0);
			mat(0, 1) = at(1, 0);
			mat(0, 2) = at(2, 0);
			mat(0, 3) = at(3, 0);

			mat(1, 0) = at(0, 1);
			mat(1, 1) = at(1, 1);
			mat(1, 2) = at(2, 1);
			mat(1, 3) = at(3, 1);

			mat(2, 0) = at(0, 2);
			mat(2, 1) = at(1, 2);
			mat(2, 2) = at(2, 2);
			mat(2, 3) = at(3, 2);

			mat(3, 0) = at(0, 3);
			mat(3, 1) = at(1, 3);
			mat(3, 2) = at(2, 3);
			mat(3, 3) = at(3, 3);

			return mat;
		}

		/// <summary>
		/// Returns the inverse of the matrix, or an identity if none exists.
		/// </summary>
		/// <returns>The inverse of the current matrix</returns>
		Matrix4 inverse() const
		{
			Matrix4 inv;
			float det = determinant();

			if (det == 0)
				return inv;

			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					inv(x, y) = minor(x, y).determinant();

					if ((x + (y % 2 == 0)) % 2 == 0)
						inv(x, y) *= -1;
				}
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
		static Matrix4 getTranslation(const Vector4& v)
		{
			Matrix4 mat = Matrix4();

			mat(3, 0) = v.x();
			mat(3, 1) = v.y();
			mat(3, 2) = v.z();
			mat(3, 3) = v.w();

			return mat;
		}

		/// <summary>
		/// Constructs a translation matrix from the specified 4D-vector.
		/// </summary>
		/// <param name="x">Translation in the x axis</param>
		/// <param name="y">Translation in the x axis</param>
		/// <param name="z">Translation in the x axis</param>
		/// <returns>A new translation matrix</returns>
		static Matrix4 getTranslation(float x, float y, float z)
		{
			Matrix4 mat = Matrix4();

			mat(3, 0) = x;
			mat(3, 1) = y;
			mat(3, 2) = z;

			return mat;
		}

		static Matrix4 getScale(float x, float y, float z)
		{
			Matrix4 mat = Matrix4();

			mat(0, 0) = x;
			mat(1, 1) = y;
			mat(2, 2) = z;

			return mat;
		}

		/// <summary>
		/// Returns a rotation matrix for a specified rotation around the x-axis.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <returns>A rotation matrix for the rotation</returns>
		static Matrix4 getRotationX(const float rad)
		{
			Matrix4 mat;

			mat(1, 1) = cosf(rad);
			mat(2, 1) = -sinf(rad);
			mat(1, 2) = sinf(rad);
			mat(2, 2) = cosf(rad);

			return mat;
		}

		/// <summary>
		/// Returns a rotation matrix for a specified rotation around the y-axis.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <returns>A rotation matrix for the rotation</returns>
		static Matrix4 getRotationY(const float rad)
		{
			Matrix4 mat;

			mat(0, 0) = cosf(rad);
			mat(2, 0) = sinf(rad);
			mat(0, 2) = -sinf(rad);
			mat(2, 2) = cosf(rad);

			return mat;
		}

		/// <summary>
		/// Returns a rotation matrix for a specified rotation around the x-axis.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <returns>A rotation matrix for the rotation</returns>
		static Matrix4 getRotationZ(const float rad)
		{
			Matrix4 mat;

			mat(0, 0) = cosf(rad);
			mat(1, 0) = -sinf(rad);
			mat(0, 1) = sinf(rad);
			mat(1, 1) = cosf(rad);

			return mat;
		}

		/// <summary>
		/// Returns a rotation matrix for a specified rotation around the x-axis.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <param name="axis">The axis for rotation</param>
		/// <returns>A rotation matrix for the rotation</returns>
		static Matrix4 getRotationXYZ(const float rad, const Vector3 axis)
		{
			Vector3 unit = axis.norm();

			float sin = sinf(rad);
			float cos = cosf(rad);
			float x = unit.x();
			float y = unit.y();
			float z = unit.z();

			Matrix4 mat;

			mat(0, 0) = cos + (x * x) * (1 - cos);
			mat(0, 1) = y * x * (1 - cos) + z * sin;
			mat(0, 2) = z * x * (1 - cos) - y * sin;

			mat(1, 0) = x * y * (1 - cos) - z * sin;
			mat(1, 1) = cos + y * y * (1 - cos);
			mat(1, 2) = z * y * (1 - cos) + x * sin;

			mat(2, 0) = x * z * (1 - cos) + y * sin;
			mat(2, 1) = y * z * (1 - cos) - x * sin;
			mat(2, 2) = cos + z * z * (1 - cos);

			return mat;
		}

		static Matrix4 getPerspective(float left, float right, float top, float bottom, float near, float far)
		{

			Matrix4 mat(false);

			mat(0, 0) = (2 * near) / (right - left);
			mat(2, 0) = (right + left) / (right - left);
			mat(1, 1) = (2 * near) / (top - bottom);
			mat(2, 1) = (top + bottom) / (top - bottom);
			mat(2, 2) = -((far + near) / (far - near));
			mat(3, 2) = -((2 * far * near) / (far - near));
			mat(2, 3) = -1;

			return mat;
		}

		static Matrix4 getPerspective(float fovY, float aspect, float near, float far)
		{
			float tangent = tanf(fovY / 2);
			float height = near * tangent;
			float width = height * aspect;

			return getPerspective(-width, width, -height, height, near, far);
		}

		static Matrix4 getLookat(Vector3 cameraPos, Vector3 cameraTarget, Vector3 upDirection)
		{

			Vector3 cameraDirection = (cameraPos - cameraTarget).norm();
			Vector3 cameraRight = Vector3::cross(upDirection, cameraDirection).norm();
			Vector3 cameraUp = Vector3::cross(cameraDirection, cameraRight);

			Matrix4 A = Matrix4(Vector4(cameraRight), Vector4(cameraUp), Vector4(cameraDirection), Vector4());

			Matrix4 B = Matrix4();
			B(3, 0) = -cameraPos.x();
			B(3, 1) = -cameraPos.y();
			B(3, 2) = -cameraPos.z();

			return A * B;
		}

		/* === UTILITIES === */

		/// <summary>
		/// Returns a formatted multi-line string representation of the matrix.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the matrix</returns>
		std::string to_string()
		{
			std::stringstream ss;
			ss << at(0, 0) << ", " << at(1, 0) << ", " << at(2, 0) << ", " << at(3, 0) << ";\n";
			ss << at(0, 1) << ", " << at(1, 1) << ", " << at(2, 1) << ", " << at(3, 1) << ";\n";
			ss << at(0, 2) << ", " << at(1, 2) << ", " << at(2, 2) << ", " << at(3, 2) << ";\n";
			ss << at(0, 3) << ", " << at(1, 3) << ", " << at(2, 3) << ", " << at(3, 3) << ";\n";
			return ss.str();
		}

		~Matrix4() { }

		friend Vector4;
	};
}

#undef PI