//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <filesystem>

#include "imgui.h"

#include "app.h"
#include "loader.h"
#include "light.h"
#include "material.h"
#include "node.h"
#include "gltf_loader.h"
#include "program.h"
#include "rect.h"
#include "line.h"
#include "cube.h"
#include "bbox.h"
#include "quat.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768
#define CAMERA_SPEED 0.5f
#define NUM_LIGHTS 10
#define PI 3.14159f

namespace fs = std::filesystem;

float randf(float max = 1.0f)
{
	return static_cast<float>(rand()) / static_cast <float> (static_cast<float>(RAND_MAX) / max);
}

float randf(float min, float max)
{
	return min + static_cast<float>(rand()) / ( static_cast<float>(static_cast<float>(RAND_MAX) / (max - min)));
}

namespace efiilj
{

	application::application()
	: window_(nullptr), time_(0), 
		mouse_x(0), mouse_y(0), mouse_down_x(0), mouse_down_y(0), mouse_norm_x(0), mouse_norm_y(0), 
		is_dragging_mouse(false), is_mouse_captured(true) { }

	application::~application() = default;

	bool application::open()
	{
		app::open();

		this->window_ = new Display::Window(WINDOW_WIDTH, WINDOW_HEIGHT);

		if (this->window_->Open())
		{

			//enable face culling
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			//lock mouse to window
			glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// set clear color
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			this->window_->SetUiRender([this]()
					{
						ImGui::Begin("Managers");
						cameras->draw_gui();
						lights->draw_gui();
						rfwd->draw_gui();
						rdef->draw_gui();
						ImGui::End();
					});

			return true;
		}
		return false;
	}

	void application::run()
	{

		renderer_settings set;
		set.width = WINDOW_WIDTH;
		set.height = WINDOW_HEIGHT;
		set.default_primary_path = "../res/shaders/default_primary.sdr";
		set.default_secondary_path = "../res/shaders/default_secondary.sdr";

		managers = std::make_shared<manager_host>();

		entities = std::make_shared<entity_manager>();

		shaders = std::make_shared<shader_server>();
		textures = std::make_shared<texture_server>();
		meshes = std::make_shared<mesh_server>();
		materials = std::make_shared<material_server>();

		transforms = std::make_shared<transform_manager>();
		cameras = std::make_shared<camera_manager>();
		lights = std::make_shared<light_manager>();
		graphics = std::make_shared<graphics_manager>();
		material_instances = std::make_shared<material_manager>();
		mesh_instances = std::make_shared<mesh_manager>();

		rdef = std::make_shared<deferred_renderer>(set);
		rfwd = std::make_shared<forward_renderer>(set);
		sim = std::make_shared<simulator>();
		gltf = std::make_shared<gltf_model_server>();

		managers->register_manager(entities, 'ENTS');
		managers->register_manager(transforms, 'TRFM');

		managers->register_manager(shaders, 'SHDR');
		managers->register_manager(textures, 'TXSR');
		managers->register_manager(meshes, 'MESR');
		managers->register_manager(materials, 'MASR');

		managers->register_manager(cameras, 'CAMS');
		managers->register_manager(lights, 'LGHT');
		managers->register_manager(graphics, 'GRFX');
		managers->register_manager(mesh_instances, 'MEMR');
		managers->register_manager(material_instances, 'MAMR');

		managers->register_manager(rdef, 'RDEF');
		managers->register_manager(rfwd, 'RFWD');
		managers->register_manager(sim, 'PHYS');
		managers->register_manager(gltf, 'GLTF');

		// Camera entity

		entity_id cam_ent = entities->create();
		transform_id cam_trf = transforms->register_entity(cam_ent);
		camera_id cam_id = cameras->register_entity(cam_ent);

		cameras->set_transform(cam_id, cam_trf);
		cameras->set_size(cam_id, WINDOW_WIDTH, WINDOW_HEIGHT);
		cameras->set_fov(cam_id, 1.2f);
		cameras->set_camera(cam_id);

		// Test entity 1

		entity_id e1 = entities->create();
		transform_id e1_trf = transforms->register_entity(e1);
		transforms->set_scale(e1_trf, 5.0f);

		shader_id e1_sdr = shaders->create();
		shaders->set_uri(e1_sdr, "../res/shaders/default_color.sdr");
		shaders->compile(e1_sdr);

		model_id test_mdl = gltf->create();
		gltf->set_uri(test_mdl, "../res/gltf/FlightHelmet/FlightHelmet.gltf");
		gltf->load(test_mdl);
		gltf->get_nodes(test_mdl);
		gltf->unload(test_mdl);

		//model_id e1_gltf = gltf->create();
		//graphics_id e1_grph = graphics->create();

		//gltf->set_path(e1_gltf, "../res/gltf/FlightHelmet/FlightHelmet.gltf");
		//gltf->open(e1_gltf);
		//gltf->get_nodes(e1_gltf);
		//gltf->close(e1_gltf);

		//auto rect_mat_ptr = std::make_shared<material_base>(color_prog_ptr);
		//rect_mat_ptr->color = vector4(randf(), randf(), randf(), 1.0f);
		//rect_mat_ptr->wireframe = true;

		//object_loader obj_sphere("../res/volumes/v_pointlight.obj");
		//auto sphere_mesh_ptr = obj_sphere.get_resource();
		//auto cube_mesh_ptr = std::make_shared<cube>();

		//render_id e1_fwd = rfwd->register_entity(e1);
		//rfwd->set_mesh(e1_fwd, sphere_mesh_ptr);
		//rfwd->set_material(e1_fwd, rect_mat_ptr);
		//rfwd->set_transform(e1_fwd, e1_trf);

		//entity_id e2 = entities->create();
		//transform_id e2_trf = transforms->register_entity(e2);
		//transforms->set_scale(e2_trf, 5.0f);
		////render_id e2_gfx = rdef->register_entity(e2);

		// Lights
		
		entity_id e_sun = entities->create();
		light_id sun_light = lights->register_entity(e_sun);
		lights->set_base(sun_light, vector3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);

		transform_id sun_trf = transforms->register_entity(e_sun);
		transforms->add_rotation(sun_trf, vector3(1, 1, 0).norm(), PI / 2.0f);
		lights->set_transform(sun_light, sun_trf);

		std::set<int> keys;
		
		window_->SetKeyPressFunction([&](const int key, int, const int action, int)
		{
			if (action == 1)
			{
				keys.emplace(key);

				if (key == GLFW_KEY_TAB)
				{
					is_mouse_captured = !is_mouse_captured;

					//if (is_mouse_captured_)
					//	window_->SetCursorMode(GLFW_CURSOR_DISABLED);
					//else
					//	window_->SetCursorMode(GLFW_CURSOR_NORMAL);

				}
			}
			else if (action == 0)
			{
				const auto it = keys.find(key);
				if (it != keys.end())
					keys.erase(it);
			}
		});

			window_->SetMouseMoveFunction([&](const double x, const double y)
		{
			mouse_x = x;
			mouse_y = y;

			mouse_norm_x = x / WINDOW_WIDTH - 0.5f;
			mouse_norm_y = y / WINDOW_HEIGHT - 0.5f;

		});

		window_->SetMousePressFunction([&](const int button, const int action, int mods)  //NOLINT
		{
			if (action == 1)
			{
				mouse_down_x = mouse_norm_x;
				mouse_down_y = mouse_norm_y;

				is_dragging_mouse = true;

				//auto camera_ptr = _cam_mgr->get_active_camera();
				//ray r = camera_ptr->get_ray_from_camera(mouse_x_, mouse_y_);
				//float nearest = std::numeric_limits<float>::max();
				//vector3 hit, norm;

				//for (auto body : _simulator->get_rigidbodies())
				//{
				//	if (body->ray_intersect_triangle(r, hit, norm))
				//	{
				//		float len = (hit - r.origin).length();
				//		if (len < nearest)
				//		{
				//			nearest = len;
				//		}
				//	}
				//}
			}
			else
			{
				is_dragging_mouse = false;
			}
		});

		while (this->window_->IsOpen())
		{

			transform_id selected_trf = is_mouse_captured ? cam_trf : e1_trf;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->window_->Update();
	
			if (keys.find(GLFW_KEY_W) != keys.end())
				transforms->add_position(selected_trf, transforms->get_forward(selected_trf) * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_S) != keys.end())
				transforms->add_position(selected_trf, transforms->get_backward(selected_trf) * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_A) != keys.end())
				transforms->add_position(selected_trf, transforms->get_left(selected_trf) * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_D) != keys.end())
				transforms->add_position(selected_trf, transforms->get_right(selected_trf) * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_SPACE) != keys.end())
				transforms->add_position(selected_trf, transforms->get_up(selected_trf) * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_LEFT_SHIFT) != keys.end())
				transforms->add_position(selected_trf, transforms->get_down(selected_trf) * CAMERA_SPEED);

			if (keys.find(GLFW_KEY_UP) != keys.end())
				transforms->add_rotation(selected_trf, transforms->get_right(selected_trf), -0.1f);

			if (keys.find(GLFW_KEY_DOWN) != keys.end())
				transforms->add_rotation(selected_trf, transforms->get_right(selected_trf), 0.1f);

			if (keys.find(GLFW_KEY_RIGHT) != keys.end())
				transforms->add_rotation(selected_trf, transforms->get_up(selected_trf), -0.1f);

			if (keys.find(GLFW_KEY_LEFT) != keys.end())
				transforms->add_rotation(selected_trf, transforms->get_up(selected_trf), 0.1f);

			if (keys.find(GLFW_KEY_PAGE_UP) != keys.end())
				transforms->add_rotation(selected_trf, transforms->get_forward(selected_trf), -0.1f);

			if (keys.find(GLFW_KEY_PAGE_DOWN) != keys.end())
				transforms->add_rotation(selected_trf, transforms->get_forward(selected_trf), 0.1f);
			
			if (keys.find(GLFW_KEY_ESCAPE) != keys.end())
				window_->Close();

			cameras->update();

			rdef->begin_frame();
			rdef->render_frame();
			rdef->end_frame();

			rfwd->begin_frame();
			rfwd->render_frame();
			rfwd->end_frame();

			this->window_->SwapBuffers();
			
		}
	}
}
