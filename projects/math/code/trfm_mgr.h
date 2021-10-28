#pragma once

#include "manager.h"
#include "ifmgr.h"
#include "quat.h"

#include <set>

namespace efiilj
{
	typedef int transform_id;

	class transform_manager : public manager<transform_id>
	{
		private:

			struct TransformData
			{
				ComponentData<matrix4> model;
				ComponentData<matrix4> inverse;

				ComponentData<vector4> position;
				ComponentData<vector4> offset;
				ComponentData<vector4> scale;
				ComponentData<quaternion> rotation;

				ComponentData<transform_id> parent;

				ComponentData<bool> model_updated;
				ComponentData<bool> inverse_updated;

				ComponentData<std::set<transform_id>> children;

			} _data;

		public:

			transform_manager();
			~transform_manager();

			void on_editor_gui() override;
			void on_editor_gui(transform_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			void update_models();

			const matrix4& get_model(transform_id);
			const matrix4& get_model_inv(transform_id);

			void detach(transform_id);

			transform_id get_parent(transform_id) const;
			void set_parent(transform_id child_id, transform_id parent_id);
			
			void set_updated(transform_id idx, bool updated);

			const std::set<transform_id>& get_children(transform_id idx) const
			{
				return _data.children[idx];
			}

			void set_offset(transform_id idx, const vector3& offset)
			{
				_data.offset[idx] = vector4(offset, 1.0f);
				set_updated(idx, false);
			}

			vector3 get_offset(transform_id idx) const
			{
				return _data.offset[idx].xyz();
			}

			vector3 get_position(transform_id);
			void set_position(transform_id, const vector3& pos);

			const quaternion& get_rotation(transform_id) const;
			void set_rotation(transform_id, const quaternion& rot);
			void set_rotation(transform_id, const vector3& euler);

			vector3 get_scale(transform_id)	const;
			void set_scale(transform_id, const vector3& scale);
			void set_scale(transform_id, const float& scale);

			void add_position(transform_id, const vector3& delta);
			void add_scale(transform_id, const vector3& delta);
			void add_rotation(transform_id, const vector3& axis, float angle);

			vector3 get_left(transform_id);
			vector3 get_right(transform_id);
			vector3 get_up(transform_id);
			vector3 get_down(transform_id);
			vector3 get_forward(transform_id);
			vector3 get_backward(transform_id);

	};
}
