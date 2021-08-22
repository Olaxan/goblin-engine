#pragma once

#include <vector>
#include <unordered_map>

#include "entity.h"
#include "quat.h"

namespace efiilj
{
	typedef unsigned int transform_id;

	class transform_manager : public manager<transform_id>
	{
		private:

			std::unordered_map<entity_id, transform_id> _instances;

			struct
			{
				std::vector<matrix4> model;
				std::vector<matrix4> inverse;

				std::vector<quaternion> rotation;

				std::vector<bool> model_dirty;
				std::vector<bool> inverse_dirty;

				std::vector<transform_id> parent;

			} _data;

		public:

			transform_manager();
			~transform_manager();

			transform_id register_entity(entity_id eid);
			bool unregister_entity(transform_id idx);

			const matrix4& get_model() const;
			const matrix4& get_model_inv() const;

			transform_id get_parent() const;
			void set_parent(transform_id child_id, transform_id parent_id);

			vector3 get_position(transform_id) const;
			void set_position(transform_id, const vector3& pos);

			vector3 get_rotation(transform_id) const;
			void set_rotation(transform_id, const vector3& rot);

			vector3 get_scale(transform_id)	const;
			void set_scale(transform_id, const vector3& scale);
			void set_scale(transform_id, const float& scale);

			void add_position(transform_id, const vector3& delta);
			void add_scale(transform_id, const vector3& delta);
			void add_rotation(transform_id, const vector3& axis, float angle);

			vector3 get_forward(transform_id) const;
			vector3 get_backward(transform_id) const;
			vector3 get_left(transform_id) const;
			vector3 get_right(transform_id) const;
			vector3 get_up(transform_id) const;
			vector3 get_down(transform_id) const;

	};
}
