#include "dbg_rend.h"
#include "loader.h"

#include "imgui.h"

#include <GL/glew.h>

namespace efiilj
{
	debug_renderer::debug_renderer()
		: _shader(-1)
	{
		printf("Init debug...\n");
		_name = "Debug renderer";
	}

	debug_renderer::~debug_renderer()
	{
		printf("Debug renderer exit\n");
	}
	
	void debug_renderer::create_bbox_mesh(debug_id idx)
	{
		mesh_id mid = _meshes->create_primitive(primitive::bbox);
		_meshes->build(mid, GL_DYNAMIC_DRAW);
		_meshes->set_triangle_mode(mid, GL_LINES);
		_data.bbox[idx] = mid;
	}
	
	void debug_renderer::update_bbox_mesh(debug_id idx)
	{
		entity_id eid = get_entity(idx);
		mesh_id mid = _data.bbox[idx];

		collider_id col_id = _colliders->get_component(eid);

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return;

		if (_colliders->is_valid(col_id))
		{
			bounds bt = _colliders->get_bounds_world(col_id);

			std::vector<vector3> positions = {
				vector3(bt.max.x, bt.min.y, bt.min.z),
				vector3(bt.max.x, bt.max.y, bt.min.z),
				vector3(bt.min.x, bt.max.y, bt.min.z),
				vector3(bt.min.x, bt.min.y, bt.min.z),
				vector3(bt.max.x, bt.min.y, bt.max.z),
				vector3(bt.max.x, bt.max.y, bt.max.z),
				vector3(bt.min.x, bt.min.y, bt.max.z),
				vector3(bt.min.x, bt.max.y, bt.max.z)
			};

			_meshes->set_positions(mid, positions);
			_meshes->update(mid);
		}
	}

	void debug_renderer::extend_defaults(debug_id idx)
	{
		_data.bbox.emplace_back(-1);
		_data.draw_bounds.emplace_back(true);
		
		create_bbox_mesh(idx);
	}

	void debug_renderer::draw_gui()
	{

	}

	void debug_renderer::draw_gui(debug_id idx)
	{
		if (!is_valid(idx))
			return;

		ImGui::Text("Debug instance %d", idx);
		ImGui::Text("Shader instance %d", _shader);

		bool draw_bounds = _data.draw_bounds[idx];
		if (ImGui::Checkbox("Draw collider bounds", &draw_bounds))
			_data.draw_bounds[idx] = draw_bounds;
	}

	void debug_renderer::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
		_lights = host->get_manager_from_fcc<light_manager>('LGHT');
		_cameras = host->get_manager_from_fcc<camera_manager>('CAMS');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
		_material_instances = host->get_manager_from_fcc<material_manager>('MAMR');

		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_materials = host->get_manager_from_fcc<material_server>('MASR');
		_shaders = host->get_manager_from_fcc<shader_server>('SHDR');
		_colliders = host->get_manager_from_fcc<collider_manager>('RAYS');
		_sim = host->get_manager_from_fcc<simulator>('PHYS');

		object_loader loader("../res/volumes/v_pointlight.obj", _meshes);
		sphere = loader.get_mesh();
	}

	void debug_renderer::render(debug_id idx)
	{

		if (!_shaders->use(_shader))
			return;

		mesh_id mid = _data.bbox[idx];
		_meshes->bind(mid);

		entity_id eid = get_entity(idx);

		collider_id col_id = _colliders->get_component(eid);

		if (!_colliders->is_valid(col_id))
			return;

		bool collision = _colliders->test_narrow(col_id);

		vector4 color(1,1,1,1);
		if (_colliders->test_broad(col_id))
		{
			color = collision ? vector4(1,0,0,1) : vector4(1,1,0,1);
		}

		_shaders->set_uniform("base_color_factor", color);
		_shaders->set_uniform("model", matrix4());
		_meshes->draw_elements(mid);

		const auto& collisions = _colliders->get_collisions(col_id);

		transform_id trf_id = _transforms->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return;

		vector3 pos = _transforms->get_position(trf_id);
		matrix4 s = matrix4::get_scale(0.01f);
		_meshes->bind(sphere);

		for (const auto& col : collisions)
		{
			matrix4 t = matrix4::get_translation(vector4(col.point1, 1.0f));
			_shaders->set_uniform("model", t * s);
			_shaders->set_uniform("base_color_factor", vector4(0, 1.0f, 0, 1.0f));
			_meshes->draw_elements(sphere);

			t = matrix4::get_translation(vector4(col.point2, 1.0f));
			_shaders->set_uniform("model", t * s);
			_shaders->set_uniform("base_color_factor", vector4(1.0f, 0, 1.0f, 1.0f));
			_meshes->draw_elements(sphere);
		}

		physics_id phys_id = _sim->get_component(eid);

		if (!_sim->is_valid(phys_id))
			return;

		vector3 com = pos + _sim->get_com(phys_id);

		matrix4 t = matrix4::get_translation(vector4(com, 1.0f));
		_shaders->set_uniform("model", t * s);
		_shaders->set_uniform("base_color_factor", vector4(0, 0, 1.0f, 1.0f));
		_meshes->draw_elements(sphere);

	}

	void debug_renderer::begin_frame()
	{
		_shaders->use(_shader);	
	}

	void debug_renderer::render_frame()
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		for (const auto& idx : _instances)
		{
			if (_data.draw_bounds[idx])
			{
				update_bbox_mesh(idx);
				render(idx);
			}
		}
	}

	void debug_renderer::end_frame()
	{
		_meshes->unbind();
	}

}
