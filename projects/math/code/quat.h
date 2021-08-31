#pragma once

#include "matrix4.h"

namespace efiilj
{
	class quaternion
	{
		protected:


		public:

			union
			{
				vector4 xyzw;
				struct { float x, y, z, w; };
			};

			quaternion();
			quaternion(const vector3& euler);
			quaternion(const vector3& axis, const float& angle);
			quaternion(const vector4& xyzw);
			quaternion(float x, float y, float z, float w);

			quaternion(const quaternion& copy);
			quaternion(const quaternion&& move);

			~quaternion();

			quaternion norm() const;
			void normalize();

			float square_magnitude() const;
			float magnitude() const;

			void add_axis_rotation(const vector3& axis, const float& angle);

			matrix4 get_rotation_matrix() const;
			vector3 get_euler() const;
			quaternion get_local_rotation(const vector3& axis, const float& angle);

			quaternion operator + (const quaternion& other) const;
			quaternion operator * (const quaternion& other) const;
			quaternion operator * (const float s) const;

			void operator = (const quaternion& other);

			quaternion& operator *= (const quaternion& other);
			quaternion& operator += (const quaternion& other);
	};

	extern quaternion operator * (float lhs, const quaternion& rhs);
}
