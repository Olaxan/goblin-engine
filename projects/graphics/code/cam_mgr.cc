#include "cam_mgr.h"
#include "imgui.h"
#include "mgr_host.h"

#include <GL/glew.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 1024
#define DEFAULT_FOV 1.30899694f
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 100.0f


namespace efiilj
{
	camera_manager::camera_manager()
	: _ubo(0), _current(0) 
	{
		printf("Init camera...\n");
		_name = "Camera";
	}

	void camera_manager::on_editor_gui()
	{
		if (ImGui::TreeNode("Camera"))
		{
			on_editor_gui(_current);
			ImGui::TreePop();
		}
	}

	void camera_manager::on_editor_gui(camera_id idx)
	{

		if (!is_valid(idx))
		{
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "No camera selected");
			return;
		}

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Camera %d", idx);
		ImGui::Text("Uniform buffer %d", _ubo);

		if (!(_current == idx) && ImGui::Button("Set active"))
			set_camera(idx);

		if (ImGui::TreeNode("Properties"))
		{
			if (ImGui::InputFloat("Width", &_data.width[idx])
					|| ImGui::InputFloat("Height", &_data.height[idx])
					|| ImGui::DragFloat("FOV", &_data.fov[idx], 0.01f)
					|| ImGui::DragFloat("Near", &_data.near[idx], 0.1f)
					|| ImGui::DragFloat("Far", &_data.far[idx], 0.1f))
			{
				update_perspective(idx);
				push_perspective();

				update_view(idx);
				push_view();
			}

			if (ImGui::TreeNode("Perspective matrix"))
			{
				ImGui::InputFloat4("X", &_data.perspective[idx][0].x);
				ImGui::InputFloat4("Y", &_data.perspective[idx][1].x);
				ImGui::InputFloat4("Z", &_data.perspective[idx][2].x);
				ImGui::InputFloat4("W", &_data.perspective[idx][3].x);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("View matrix"))
			{
				ImGui::InputFloat4("X", &_data.view[idx][0].x);
				ImGui::InputFloat4("Y", &_data.view[idx][1].x);
				ImGui::InputFloat4("Z", &_data.view[idx][2].x);
				ImGui::InputFloat4("W", &_data.view[idx][3].x);
				
				ImGui::TreePop();
			}

			ImGui::TreePop();

		}
	}
	
	void camera_manager::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');

		add_data({
				&_data.width,
				&_data.height,
				&_data.fov,
				&_data.near,
				&_data.far,
				&_data.perspective,
				&_data.p_inverse,
				&_data.view,
				&_data.transform
				});
	}

	void camera_manager::on_setup()
	{
		setup_ubo();
	}

	void camera_manager::on_activate(camera_id idx)
	{

	}

	void camera_manager::on_begin_frame()
	{
		update_view(_current);
		push_view();
	}

	ray camera_manager::get_ray_from_camera(camera_id idx, int mouse_x, int mouse_y) const
	{
		float x = (2.0f * mouse_x) / _data.width[idx] - 1.0f;
		float y = 1.0f - (2.0f * mouse_y) / _data.height[idx];

		vector4 ray_clip(x, y, -1.0f, 1.0f);
		vector4 ray_eye = _data.p_inverse[idx] * ray_clip;
		ray_eye = vector4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

		vector4 ray_world = _data.view[idx].inverse() * ray_eye;
		vector3 ray_dir = ray_world.xyz();
		ray_dir = ray_dir.norm();

		vector3 pos = _transforms->get_position(_data.transform[idx]);

		return ray(pos, ray_dir);
	}

	void camera_manager::setup_ubo()
	{
		glGenBuffers(1, &_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(matrix4), nullptr, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, 2 * sizeof(matrix4));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::update_perspective(camera_id idx)
	{
		float aspect = _data.width[idx] / _data.height[idx];
		_data.perspective[idx] = matrix4::get_perspective(_data.fov[idx], aspect, _data.near[idx], _data.far[idx]);
		_data.p_inverse[idx] = _data.perspective[idx].inverse();
	}

	void camera_manager::push_perspective()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &_data.perspective[_current]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void camera_manager::update_view(camera_id idx)
	{
		vector3 pos = _transforms->get_position(_data.transform[idx]);
		vector3 fwd = pos + _transforms->get_forward(_data.transform[idx]);
		vector3 up = _transforms->get_up(_data.transform[idx]);
		_data.view[idx] = matrix4::get_lookat(pos, fwd, up);
	}

	void camera_manager::push_view()
	{	
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrix4), sizeof(matrix4), &_data.view[_current]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	camera_id camera_manager::get_camera() const
	{
		return _current;
	}
	
	bool camera_manager::set_camera(camera_id idx)
	{
		if (is_valid(idx))
		{
			_current = idx;

			update_perspective(idx);
			push_perspective();
			push_view();

			return true;
		}

		return false;
	}

	float camera_manager::get_width(camera_id idx) const
	{
		return _data.width[idx];
	}

	void camera_manager::set_width(camera_id idx, float width)
	{
		_data.width[idx] = width;
		update_view(idx);
		push_view();
	}

	float camera_manager::get_height(camera_id idx) const
	{
		return _data.height[idx];
	}

	void camera_manager::set_height(camera_id idx, float height)
	{
		_data.height[idx] = height;
		update_view(idx);
		push_view();
	}

	void camera_manager::set_size(camera_id idx, float width, float height)
	{
		_data.width[idx] = width;
		_data.height[idx] = height;
		update_view(idx);
		push_view();
	}

	float camera_manager::get_fov(camera_id idx) const
	{
		return _data.fov[idx];
	}

	void camera_manager::set_fov(camera_id idx, float fov)
	{
		_data.fov[idx] = fov;
		update_perspective(idx);
		push_perspective();
	}

	float camera_manager::get_near(camera_id idx) const
	{
		return _data.near[idx];
	}

	void camera_manager::set_near(camera_id idx, float near)
	{
		_data.near[idx] = near;
		update_perspective(idx);
		push_perspective();
	}

	float camera_manager::get_far(camera_id idx) const
	{
		return _data.far[idx];
	}

	void camera_manager::set_far(camera_id idx, float far)
	{
		_data.far[idx] = far;
		update_perspective(idx);
		push_perspective();
	}

	const matrix4& camera_manager::get_perspective(camera_id idx) const
	{ return _data.view[idx]; }

	const matrix4& camera_manager::get_view(camera_id idx) const
	{ return _data.perspective[idx]; }
	
	transform_id camera_manager::get_transform(camera_id idx) const
	{ return _data.transform[idx]; }

	void camera_manager::set_transform(camera_id idx, transform_id trf)
	{ _data.transform[idx] = trf; }
}
