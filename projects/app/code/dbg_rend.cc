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

	void debug_renderer::create_line_mesh()
	{
		line = _meshes->create_primitive(primitive::line);
		_meshes->build(line, GL_DYNAMIC_DRAW);
		_meshes->set_triangle_mode(line, GL_LINES);
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
		_data.draw_com.emplace_back(true);
		_data.draw_penetration.emplace_back(true);
		_data.draw_impulses.emplace_back(true);
		_data.draw_minowski.emplace_back(true);
		
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

		bool draw_com = _data.draw_com[idx];
		if (ImGui::Checkbox("Draw center-of-mass", &draw_com))
			_data.draw_com[idx] = draw_com;

		bool draw_penetration = _data.draw_penetration[idx];
		if (ImGui::Checkbox("Draw penetration", &draw_penetration))
			_data.draw_penetration[idx] = draw_penetration;

		bool draw_impulses = _data.draw_impulses[idx];
		if (ImGui::Checkbox("Draw impulses", &draw_impulses))
			_data.draw_impulses[idx] = draw_impulses;

		bool draw_minowski = _data.draw_minowski[idx];
		if (ImGui::Checkbox("Draw minowski regions", &draw_minowski))
			_data.draw_minowski[idx] = draw_minowski;
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

		create_line_mesh();

	}

	void debug_renderer::render(debug_id idx)
	{

		if (!_shaders->use(_shader))
			return;

		const vector4 white(1, 1, 1, 1);
		const vector4 red(1, 0, 0, 1);
		const vector4 green(0, 1, 0, 1);
		const vector4 blue(0, 0, 1, 1);
		const vector4 yellow(1, 1, 0, 1);
		const vector4 pink(1, 0, 1, 1);

		mesh_id bbox = _data.bbox[idx];
		_meshes->bind(bbox);

		entity_id eid = get_entity(idx);

		collider_id col_id = _colliders->get_component(eid);

		if (!_colliders->is_valid(col_id))
			return;

		bool collision = _colliders->test_narrow(col_id);

		vector4 color = white;
		if (_colliders->test_broad(col_id))
		{
			color = collision ? red : yellow;
		}

		if (_data.draw_bounds[idx])
		{
			_shaders->set_uniform("base_color_factor", color);
			_shaders->set_uniform("model", matrix4());
			_meshes->draw_elements(bbox);
		}

		const auto& collisions = _colliders->get_collisions(col_id);

		transform_id trf_id = _transforms->get_component(eid);
		physics_id phys_id = _sim->get_component(eid);

		if (!_transforms->is_valid(trf_id))
			return;

		if (!_sim->is_valid(phys_id))
			return;

		vector3 pos = _transforms->get_position(trf_id);
		vector3 com = pos + _sim->get_com(phys_id);

		if (_data.draw_com[idx])
			draw_sphere(com, 0.02f, blue);

		for (const auto& col : collisions)
		{
			if (_data.draw_penetration[idx])
			{
				draw_sphere(col.point1, 0.01f, green);
				draw_sphere(col.point2, 0.01f, pink);
				draw_line(com, com + col.normal * col.depth, yellow);
				draw_line(col.point1, col.point2, white);
			}

			auto face = col.face;

			if (_data.draw_minowski[idx])
			{
				draw_line(face.a.s1, face.b.s1, green);
				draw_line(face.b.s1, face.c.s1, green);
				draw_line(face.c.s1, face.a.s1, green);

				draw_line(face.a.s2, face.b.s2, pink);
				draw_line(face.b.s2, face.c.s2, pink);
				draw_line(face.c.s2, face.a.s2, pink);

				draw_line(face.a.point, face.b.point, yellow);
				draw_line(face.b.point, face.c.point, yellow);
				draw_line(face.c.point, face.a.point, yellow);
			}
		}

		if (_data.draw_impulses[idx])
		{
			const auto& impulses = _sim->get_impulses(phys_id);
			for (const auto& impulse : impulses)
			{
				draw_line(impulse.p, impulse.p + impulse.force * 100.0f, green);
			}
		}
	}

	void debug_renderer::draw_line(const vector3& a, const vector3& b, const vector4& color) const
	{
		_shaders->set_uniform("base_color_factor", color);
		draw_line(a, b);
	}

	void debug_renderer::draw_line(const vector3& a, const vector3& b) const
	{
		_meshes->bind(line);
		_shaders->set_uniform("model", matrix4());
		std::vector<vector3> ab = { a, b };
		_meshes->set_positions(line, ab);
		_meshes->update(line);
		_meshes->draw_elements(line);
	}

	void debug_renderer::draw_sphere(const vector3& pos, float size, const vector4& color) const
	{
		_shaders->set_uniform("base_color_factor", color);
		draw_sphere(pos, size);
	}

	void debug_renderer::draw_sphere(const vector3& pos, float size) const
	{
		_meshes->bind(sphere);
		matrix4 t = matrix4::get_translation(vector4(pos, 1.0f));
		matrix4 s = matrix4::get_scale(size);
		_shaders->set_uniform("model", t * s);
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
