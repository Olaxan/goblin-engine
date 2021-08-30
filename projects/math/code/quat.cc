#include "quat.h"

#include <cmath>

#define EPSILON 0.00001f

namespace efiilj
{

	quaternion::quaternion()
		: xyzw()
	{}

	quaternion::quaternion(const vector4& xyzw)
		: xyzw(xyzw)
	{}

	quaternion::quaternion(float x, float y, float z, float w)
		: xyzw(x, y, z, w)
	{}

	quaternion::quaternion(const vector3& euler)
		: xyzw()
	{
		const float& pitch = euler.x;
		const float& yaw = euler.y;
		const float& roll = euler.z;

		float cy = cosf(yaw * 0.5f);
		float sy = sinf(yaw * 0.5f);
		float cp = cosf(pitch * 0.5f);
		float sp = sinf(pitch * 0.5f);
		float cr = cosf(roll * 0.5f);
		float sr = sinf(roll * 0.5f);

		xyzw.w = cr * cp * cy + sr * sp * sy;
		xyzw.x = sr * cp * cy - cr * sp * sy;
		xyzw.y = cr * sp * cy + sr * cp * sy;
		xyzw.z = cr * cp * sy - sr * sp * cy;

	}

	quaternion::quaternion(const vector3& axis, const float& angle)
	{
		*this = get_local_rotation(axis, angle);
	}

	quaternion::quaternion(const quaternion& copy)
	{
		*this = copy;
	}

	quaternion::quaternion(const quaternion&& move)
	{
		*this = std::move(move);
	}

	quaternion::~quaternion()
	{}

	float quaternion::square_magnitude() const
	{
		return xyzw.square_magnitude();
	}

	float quaternion::magnitude() const 
	{
		return xyzw.magnitude();
	}

	quaternion quaternion::norm() const
	{
		return quaternion();
	}

	void quaternion::normalize()
	{
		xyzw.normalize();
	}

	void quaternion::add_axis_rotation(const vector3& axis, const float& angle)
	{

		quaternion local = get_local_rotation(axis, angle);
		*this = (local * (*this));

		float err = fabs(1.0f - square_magnitude());

		if (err > EPSILON)
		{
			printf("Normalized quaternion from %f\n", err);
			normalize();
		}

	}
	
	quaternion quaternion::operator * (const quaternion& other) const
	{
		const vector4& q1 = xyzw;
		const vector4& q2 = other.xyzw;

		vector4 q;

		q.w = (q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
		q.x = (q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y);
		q.y = (q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x);
		q.z = (q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w);

		return quaternion(q);
	}

	quaternion quaternion::operator * (const float& s) const
	{
		quaternion q(xyzw.x * s, xyzw.y * s, xyzw.z * s, xyzw.w * s);
	}

	void quaternion::operator = (const quaternion& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	quaternion& quaternion::operator *= (const quaternion& other)
	{
		*this = other * (*this);
		return *this;
	}

	vector3 quaternion::get_euler() const
	{
		vector3 xyz;
		float& pitch = xyz.x;
		float& yaw = xyz.y;
		float& roll = xyz.z;

		const quaternion& q = *this;

		float sinr_cosp = 2 * (q.w * q.x + q.y * q.z); 
		float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);

		roll = std::atan2(sinr_cosp, cosr_cosp);

		float sinp = 2 * (q.w * q.y - q.z * q.x);

		if (std::abs(sinp) >= 1.0f)
			pitch = std::copysign(M_PI / 2.0, sinp);
		else
			pitch = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);

		yaw = std::atan2(siny_cosp, cosy_cosp);

		return xyz;
		
	}

	matrix4 quaternion::get_rotation_matrix() const 
	{
		matrix4 m(true);		

		m[0][0] = 1 - 2 * powf(y, 2) - 2 * powf(z, 2);
		m[1][0] = 2 * x * y - 2 * w * z;
		m[2][0] = 2 * x * z + 2 * w * y;

		m[0][1] = 2 * x * y + 2 * w * z;
		m[1][1] = 1 - 2 * powf(x, 2) - 2 * powf(z, 2);
		m[2][1] = 2 * y * z - 2 * w * x;

		m[0][2] = 2 * x * z - 2 * w * y;
		m[1][2] = 2 * y * z + 2 * w * x;
		m[2][2] = 1 - 2 * powf(x, 2) - 2 * powf(y, 2);

		return m;

	}

	quaternion quaternion::get_local_rotation(const vector3& axis, const float& angle)
	{
		vector4 v
			(
				axis.x * sinf(angle / 2.0f),
				axis.y * sinf(angle / 2.0f),
				axis.z * sinf(angle / 2.0f),
				cosf(angle / 2.0f)
			);

		return quaternion(v);
	}
}

#undef EPSILON
