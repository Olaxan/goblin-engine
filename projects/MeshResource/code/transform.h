#pragma once

#include "matrix4.h"

namespace efiilj
{
	class transform_model
	{
	private:

		Vector3 position_;
		Vector3 scale_;
		Vector3 rotation_;

		Matrix4 model_;
		bool is_dirty_;

		void set_dirty(const bool dirty = true) { this->is_dirty_ = dirty; }

	public:

		explicit transform_model(const Vector3& pos = Vector3(0, 0, 0), const Vector3& rot = Vector3(0, 0, 0),
		                         const Vector3& scale = Vector3(1, 1, 1));

		void position(const Vector3& xyz, const bool relative = false)
		{
			relative ? position_ += xyz : position_ = xyz;
			set_dirty();
		}

		const Vector3& position() const { return this->position_; }

		void rotation(const Vector3& xyz, const bool relative = false)
		{
			relative ? rotation_ += xyz : rotation_ = xyz;
			set_dirty();
		}

		const Vector3& rotation() const { return this->rotation_; }

		void scale(const Vector3& xyz, const bool relative = false)
		{
			relative ? scale_ += xyz : scale_ = xyz;
			set_dirty();
		}

		const Vector3& scale() const { return this->scale_; }

		const Matrix4& model();

		~transform_model()
		= default;
	};
}
