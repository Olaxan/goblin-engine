#include "fwd_rend.h"
#include "mgr_host.h"

#include <GL/glew.h>

namespace efiilj
{
	forward_renderer::forward_renderer(const renderer_settings& set)
		: 
			settings_(set), frame_index_(0), delta_time_() 
	{
		printf("Init forward renderer...\n");
		last_frame_ = frame_timer::now();
		printf("Forward renderer ready\n");
	}  

	render_id forward_renderer::register_entity(entity_id eid)
	{
		render_id new_id = _instances.size();
		_instances.emplace_back(new_id);
		_instance_mapping.emplace(eid, new_id);

		_data.mesh.emplace_back(nullptr);
		_data.material.emplace_back(nullptr);
		_data.transform.emplace_back(-1);

		return new_id;
	}

	bool forward_renderer::unregister_entity(render_id idx)
	{
		return false;
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
	}

	void forward_renderer::set_mesh(render_id idx, std::shared_ptr<mesh_resource> mesh)
	{
		_data.mesh[idx] = std::move(mesh);
	}

	void forward_renderer::set_material(render_id idx, std::shared_ptr<material_base> material)
	{
		_data.material[idx] = std::move(material);
	}

	void forward_renderer::set_transform(render_id idx, transform_id transform)
	{
		_data.transform[idx] = transform;
	}

	void forward_renderer::begin_frame()
	{
		delta_time_ = frame_timer::now() - last_frame_;
		last_frame_ = frame_timer::now();

		on_begin_frame();
	}

	void forward_renderer::render(render_id idx) const
	{
		_data.mesh[idx]->bind();
		_data.material[idx]->apply();
		
		matrix4 model = _transforms->get_model(_data.transform[idx]);
		_data.material[idx]->get_program()->set_uniform("model", model);

		_data.mesh[idx]->update();
		_data.mesh[idx]->draw_elements();
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
		frame_index_++;	

		on_end_frame();
	}
}
