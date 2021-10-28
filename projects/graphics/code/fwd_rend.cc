#include "fwd_rend.h"
#include "mgr_host.h"

#include <GL/glew.h>

namespace efiilj
{
	forward_renderer::forward_renderer(const renderer_settings& set)
		: 
			settings_(set), _fallback_primary(-1)
	{
		printf("Init forward renderer...\n");
		_name = "Forward renderer";
	}  

	void forward_renderer::on_editor_gui()
	{ }

	void forward_renderer::on_editor_gui(render_id idx)
	{
		unsigned int s1 = _shaders->get_program_id(_fallback_primary);

		ImGui::BulletText("Default primary: %d / %u", _fallback_primary, s1);
		ImGui::BulletText("Nodes: %lu", _instances.size());
		ImGui::BulletText("Width: %u, Height: %u", settings_.width, settings_.height);

		bool err = _data.error[idx];
		bool vis = _data.visible[idx];

		ImGui::TextColored(err ? ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1), err ? "Model error state!" : "No error detected!");
		if (vis)
		{
			if (ImGui::Button("Visible"))
				_data.visible[idx] = false;
		}
		else
		{
			if (ImGui::Button("Hidden"))
				_data.visible[idx] = true;
		}
	}
	
	void forward_renderer::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
		_lights = host->get_manager_from_fcc<light_manager>('LGHT');
		_cameras = host->get_manager_from_fcc<camera_manager>('CAMS');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
		_material_instances = host->get_manager_from_fcc<material_manager>('MAMR');

		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_materials = host->get_manager_from_fcc<material_server>('MASR');
		_shaders = host->get_manager_from_fcc<shader_server>('SHDR');

		add_data(
				_data.error,
				_data.visible
				);
	}

	void forward_renderer::on_setup()
	{
		_fallback_primary = _shaders->create();
		_shaders->set_uri(_fallback_primary, settings_.default_fallback_path);
		_shaders->compile(_fallback_primary);
	}

	void forward_renderer::on_begin_frame()
	{}

	void forward_renderer::on_frame()
	{
		render_all();
	}

	void forward_renderer::on_end_frame()
	{}

	void forward_renderer::render(render_id idx)
	{
		if (!is_valid(idx))
			return;

		if (!get_visible(idx))
			return;

		if (get_error(idx))
			return;

		entity_id eid = get_entity(idx);

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
		{
			set_error(idx, true);
			return;
		}

		const matrix4& model = _transforms->get_model(trf_id);
		const auto& meshes = _mesh_instances->get_components(eid);
		
		for (auto it = meshes.first; it != meshes.second; it++)
		{
			mesh_instance_id miid = it->second;
			mesh_id mid = _mesh_instances->get_mesh(miid);
			_meshes->bind(mid);

			material_id mat_id = _mesh_instances->get_material(miid);

			if (_materials->apply(mat_id, _fallback_primary))
			{
				_shaders->set_uniform(settings_.u_model, model);
				_meshes->draw_elements(mid);
			}
			else set_error(idx, true);

		}

		_meshes->unbind();

	}

	void forward_renderer::render_all()
	{
		for (auto& idx : _instances)
		{
			render(idx);
		}
	}
}
