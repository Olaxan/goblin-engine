#pragma once

#include "matrix4.h"

namespace efiilj
{
	/**
	 * \brief A class to represent a point in 3D-space.
	 */
	struct transform_model
	{
	private:

		matrix4 model_;
		matrix4 inverse_;

		vector4 position_;
		vector4 scale_;
		vector4 rotation_;

		bool inverse_dirty_;

		void update_model();

	public:

		/**
		 * \brief Creates a new transform instance.
		 * \param pos Position XYZ coordinates
		 * \param rot Rotation in Euler angles (pitch, roll, yaw)
		 * \param scale Scale along XYZ axis
		 */
		explicit transform_model(
				const vector3& pos = vector3(0, 0, 0), 
				const vector3& rot = vector3(0, 0, 0),
			    const vector3& scale = vector3(1, 1, 1));

		/**
		 * \brief Retrieves a model matrix for the current transform.
		 * \return A 4-dimensional matrix which represents the point in 3D-space
		 */
		const matrix4& get_model() const { return model_; }
		const matrix4& get_model_inv();

		const vector4& get_position() const { return position_; }
		const vector4& get_rotation() const { return rotation_; }
		const vector4& get_scale() const { return scale_; }

		void set_position(const vector4& position) { position_ = position; update_model(); }
		void set_rotation(const vector4& rotation) { rotation_ = rotation; update_model(); }
		void set_scale(const vector4& scale) { scale_ = scale; update_model(); }
		void set_scale(const float& scale) { scale_ = vector4(scale, scale, scale, 1.0); update_model(); }

		void add_position(const vector4& delta) { position_ += delta; update_model(); }
		void add_rotation(const vector4& delta) { rotation_ += delta; update_model(); }
		void add_scale(const vector4& delta) { scale_ += delta; update_model(); }

		/**
		 * \brief Returns a forward vector relative to the current transform
		 * \return Unit vector forward relative to transform
		 */
		vector4 forward() const;

		/**
		 * \brief Returns a reverse forward vector relative to the current transform
		 * \return Unit vector backward relative to transform
		 */
		vector4 backward() const;

		/**
		 * \brief Returns a left pointing vector relative to the current transform
		 * \return Unit vector left relative to transform
		 */
		vector4 left() const;

		/**
		 * \brief Returns a right pointing vector relative to the current transform
		 * \return Unit vector right relative to transform
		 */
		vector4 right() const;

		/**
		 * \brief Returns a upwards vector relative to the current transform
		 * \return Unit vector up relative to transform
		 */
		vector4 up() const;

		/**
		 * \brief Returns a downward vector relative to the current transform
		 * \return Unit vector down relative to transform
		 */
		vector4 down() const;

		~transform_model()
			= default;
	};
}
