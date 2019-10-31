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

	public:

		/**
		 * \brief Creates a new transform instance.
		 * \param pos Position XYZ coordinates
		 * \param rot Rotation in Euler angles (pitch, roll, yaw)
		 * \param scale Scale along XYZ axis
		 */
		explicit transform_model(const vector3& pos = vector3(0, 0, 0), const vector3& rot = vector3(0, 0, 0),
		                         const vector3& scale = vector3(1, 1, 1));


		vector4 position;
		vector4 scale;
		vector4 rotation;

		/**
		 * \brief Retrieves a model matrix for the current transform.
		 * \return A 4-dimensional matrix which represents the point in 3D-space
		 */
		const matrix4& model();

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
