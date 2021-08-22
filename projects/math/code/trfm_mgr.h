#pragma once

#include <vector>
#include <unordered_map>

#include "entity.h"
#include "quat.h"

namespace efiilj
{
	typedef int transform_id;

	class transform_manager : public manager<transform_id>
	{
		private:

			std::vector<transform_id> _instances;
			std::unordered_map<entity_id, transform_id> _instance_mapping;

			struct
			{
				mutable std::vector<matrix4> model;
				mutable std::vector<matrix4> inverse;

				std::vector<vector4> position;
				std::vector<vector4> scale;
				std::vector<quaternion> rotation;

				std::vector<transform_id> parent;

				mutable std::vector<bool> model_updated;
				mutable std::vector<bool> inverse_updated;

				std::vector<std::vector<transform_id>> children;

			} _data;

		public:

			transform_manager();
			~transform_manager();

			transform_id register_entity(entity_id eid);
			bool unregister_entity(transform_id idx);

			void update_models();

			const matrix4& get_model(transform_id) const;
			const matrix4& get_model_inv(transform_id) const;

			transform_id get_parent(transform_id) const;
			void set_parent(transform_id child_id, transform_id parent_id);

			vector3 get_position(transform_id) const;
			void set_position(transform_id, const vector3& pos);

			const quaternion& get_rotation(transform_id) const;
			void set_rotation(transform_id, const quaternion& rot);

			vector3 get_scale(transform_id)	const;
			void set_scale(transform_id, const vector3& scale);
			void set_scale(transform_id, const float& scale);

			void add_position(transform_id, const vector3& delta);
			void add_scale(transform_id, const vector3& delta);
			void add_rotation(transform_id, const vector3& axis, float angle);

			vector3 get_left(transform_id) const;
			vector3 get_right(transform_id) const;
			vector3 get_up(transform_id) const;
			vector3 get_down(transform_id) const;
			vector3 get_forward(transform_id) const;
			vector3 get_backward(transform_id) const;

	};
}
