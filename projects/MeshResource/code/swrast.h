#pragma once

#include "vertex.h"
#include "camera.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace efiilj
{
	class rasterizer
	{
	private:
		int height_, width_;
		unsigned int rbo_;
		std::vector<vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::function<void(int)> vertex_shader_, fragment_shader_;

		std::shared_ptr<camera_model> camera_;
		std::shared_ptr<transform_model> transform_;

		void generate_framebuffer();
		void put_pixel(int x, int y);

		void draw_tri(const vertex& v1, const vertex& v2, const vertex& v3) const;
		
	public:
		rasterizer(int height, int width, std::shared_ptr<camera_model> camera, std::shared_ptr<transform_model> model);

		void set_shader(std::function<void(int)> vertex, std::function<void(int)> fragment);
		void set_buffers(std::vector<vertex> vertices, std::vector<unsigned int> indices);

		int get_width() const { return width_; }
		int get_height() const { return height_; }
		unsigned int get_buffer() const { return rbo_; }

		void set_vertex_shader(std::function<void(int)> shader) { vertex_shader_ = std::move(shader); }
		void set_fragment_shader(std::function<void(int)> shader) { fragment_shader_ = std::move(shader); }

		void bind() const;
		static void unbind();
		void render();
	};
}