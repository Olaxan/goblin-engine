#include "fwd_rend.h"
#include "mgr_host.h"

#include <GL/glew.h>

namespace efiilj
{
	forward_renderer::forward_renderer(const renderer_settings& set)
		: 
			settings_(set), _default_fallback(-1)
	{
		printf("Init forward renderer...\n");
		_name = "Forward renderer";
	}  

	void forward_renderer::extend_defaults(render_id)
	{
	}

	void forward_renderer::draw_gui()
	{
	}

	void forward_renderer::draw_gui(render_id idx)
	{
		ImGui::Text("Render id %d", idx);
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
		
		_default_fallback = _shaders->create();
		_shaders->set_uri(_default_fallback, settings_.default_fallback_path);
		_shaders->compile(_default_fallback);
	}

	void forward_renderer::begin_frame()
	{
		on_begin_frame();
	}

	void forward_renderer::render(render_id idx) const
	{
		entity_id eid = _entities[idx];

		const auto& meshes = _mesh_instances->get_components(eid);

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return;

		const matrix4& model = _transforms->get_model(trf_id);
		
		for (auto it = meshes.first; it != meshes.second; it++)
		{
			mesh_instance_id miid = it->second;
			mesh_id mid = _mesh_instances->get_mesh(miid);
			_meshes->bind(mid);

			material_id mat_id = _mesh_instances->get_material(miid);

			if (_materials->is_valid(mat_id))
			{
				_materials->apply(mat_id);
				
				shader_id sid = _materials->get_program(mat_id);
				_shaders->set_uniform(sid, "model", model);

				_meshes->draw_elements(mid);
			}
		}

		_meshes->unbind();
	}

	void forward_renderer::render_frame() const
	{
		for (auto& idx : _instances)
		{
			render(idx);
		}
	}

	void forward_renderer::end_frame()
	{
		on_end_frame();
	}
}
