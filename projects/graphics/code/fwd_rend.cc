#include "fwd_rend.h"
#include "mgr_host.h"

#include <GL/glew.h>

namespace efiilj
{
	forward_renderer::forward_renderer(const renderer_settings& set)
		: 
			settings_(set)
	{
		printf("Init forward renderer...\n");
		printf("Forward renderer ready\n");
	}  

	void forward_renderer::extend_defaults(render_id new_id)
	{
		_data.mesh.emplace_back(-1);
		_data.material.emplace_back(-1);
		_data.transform.emplace_back(-1);
	}

	void forward_renderer::draw_gui()
	{
		if (ImGui::TreeNode("Forward renderer"))
		{
			ImGui::Text("No render selected");	
			ImGui::TreePop();
		}
	}

	void forward_renderer::draw_gui(render_id idx)
	{
		ImGui::Text("Not implemented!");
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
	}

	void forward_renderer::begin_frame()
	{
		on_begin_frame();
	}

	void forward_renderer::render(render_id idx) const
	{
		//_data.mesh[idx]->bind();
		//_data.material[idx]->apply();
		//
		//matrix4 model = _transforms->get_model(_data.transform[idx]);
		//_data.material[idx]->get_program()->set_uniform("model", model);

		//_data.mesh[idx]->update();
		//_data.mesh[idx]->draw_elements();
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
