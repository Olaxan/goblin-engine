#pragma once

#include "matrix4.h"

namespace efiilj
{
	struct transform_model
	{
	private:

		vector3 position_;
		vector3 scale_;
		vector3 rotation_;

		matrix4 model_;
		bool is_changed_;

	public:

		explicit transform_model(const vector3& pos = vector3(0, 0, 0), const vector3& rot = vector3(0, 0, 0),
		                         const vector3& scale = vector3(1, 1, 1));


		vector3& position() { return position_; }
		void position(const vector3& position) { position_ = position; is_changed_ = true; }

		vector3& scale() { return scale_; }
		void scale(const vector3& scale) { scale_ = scale; is_changed_ = true; }

		vector3& rotation() { return rotation_; }
		void rotation(const vector3& rotation) { rotation_ = rotation; is_changed_ = true; }

		const matrix4& model();
		
		vector3 forward() const
		{
			return vector3 (
				cos(rotation_.x()) * cos(rotation_.y()),
				sin(rotation_.x()),
				cos(rotation_.x()) * sin(rotation_.y())).norm();
		}

		vector3 backward() const
		{
			return forward() * -1;
		}

		vector3 left() const
		{
			return vector3::cross(vector3(0, 1, 0), forward()).norm();
		}

		vector3 right() const
		{
			return left() * -1;
		}
		
		vector3 up() const
		{
			return vector3::cross(forward(), left());
		}

		vector3 down() const
		{
			return up() * -1;
		}

		~transform_model()
			= default;
	};
}
