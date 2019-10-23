//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "quadtest.h"
#include "camera.h"
#include "loader.h"
#include "light.h"
#include "node.h"
#include "swrast.h"
#include "bufrend.h"

#include <iostream>
#include <set>


namespace efiilj
{

	quad_test::quad_test()
	: window_(nullptr), time_(0), mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), is_dragging_mouse_(false), is_mouse_captured_(true) { }

	quad_test::~quad_test() = default;

	bool quad_test::open()
	{
		app::open();

		t_start_ = std::chrono::high_resolution_clock::now();

		this->window_ = new Display::Window(1024, 1024);

		if (this->window_->Open())
		{

			//enable face culling
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			//lock mouse to window
			glfwSetInputMode(window_->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// set clear color to purple
			glClearColor(0.01f, 0.0f, 0.01f, 0.5f);

			return true;
		}
		return false;
	}

	void quad_test::run()
	{

		float fov = nvgDegToRad(75);

		object_loader fox_loader = object_loader("./res/meshes/fox.obj2");
		object_loader rock_loader = object_loader("./res/meshes/rock.obj2");

		std::string fs = shader_resource::load_shader("./res/shaders/vertex.shader");
		std::string vs = shader_resource::load_shader("./res/shaders/fragment.shader");
		
		if (!fox_loader.is_valid() || !rock_loader.is_valid())
		{
			std::cout << "\nFailed to load OBJ file - program will terminate.\n";
			return;
		}

		std::cout << "Loaded " << fox_loader.vertex_count() << " vertices, " << fox_loader.index_count() << " indices\n";
		std::cout << "Loaded " << rock_loader.vertex_count() << " vertices, " << rock_loader.index_count() << " indices\n";

		mesh_resource fox_model = fox_loader.get_resource();
		mesh_resource rock_model = rock_loader.get_resource();

		auto fox_mesh_ptr = std::make_shared<mesh_resource>(fox_model);
		auto rock_mesh_ptr = std::make_shared<mesh_resource>(rock_model);

		auto fox_texture_ptr = std::make_shared<texture_resource>("./res/textures/fox_base.png", true);
		auto rock_texture_ptr = std::make_shared<texture_resource>("./res/textures/rock_base.png", true);

		//auto fox_trans_ptr = std::make_shared<transform_model>(vector3(0, 0.5f, 0), vector3(0), vector3(0.1f, 0.1f, 0.1f));
		auto fox_trans_ptr = std::make_shared<transform_model>(vector3(0, 0.5f, 0), vector3(0), vector3(0.1f, 0.1f, 0.1f));
		auto rock_trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(1.6), vector3(0.005f, 0.005f, 0.005f));

		auto camera_trans_ptr = std::make_shared<transform_model>(vector3(0, 2, 2), vector3(0), vector3(1, 1, 1));
		auto camera_ptr = std::make_shared<camera_model>(fov, 1.0f, 0.1f, 100.0f, camera_trans_ptr, vector3(0, 1, 0));

		auto shader_ptr = std::make_shared<shader_resource>(fs, vs);

		point_light p_light = point_light(vector3(0.5f, 0.5f, 0.5f), vector3(1.0f, 1.0f, 1.0f), vector3(2, 2, 2));
		
		graphics_node fox_node(fox_mesh_ptr, fox_texture_ptr, shader_ptr, fox_trans_ptr, camera_ptr);
		graphics_node rock_node(rock_mesh_ptr, rock_texture_ptr, shader_ptr, rock_trans_ptr, camera_ptr);

		/*TEST OF SOFTWARE RENDERER*/
		auto rasterizer_ptr = std::make_shared<rasterizer>(1024, 1024, camera_ptr);
		auto node_ptr = std::make_shared<rasterizer_node>(fox_loader.get_vertices(), fox_loader.get_indices(), fox_trans_ptr);
		rasterizer_ptr->add_node(node_ptr);
		
		auto buffer_renderer_ptr = std::make_shared<buffer_renderer>(rasterizer_ptr);

		std::set<int> keys;
		
		window_->SetKeyPressFunction([&](const int key, int, const int action, int)
			{
				if (action == 1)
				{
					keys.emplace(key);

					if (key == GLFW_KEY_TAB)
					{
						if (is_mouse_captured_)
							window_->SetCursorMode(GLFW_CURSOR_NORMAL);
						else
							window_->SetCursorMode(GLFW_CURSOR_DISABLED);

						is_mouse_captured_ = !is_mouse_captured_;
					}
				}
				else if (action == 0)
				{
					const auto it = keys.find(key);
					if (it != keys.end())
						keys.erase(it);
				}
			});

		window_->SetMouseMoveFunction([&](const float x, const float y)
			{
				mouse_x_ = x / 1000.0f - 0.5f;
				mouse_y_ = y / 1000.0f - 0.5f;

				if (is_mouse_captured_)
					camera_trans_ptr->rotation = vector3(-mouse_y_, mouse_x_, 0);
				else if (is_dragging_mouse_)
					fox_trans_ptr->rotation += vector3(mouse_y_ - mouse_down_y_, mouse_x_ - mouse_down_x_, 0) * 0.5f;
			});

		window_->SetMousePressFunction([&](const int button, const int action, int mods) 
			{
				if (action == 1)
				{
					mouse_down_x_ = mouse_x_;
					mouse_down_y_ = mouse_y_;
					is_dragging_mouse_ = true;
				}
				else
				{
					is_dragging_mouse_ = false;
				}
			});
		
		while (this->window_->IsOpen())
		{

			t_now_ = std::chrono::high_resolution_clock::now();
			time_ = std::chrono::duration_cast<std::chrono::duration<float>>(t_now_ - t_start_).count();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window_->Update();

			if (keys.find(GLFW_KEY_W) != keys.end())
				camera_trans_ptr->position+= camera_trans_ptr->forward() * 0.1f;
			
			if (keys.find(GLFW_KEY_S) != keys.end())
				camera_trans_ptr->position -= camera_trans_ptr->forward() * 0.1f;
			
			if (keys.find(GLFW_KEY_A) != keys.end())
				camera_trans_ptr->position += camera_trans_ptr->left() * 0.1f;
			
			if (keys.find(GLFW_KEY_D) != keys.end())
				camera_trans_ptr->position -= camera_trans_ptr->left() * 0.1f;
			
			if (keys.find(GLFW_KEY_SPACE) != keys.end())
				camera_trans_ptr->position += camera_trans_ptr->up() * 0.1f;
			
			if (keys.find(GLFW_KEY_LEFT_SHIFT) != keys.end())
				camera_trans_ptr->position -= camera_trans_ptr->up() * 0.1f;
			
			if (keys.find(GLFW_KEY_ESCAPE) != keys.end())
				window_->Close();

			p_light.position = vector3(sinf(time_), 2.0f, cosf(time_));
			
			shader_ptr->use();
			shader_ptr->set_uniform("u_camera_position", camera_trans_ptr->position);
			shader_ptr->set_uniform("u_light.color", p_light.rgb);
			shader_ptr->set_uniform("u_light.intensity", p_light.intensity);
			shader_ptr->set_uniform("u_light.position", p_light.position);
			shader_ptr->set_uniform("u_ambient_color", vector3(0.025f, 0, 0.025f));
			shader_ptr->set_uniform("u_ambient_strength", 1.0f);
			shader_ptr->set_uniform("u_specular_strength", 0.5f);
			shader_ptr->set_uniform("u_shininess", 32);
			shader_ptr->drop();
			
			/*fox_node.draw();
			rock_node.draw();*/

			buffer_renderer_ptr->draw();

			this->window_->SwapBuffers();
		}
	}
}