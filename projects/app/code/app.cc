//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"

#include "app.h"
#include "loader.h"
#include "light.h"
#include "material.h"
#include "node.h"
#include "gltf_loader.h"
#include "program.h"
#include "def_rend.h"
#include "fwd_rend.h"
#include "cam_mgr.h"
#include "imgui.h"
#include "rect.h"
#include "line.h"
#include "math_test.h"
#include "cube.h"
#include "bbox.h"

#include "quat.h"

#include "gui_matedit.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"

#include "entity.h"
#include "trfm_mgr.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <set>

#define flase false
#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768
#define CAMERA_SPEED 0.5f
#define NUM_LIGHTS 10
#define PI 3.14159f

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
		mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), mouse_norm_x_(0), mouse_norm_y_(0), 
		is_dragging_mouse_(false), is_mouse_captured_(true), is_software_renderer_(flase) { }

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
						if (_selected_node != nullptr)
						{
							auto mat = _selected_node->get_material();
							if (mat != nullptr)
							{
								ImGui::Begin("Node");
								_selected_node->draw_node_gui();
								ImGui::End();

								ImGui::Begin("Material");
								mat->draw_material_gui();
								ImGui::End();
							}
						}

						ImGui::Begin("Light");
						_def_renderer->draw_renderer_gui();
						ImGui::End();

						ImGui::Begin("Camera");
						_cam_mgr->get_active_camera()->get_transform()->draw_transform_gui();
						ImGui::End();
					});

			return true;
		}
		return false;
	}

	void application::run()
	{

		auto entities = std::make_shared<entity_manager>();
		auto transforms = std::make_shared<transform_manager>();

		auto g_vs = std::make_shared<shader_resource>(GL_VERTEX_SHADER, "../res/shaders/dvs_geometry.glsl");
		auto g_fs = std::make_shared<shader_resource>(GL_FRAGMENT_SHADER, "../res/shaders/dfs_geometry.glsl");
		auto g_prog_ptr = std::make_shared<shader_program>(g_vs, g_fs);

		auto l_vs = std::make_shared<shader_resource>(GL_VERTEX_SHADER, "../res/shaders/dvs_lighting.glsl");
		auto l_fs = std::make_shared<shader_resource>(GL_FRAGMENT_SHADER, "../res/shaders/dfs_lighting.glsl");
		auto l_prog_ptr = std::make_shared<shader_program>(l_vs, l_fs);

		auto gltf_vs = std::make_shared<shader_resource>(GL_VERTEX_SHADER, "../res/shaders/vs_gltf.glsl");
		auto gltf_fs = std::make_shared<shader_resource>(GL_FRAGMENT_SHADER, "../res/shaders/fs_gltf.glsl");
		auto gltf_prog_ptr = std::make_shared<shader_program>(gltf_vs, gltf_fs);

		auto color_vs = std::make_shared<shader_resource>(GL_VERTEX_SHADER, "../res/shaders/vs_color.glsl");
		auto color_fs = std::make_shared<shader_resource>(GL_FRAGMENT_SHADER, "../res/shaders/fs_color.glsl");
		auto color_prog_ptr = std::make_shared<shader_program>(color_vs, color_fs);

		_cam_mgr = std::make_shared<camera_manager>(WINDOW_WIDTH, WINDOW_HEIGHT);
		
		renderer_settings set;

		set.width = WINDOW_WIDTH;
		set.height = WINDOW_HEIGHT;

		_def_renderer = std::make_shared<deferred_renderer>(_cam_mgr, set, g_prog_ptr, l_prog_ptr);
		_fwd_renderer = std::make_shared<forward_renderer>(_cam_mgr, set);
		_simulator = std::make_shared<simulator>();
		
		object_loader obj_sphere("../res/volumes/v_pointlight.obj");
		auto sphere_mesh_ptr = obj_sphere.get_resource();
		auto cube_mesh_ptr = std::make_shared<cube>();
		auto rect_mesh_ptr = std::make_shared<rect>();

		entity_id e1 = entities->create_entity();
		transform_id trf1 = transforms->register_entity(e1);
		transforms->set_scale(trf1, 25.0f);

		auto rect_mat_ptr = std::make_shared<material_base>(color_prog_ptr);
		rect_mat_ptr->color = vector4(randf(), randf(), randf(), 1.0f);

		auto e1_gfx = std::make_shared<graphics_node>(cube_mesh_ptr, rect_mat_ptr, trf1);
		_def_renderer->add_node(e1_gfx);	

		auto sun_ptr = std::make_shared<light_source>(
				std::make_shared<transform_model>(vector3(0), vector3(PI / 2, PI / 2, 0)), 
				light_type::directional);

		sun_ptr->set_base(vector3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);

		_def_renderer->add_light(sun_ptr);

		std::set<int> keys;
		
		window_->SetKeyPressFunction([&](const int key, int, const int action, int)
		{
			if (action == 1)
			{
				keys.emplace(key);

				if (key == GLFW_KEY_TAB)
				{
					is_mouse_captured_ = !is_mouse_captured_;

					//if (is_mouse_captured_)
					//	window_->SetCursorMode(GLFW_CURSOR_DISABLED);
					//else
					//	window_->SetCursorMode(GLFW_CURSOR_NORMAL);

				}
				else if (key == GLFW_KEY_LEFT_CONTROL)
				{
					is_software_renderer_ = !is_software_renderer_;
				}
				else if (key == GLFW_KEY_R)
				{
					_def_renderer->reload_shaders();
					_fwd_renderer->reload_shaders();
				}
				else if (key == GLFW_KEY_B)
				{
					auto& bodies = _simulator->get_rigidbodies();

					for (auto& body : bodies)
					{
						auto bbox_mesh = std::make_shared<bbox>(body);
						auto bbox_node = std::make_shared<graphics_node>(bbox_mesh, rect_mat_ptr);
						_fwd_renderer->add_node(bbox_node);
					}
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
			mouse_x_ = x;
			mouse_y_ = y;

			mouse_norm_x_ = x / WINDOW_WIDTH - 0.5f;
			mouse_norm_y_ = y / WINDOW_HEIGHT - 0.5f;

		});

		window_->SetMousePressFunction([&](const int button, const int action, int mods) 
		{
			if (action == 1)
			{
				mouse_down_x_ = mouse_norm_x_;
				mouse_down_y_ = mouse_norm_y_;

				is_dragging_mouse_ = true;

				auto camera_ptr = _cam_mgr->get_active_camera();
				ray r = camera_ptr->get_ray_from_camera(mouse_x_, mouse_y_);
				float nearest = std::numeric_limits<float>::max();
				vector3 hit, norm;

				for (auto body : _simulator->get_rigidbodies())
				{
					if (body->ray_intersect_triangle(r, hit, norm))
					{
						float len = (hit - r.origin).length();
						if (len < nearest)
						{
							nearest = len;
						}
					}
				}
			}
			else
			{
				is_dragging_mouse_ = false;
			}
		});

		while (this->window_->IsOpen())
		{
			auto camera_ptr = _cam_mgr->get_active_camera();
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->window_->Update();
	
			if (keys.find(GLFW_KEY_W) != keys.end())
				transforms->add_position(transforms->forward() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_S) != keys.end())
				transforms->add_position(transforms->backward() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_A) != keys.end())
				transforms->add_position(transforms->left() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_D) != keys.end())
				transforms->add_position(transforms->right() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_SPACE) != keys.end())
				transforms->add_position(transforms->up() * CAMERA_SPEED);
			
			if (keys.find(GLFW_KEY_LEFT_SHIFT) != keys.end())
				transforms->add_position(transforms->down() * CAMERA_SPEED);

			if (keys.find(GLFW_KEY_UP) != keys.end())
				transforms->add_rotation(transforms->right(), -0.1f);

			if (keys.find(GLFW_KEY_DOWN) != keys.end())
				transforms->add_rotation(transforms->right(), 0.1f);

			if (keys.find(GLFW_KEY_LEFT) != keys.end())
				transforms->add_rotation(transforms->up(), 0.1f);

			if (keys.find(GLFW_KEY_RIGHT) != keys.end())
				transforms->add_rotation(transforms->up(), -0.1f);

			if (keys.find(GLFW_KEY_PAGE_DOWN) != keys.end())
				transforms->add_rotation(transforms->forward(), 0.1);

			if (keys.find(GLFW_KEY_PAGE_UP) != keys.end())
				transforms->add_rotation(transforms->forward(), -0.1);
			
			if (keys.find(GLFW_KEY_ESCAPE) != keys.end())
				window_->Close();

			if (keys.find(GLFW_KEY_LEFT_CONTROL) != keys.end())
			{
				_def_renderer->toggle_debug();
				_fwd_renderer->toggle_debug();
			}

			_cam_mgr->update_camera();

			float dt = _def_renderer->get_delta_time();
			float d = _def_renderer->get_frame_index() / 100.0f;

			_def_renderer->begin_frame();
			_fwd_renderer->begin_frame();

			_def_renderer->render();
			_fwd_renderer->render();

			_def_renderer->end_frame();
			_fwd_renderer->end_frame();

			this->window_->SwapBuffers();
			
		}
	}
}

#undef flase
