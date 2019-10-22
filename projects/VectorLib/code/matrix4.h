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
	class matrix4
	{
	private:
		vector4 arr_[4];

	public:

		/* === CONSTRUCTORS === */

		/// <summary>
		/// Creates a new identity Matrix4.
		/// </summary>
		matrix4(const bool identity = true)
		{
			clear(identity);
		}

		/// <summary>
		/// Inserts the specified Matrix2 into the top-left corner of an 4x4 identity matrix.
		/// </summary>
		/// <param name="copy">The matrix of which to create a copy</param>
		matrix4(const matrix2& copy)
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
		matrix4(const matrix3& copy)
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
		matrix4(const matrix4& copy)
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
		matrix4(const vector4& a, const vector4& b, const vector4& c, const vector4& d)
		{
			arr_[0] = a;
			arr_[1] = b;
			arr_[2] = c;
			arr_[3] = d;
		}

		/* === OPERATORS === */

		/// <summary>
		/// Sets the matrix to equal the specified matrix.
		/// </summary>
		/// <param name="other">The matrix which values are to be copied</param>
		/// <returns>A reference to the current matrix, after modification</returns>
		matrix4& operator = (const matrix4& other)
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
		matrix4 operator * (const matrix4& other) const
		{
			matrix4 mat;

			mat(0, 0) = vector4::dot4(row(0), other.col(0));
			mat(0, 1) = vector4::dot4(row(1), other.col(0));
			mat(0, 2) = vector4::dot4(row(2), other.col(0));
			mat(0, 3) = vector4::dot4(row(3), other.col(0));

			mat(1, 0) = vector4::dot4(row(0), other.col(1));
			mat(1, 1) = vector4::dot4(row(1), other.col(1));
			mat(1, 2) = vector4::dot4(row(2), other.col(1));
			mat(1, 3) = vector4::dot4(row(3), other.col(1));

			mat(2, 0) = vector4::dot4(row(0), other.col(2));
			mat(2, 1) = vector4::dot4(row(1), other.col(2));
			mat(2, 2) = vector4::dot4(row(2), other.col(2));
			mat(2, 3) = vector4::dot4(row(3), other.col(2));

			mat(3, 0) = vector4::dot4(row(0), other.col(3));
			mat(3, 1) = vector4::dot4(row(1), other.col(3));
			mat(3, 2) = vector4::dot4(row(2), other.col(3));
			mat(3, 3) = vector4::dot4(row(3), other.col(3));

			return mat;
		}

		/// <summary>
		/// Performs a matrix multiplication with a scalar.
		/// </summary>
		/// <param name="other">The right-hand side of the operation</param>
		/// <returns>The Matrix4 resulting from the operation</returns>
		matrix4 operator * (const float& other) const
		{
			matrix4 mat = *this;

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
		vector4 operator * (const vector4& other) const
		{
			vector4 vect;
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
		matrix4 operator / (const float& other) const
		{
			const matrix4 mat = *this;

			return mat * (1 / other);
		}

		/* === SHORTHAND OPERATORS === */

		/// <summary>
		/// Multiplies this matrix with a vector and returns the result.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>A reference to the Vector4 resulting from the operation</returns>
		vector4& operator *= (vector4& other) const
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
			return arr_[i / 4][i % 4];
		}

		/// <summary>
		/// Returns the (non-const) value at matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-poisition, row</param>
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
			if (n >= 16)
				throw std::out_of_range("Matrix index out of range");

			return at(n % 4, n / 4);
		}

		/// <summary>
		/// Returns the (const) value at the matrix position x, y.
		/// </summary>
		/// <param name="x">Matrix x-position, column</param>
		/// <param name="y">Matrix y-position, row</param>
		/// <returns>The value at matrix position x, y</returns>
		const float& at(const int x, const int y) const
		{
			if (x >= 4 || y >= 4)
				throw std::out_of_range("Matrix index out of range");

			return arr_[y].at(x);
		}

		/// <summary>
		/// Returns the row at specified y-position as a 4D-vector.
		/// </summary>
		/// <param name="y">The row index to return</param>
		/// <returns>The row at the specified y-position</returns>
		vector4 row(const int y) const
		{
			if (y >= 4)
				throw std::out_of_range("Row index out of range");

			return arr_[y];
		}

		/// <summary>
		/// Sets the row at the specified y-position.
		/// </summary>
		/// <param name="y">The row index to change</param>
		/// <param name="row">The Vector4 to replace the row with</param>
		/// <param name="relative">Whether the operation should be added to the existing values, or replace them<param name=""></param>
		void row(const int y, vector4& row, const bool relative = false)
		{
			if (y >= 4)
				throw std::out_of_range("Row index out of range");

			arr_[y] = relative ? arr_[y] + row : row ;
		}

		/// <summary>
		/// Returns the column at specified x-position as a 4D-vector.
		/// </summary>
		/// <param name="x">The column index to return</param>
		/// <returns>The column at the specified x-position</returns>
		vector4 col(const int x) const
		{
			if (x > 4)
				throw std::out_of_range("Column index out of range");

			return vector4(at(x, 0), at(x, 1), at(x, 2), at(x, 3));
		}

		/// <summary>
		/// Sets the column at the specified y-position.
		/// </summary>
		/// <param name="x">The column index to change</param>
		/// <param name="col">The Vector4 to change the column with</param>
		/// <param name="relative">Whether the operation should be added to the existing values, or replace them</param>
		void col(const int x, const vector4& col, const bool relative = false)
		{
			if (x > 4)
				throw std::out_of_range("Row index out of range");

			(*this)(x, 0) = relative ? at(x, 0) + col.x() : col.x();
			(*this)(x, 1) = relative ? at(x, 1) + col.y() : col.y();
			(*this)(x, 2) = relative ? at(x, 2) + col.z() : col.z();
			(*this)(x, 3) = relative ? at(x, 3) + col.w() : col.w();
		}

		/* === MATRIX FUNCTIONS === */

		/// <summary>
		/// Clears the matrix, optionally creating an identity matrix.
		/// </summary>
		/// <param name="identity">Whether to create an empty matrix, or an identity matrix</param>
		void clear(const bool identity = true)
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
		matrix3 minor(const int x, const int y) const
		{
			matrix3 mat;
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
			const matrix3 a = minor(0, 0);
			const matrix3 b = minor(1, 0);
			const matrix3 c = minor(2, 0);
			const matrix3 d = minor(3, 0);

			return at(0) * a.determinant() - at(1) * b.determinant() + at(2) * c.determinant() - at(3) * d.determinant();
		}

		/// <summary>
		/// Returns a transposed copy of the matrix (shifted along the diagonal).
		/// </summary>
		/// <returns>Transposed copy of the current matrix</returns>
		matrix4 transpose() const
		{
			matrix4 mat;

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
		matrix4 inverse() const
		{
			matrix4 inv;
			const float det = determinant();

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
		static matrix4 get_translation(const vector4& v)
		{
			matrix4 mat = matrix4();

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
		static matrix4 get_translation(const float x, const float y, const float z)
		{
			matrix4 mat = matrix4();

			mat(3, 0) = x;
			mat(3, 1) = y;
			mat(3, 2) = z;

			return mat;
		}

		static matrix4 get_scale(const float x, const float y, const float z)
		{
			matrix4 mat = matrix4();

			mat(0, 0) = x;
			mat(1, 1) = y;
			mat(2, 2) = z;

			return mat;
		}

		static matrix4 get_scale(const vector3& xyz)
		{
			return get_scale(xyz.x(), xyz.y(), xyz.z());
		}

		/// <summary>
		/// Returns a rotation matrix for a specified rotation around the x-axis.
		/// </summary>
		/// <param name="rad">The rotation in radians</param>
		/// <returns>A rotation matrix for the rotation</returns>
		static matrix4 get_rotation_x(const float rad)
		{
			matrix4 mat;

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
		static matrix4 get_rotation_y(const float rad)
		{
			matrix4 mat;

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
		static matrix4 get_rotation_z(const float rad)
		{
			matrix4 mat;

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
		static matrix4 get_rotation_xyz(const float rad, const vector3& axis)
		{
			const vector3 unit = axis.norm();

			const float sin = sinf(rad);
			const float cos = cosf(rad);
			const float x = unit.x();
			const float y = unit.y();
			const float z = unit.z();

			matrix4 mat;

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

		static matrix4 get_rotation_xyz(const vector3& eulers)
		{
			// TODO: Optimize
			return get_rotation_z(eulers.z()) * get_rotation_y(eulers.y()) * get_rotation_x(eulers.x());
		}

		static matrix4 get_perspective(const float left, const float right, const float top, const float bottom, const float near, const float far)
		{

			matrix4 mat(false);

			mat(0, 0) = (2.0f * near) / (right - left);
			mat(2, 0) = -((right + left) / (right - left));
			mat(1, 1) = (2.0f * near) / (top - bottom);
			mat(2, 1) = -((top + bottom) / (top - bottom));
			mat(2, 2) = -(far / (far - near));
			mat(3, 2) = -((far * near) / (far - near));
			mat(2, 3) = -1.0f;

			return mat;
		}
		
		//144540105

		static matrix4 getPerspective(const float fov, const float aspect, const float near, const float far)
		{
			const float top = tanf(fov / 2.0f) * near;
			const float bottom = -top;
			const float right = top * aspect;
			const float left = bottom * aspect;

			return get_perspective(left, right, top, bottom, near, far);
		}

		static matrix4 get_lookat(const vector3& camera_pos, const vector3& camera_target, const vector3& up_direction)
		{
			const vector3 camera_direction = (camera_pos - camera_target).norm();
			const vector3 camera_right = vector3::cross(up_direction, camera_direction).norm();
			const vector3 camera_up = vector3::cross(camera_direction, camera_right);

			matrix4 a = matrix4(vector4(camera_right, 1), vector4(camera_up, 1), vector4(camera_direction, 1), vector4());
			a(3, 0) = vector3::dot(camera_right, camera_pos) * -1;
			a(3, 1) = vector3::dot(camera_up, camera_pos) * -1;
			a(3, 2) = vector3::dot(camera_direction, camera_pos) * -1;

			return a;
		}

		/* === UTILITIES === */

		/// <summary>
		/// Returns a formatted multi-line string representation of the matrix.
		/// </summary>
		/// <returns>A formatted multi-line string representation of the matrix</returns>
		std::string to_string() const
		{
			std::stringstream ss;
			ss << at(0, 0) << ", " << at(1, 0) << ", " << at(2, 0) << ", " << at(3, 0) << ";\n";
			ss << at(0, 1) << ", " << at(1, 1) << ", " << at(2, 1) << ", " << at(3, 1) << ";\n";
			ss << at(0, 2) << ", " << at(1, 2) << ", " << at(2, 2) << ", " << at(3, 2) << ";\n";
			ss << at(0, 3) << ", " << at(1, 3) << ", " << at(2, 3) << ", " << at(3, 3) << ";\n";
			return ss.str();
		}

		std::string to_mem_string() const
		{
			std::stringstream ss;
			for (int i = 0; i < 16; i++)
			{
				ss << *(&(*this).at(0) + i) << ", ";
			}

			ss << std::endl;
			return ss.str();
		}

		~matrix4()
			= default;

		friend vector4;
	};
}

#undef PI