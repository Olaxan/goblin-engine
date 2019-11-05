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
#include "color.h"

#include <iostream>
#include <set>


namespace efiilj
{

	quad_test::quad_test()
	: window_(nullptr), time_(0), mouse_x_(0), mouse_y_(0), mouse_down_x_(0), mouse_down_y_(0), is_dragging_mouse_(false), is_mouse_captured_(true), is_software_renderer_(true) { }

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

		object_loader fox_loader = object_loader("./res/meshes/cube.obj");

		std::string fs = shader_resource::load_shader("./res/shaders/vertex.shader");
		std::string vs = shader_resource::load_shader("./res/shaders/fragment.shader");
		
		if (!fox_loader.is_valid())
		{
			std::cout << "\nFailed to load OBJ file - program will terminate.\n";
			return;
		}

		std::cout << "Loaded " << fox_loader.vertex_count() << " vertices, " << fox_loader.index_count() << " indices\n";

		mesh_resource fox_model = fox_loader.get_resource();
		auto fox_mesh_ptr = std::make_shared<mesh_resource>(fox_model);
		auto fox_texture_ptr = std::make_shared<texture_resource>("./res/textures/fox_base.png", true);
		auto fox_trans_ptr = std::make_shared<transform_model>(vector3(4, 2, 2), vector3(0), vector3(0.1f, 0.1f, 0.1f));
		
		auto camera_trans_ptr = std::make_shared<transform_model>(vector3(0, 2, 2), vector3(0), vector3(1, 1, 1));
		auto camera_ptr = std::make_shared<camera_model>(fov, 1.0f, 0.1f, 100.0f, camera_trans_ptr, vector3(0, 1, 0));

		auto shader_ptr = std::make_shared<shader_resource>(fs, vs);

		point_light p_light = point_light(vector3(0.5f, 0.5f, 0.5f), vector3(1.0f, 1.0f, 1.0f), vector3(2, 2, 2));
		
		graphics_node fox_node(fox_mesh_ptr, fox_texture_ptr, shader_ptr, fox_trans_ptr, camera_ptr);

		/*SOFTWARE RENDERER*/
		auto rasterizer_ptr = std::make_shared<rasterizer>(1024, 1024, camera_ptr, color(0, 0, 10));
		auto node_ptr = std::make_shared<rasterizer_node>(fox_loader.get_vertices(), fox_loader.get_indices(), fox_trans_ptr);
		
		node_ptr->vertex_shader = [](vertex* vert, const vertex_uniforms& uniforms) -> vertex_data
		{
			vertex_data data;
			data.pos = uniforms.camera * uniforms.model * vert->xyzw;
			data.uv = vert->uv;
			data.color = vert->rgba;
			data.normal = uniforms.normal * vert->normal;
			data.fragment = (uniforms.model * data.pos);

			return data;
		};

		node_ptr->fragment_shader = [](const vertex_data& data, const texture_data& texture, const fragment_uniforms& uniforms) -> color
		{
			const vector4 col = texture.get_pixel(data.uv);
			return color(col.x(), col.y(), col.z(), col.w());
		};

		auto tex_ptr = std::make_shared<texture_data>("./res/textures/fox_base.png");
		node_ptr->texture(tex_ptr);
		
		rasterizer_ptr->add_node(node_ptr);
		
		auto buffer_renderer_ptr = std::make_shared<buffer_renderer>(rasterizer_ptr);
		/*END SOFTWARE RENDERER*/

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
					else if (key == GLFW_KEY_LEFT_CONTROL)
					{
						is_software_renderer_ = !is_software_renderer_;
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
					camera_trans_ptr->rotation = vector4(-mouse_y_, mouse_x_, 0, 1);
				else if (is_dragging_mouse_)
					fox_trans_ptr->rotation += vector4(mouse_y_ - mouse_down_y_, mouse_x_ - mouse_down_x_, 0, 1) * 0.5f;
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

			if (is_software_renderer_)
			{
				buffer_renderer_ptr->draw();
			}
			else
			{
				shader_ptr->use();
				shader_ptr->set_uniform("u_camera_position", camera_trans_ptr->position);
				shader_ptr->set_uniform("u_light.color", p_light.rgb);
				shader_ptr->set_uniform("u_light.intensity", p_light.intensity);
				shader_ptr->set_uniform("u_light.position", p_light.position); // should not be vec4!
				shader_ptr->set_uniform("u_ambient_color", vector3(0.025f, 0, 0.025f));
				shader_ptr->set_uniform("u_ambient_strength", 1.0f);
				shader_ptr->set_uniform("u_specular_strength", 0.5f);
				shader_ptr->set_uniform("u_shininess", 32);
				shader_ptr->drop();
				
				fox_node.draw();
			}


			this->window_->SwapBuffers();
		}
	}
}