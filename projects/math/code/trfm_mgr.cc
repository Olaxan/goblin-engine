#include "trfm_mgr.h"

#include "imgui.h"
#include "stdio.h"
#include "mathutils.h"

namespace efiilj
{
	transform_manager::transform_manager()
	{
		printf("Init transform...\n");
		_name = "Transform";
	}

	transform_manager::~transform_manager()
	{}

	void transform_manager::extend_defaults(transform_id)
	{
		_data.model.emplace_back(matrix4());
		_data.inverse.emplace_back(matrix4());
		_data.position.emplace_back(vector4());
		_data.scale.emplace_back(vector4(1.0f, 1.0f, 1.0f, 1.0f));
		_data.rotation.emplace_back(quaternion());
		_data.parent.emplace_back(-1);
		_data.children.emplace_back();
		_data.model_updated.emplace_back(true);
		_data.inverse_updated.emplace_back(true);
	}

	void transform_manager::draw_gui()
	{
	}

	void transform_manager::draw_gui(transform_id idx)
	{
		if (!is_valid(idx))
			return;

		ImGui::Text("Parent id: %d", _data.parent[idx]);
		ImGui::Text("Is model updated: %s", _data.model_updated[idx] ? "true" : "false");
		ImGui::Text("Is inverse updated: %s", _data.inverse_updated[idx] ? "true" : "false");

		if (ImGui::TreeNode("Properties"))
		{
			vector3 temp_euler = _data.rotation[idx].get_euler();
			bool pos_changed = ImGui::DragFloat3("Position", &_data.position[idx].x, 0.01f);
			bool rot_changed = ImGui::DragFloat3("Rotation", &temp_euler.x, 0.01f);
			ImGui::InputFloat4("Quaternion", &_data.rotation[idx].x);
			bool scl_changed = ImGui::DragFloat3("Scale", &_data.scale[idx].x, 0.01f);

			if (rot_changed)
				set_rotation(idx, temp_euler);

			if (pos_changed || rot_changed || scl_changed)
				set_updated(idx, false);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Model matrix"))
		{
			ImGui::InputFloat4("X", &_data.model[idx][0].x);
			ImGui::InputFloat4("Y", &_data.model[idx][1].x);
			ImGui::InputFloat4("Z", &_data.model[idx][2].x);
			ImGui::InputFloat4("W", &_data.model[idx][3].x);

			if (ImGui::Button("Randomize!"))
			{
				for (size_t i = 0; i < 4 * 4; i++)
					_data.model[idx].at(i) = randf(-1.0f, 1.0f);

				set_updated(idx, false);
			}
			ImGui::TreePop();
		}
	}

	void transform_manager::on_register(std::shared_ptr<manager_host> host) //NOLINT
	{
	}

	void transform_manager::update_models()
	{
		for (const transform_id& idx : _instances)
		{
			if (_data.model_updated[idx])
				continue;

			get_model(idx);
		}
	}

	const matrix4& transform_manager::get_model(transform_id idx)
	{

		if (!_data.model_updated[idx])
		{
			matrix4 t = matrix4::get_translation(_data.position[idx]);
			matrix4 r = _data.rotation[idx].get_rotation_matrix();
			matrix4 s = matrix4::get_scale(_data.scale[idx]);

			_data.model[idx] = t * r * s;

			transform_id parent_id = _data.parent[idx];

			if (is_valid(parent_id))
			{
				_data.model[idx] = get_model(parent_id) * _data.model[idx];
			}

			set_updated(idx, true);
		}

		return _data.model[idx];
	}

	const matrix4& transform_manager::get_model_inv(transform_id idx)
	{
		if (!_data.inverse_updated[idx])
		{
			_data.inverse[idx] = _data.model[idx].inverse();
			_data.inverse_updated[idx] = true;
		}

		return _data.inverse[idx];
	}

	void transform_manager::set_updated(transform_id idx, bool updated)
	{
		_data.model_updated[idx] = updated;
		_data.inverse_updated[idx] = false;

		if (!updated)
		{
			for (const auto& child : _data.children[idx])
				set_updated(child, false);
		}
	}

	transform_id transform_manager::get_parent(transform_id idx) const
	{
		return _data.parent[idx];
	}

	void transform_manager::detach(transform_id idx)
	{
		transform_id parent = get_parent(idx);

		if (is_valid(parent))
		{
			_data.children[parent].erase(idx);
			_data.parent[idx] = -1;
		}
	}

	void transform_manager::set_parent(transform_id child_id, transform_id parent_id)
	{
		if (child_id == parent_id)
			return;

		detach(child_id);

		if (_data.children[child_id].find(parent_id) != _data.children[child_id].end())
			detach(parent_id);

		_data.parent[child_id] = parent_id;
		_data.children[parent_id].emplace(child_id);
		_data.model_updated[child_id] = false;
	}

	vector3 transform_manager::get_position(transform_id idx)
	{
		get_model(idx);
		return _data.model[idx].col(3).xyz();
	}

	void transform_manager::set_position(transform_id idx, const vector3& pos)
	{
		_data.position[idx] = vector4(pos, 1.0f);
		set_updated(idx, false);
	}

	const quaternion& transform_manager::get_rotation(transform_id idx) const
	{
		return _data.rotation[idx];
	}

	void transform_manager::set_rotation(transform_id idx, const quaternion& rot)
	{
		_data.rotation[idx] = rot;	
		set_updated(idx, false);
	}
	
	void transform_manager::set_rotation(transform_id idx, const vector3& euler)
	{
		_data.rotation[idx] = quaternion(euler);
		set_updated(idx, false);
	}

	vector3 transform_manager::get_scale(transform_id idx)	const
	{
		return _data.scale[idx].xyz();
	}

	void transform_manager::set_scale(transform_id idx, const vector3& scale)
	{
		_data.scale[idx] = vector4(scale, 1.0f);
		set_updated(idx, false);
	}

	void transform_manager::set_scale(transform_id idx, const float& scale)
	{
		_data.scale[idx] = vector4(scale, scale, scale, 1.0f);
		set_updated(idx, false);
	}

	void transform_manager::add_position(transform_id idx, const vector3& delta)
	{
		_data.position[idx] += vector4(delta, 1.0f);
		set_updated(idx, false);
	}

	void transform_manager::add_scale(transform_id idx, const vector3& delta)
	{
		_data.scale[idx] += vector4(delta, 1.0f);
		set_updated(idx, false);
	}

	void transform_manager::add_rotation(transform_id idx, const vector3& axis, float angle)
	{
		_data.rotation[idx].add_axis_rotation(axis, angle);
		set_updated(idx, false);
	}

	vector3 transform_manager::get_left(transform_id idx)
	{
		get_model(idx);
		return _data.model[idx].col(0).xyz().norm();
	}

	vector3 transform_manager::get_right(transform_id idx)
	{
		return get_left(idx) * -1.0f;
	}

	vector3 transform_manager::get_up(transform_id idx)
	{
		get_model(idx);
		return _data.model[idx].col(1).xyz().norm();
	}

	vector3 transform_manager::get_down(transform_id idx)
	{
		return get_up(idx) * -1.0f;
	}

	vector3 transform_manager::get_forward(transform_id idx)
	{
		get_model(idx);
		return _data.model[idx].col(2).xyz().norm();
	}

	vector3 transform_manager::get_backward(transform_id idx)
	{
		return get_forward(idx) * -1.0f;
	}
}
