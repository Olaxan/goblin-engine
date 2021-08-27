//------------------------------------------------------------------------------
// exampleapp.cc // (C) 2015-2018 Individual contributors, see AUTHORS file
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
#include "gltf_loader.h"
#include "quat.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768
#define CAMERA_SPEED 0.05f
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
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//lock mouse to window
			glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// set clear color
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			this->window_->SetUiRender([this]()
					{
						ImGui::Begin("Managers");
						editor->show_entity_gui();
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
		set.default_fallback_path_primary = "../res/shaders/default_primary.sdr";
		set.default_fallback_path_secondary = "../res/shaders/default_secondary.sdr";

		managers = std::make_shared<manager_host>();

		entities = std::make_shared<entity_manager>();

		shaders = std::make_shared<shader_server>();
		textures = std::make_shared<texture_server>();
		meshes = std::make_shared<mesh_server>();
		materials = std::make_shared<material_server>();

		metadata = std::make_shared<meta_manager>();
		transforms = std::make_shared<transform_manager>();
		cameras = std::make_shared<camera_manager>();
		lights = std::make_shared<light_manager>();
		graphics = std::make_shared<graphics_manager>();
		material_instances = std::make_shared<material_manager>();
		mesh_instances = std::make_shared<mesh_manager>();

		rdef = std::make_shared<deferred_renderer>(set);
		rfwd = std::make_shared<forward_renderer>(set);
		sim = std::make_shared<simulator>();
		colliders = std::make_shared<collider_manager>();
		gltf = std::make_shared<gltf_model_server>();

		managers->register_manager(entities, 'ENTS');
		managers->register_manager(transforms, 'TRFM');
		managers->register_manager(metadata, 'META');

		managers->register_manager(shaders, 'SHDR');
		managers->register_manager(textures, 'TXSR');
		managers->register_manager(meshes, 'MESR');
		managers->register_manager(materials, 'MASR');

		managers->register_manager(cameras, 'CAMS');
		managers->register_manager(lights, 'LGHT');
		managers->register_manager(graphics, 'GRFX');
		managers->register_manager(mesh_instances, 'MEMR');
		managers->register_manager(material_instances, 'MAMR');

		managers->register_manager(rfwd, 'RFWD');
		managers->register_manager(rdef, 'RDEF');
		managers->register_manager(sim, 'PHYS');
		managers->register_manager(colliders, 'RAYS');
		managers->register_manager(gltf, 'GLTF');

		editor = std::make_shared<entity_editor>(entities, managers);

		// Camera entity

		entity_id cam_ent = entities->create();
		transform_id cam_trf_id = transforms->register_entity(cam_ent);
		camera_id cam_cam_id = cameras->register_entity(cam_ent);
		meta_id cam_meta_id = metadata->register_entity(cam_ent);

		cameras->set_transform(cam_cam_id, cam_trf_id);
		cameras->set_size(cam_cam_id, WINDOW_WIDTH, WINDOW_HEIGHT);
		cameras->set_fov(cam_cam_id, 1.2f);
		cameras->set_camera(cam_cam_id);

		metadata->set_name(cam_meta_id, "Main camera");
		metadata->set_description(cam_meta_id, "This is the main camera of the application.");

		model_id test_mdl = gltf->create();
		gltf->set_uri(test_mdl, "../res/gltf/FlightHelmet/FlightHelmet.gltf");
		gltf->load(test_mdl);
		gltf->get_nodes(test_mdl);

		for (const auto& node : gltf->get_scene(test_mdl).nodes)
		{
			rdef->register_entity(node);
			colliders->register_entity(node);
		}

		gltf->unload(test_mdl);

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

				camera_id active = cameras->get_camera();
				ray r = cameras->get_ray_from_camera(active, mouse_x, mouse_y);

				vector3 hit, norm;
				if (colliders->test_hit(r, hit, norm))
				{
					printf("Hit! At %s\n", hit.to_mem_string().c_str());
				}
			}
			else
			{
				is_dragging_mouse = false;
			}
		});

		while (this->window_->IsOpen())
		{

			//transform_id selected_trf = is_mouse_captured ? cam_trf_id : e1_trf;
			transform_id selected_trf = cam_trf_id;

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
			colliders->update();

			rdef->begin_frame();
		    rfwd->begin_frame();

			rdef->render_frame();
		    rfwd->render_frame();

			rdef->end_frame();
			rfwd->end_frame();

			this->window_->SwapBuffers();
			
		}
	}
}
