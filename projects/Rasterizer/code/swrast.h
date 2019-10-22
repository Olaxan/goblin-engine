#pragma once

#include "vertex.h"
#include "camera.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace efiilj
{
	union color
	{
		color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 0xFF)
			: c(0xFFFFFFFF)
		{
			rgba.r = r;
			rgba.g = g;
			rgba.b = b;
			rgba.a = a;
		}
		
		unsigned int c;
		
		struct rgba_store
		{
			unsigned char a;
			unsigned char b;
			unsigned char g;
			unsigned char r;
		} rgba;

		operator unsigned int() const { return c; }
	};
	
	class rasterizer
	{
	private:
		unsigned int height_, width_;
		unsigned int* buffer_;
		unsigned short* depth_;
		std::vector<vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::function<void(int)> vertex_shader_, fragment_shader_;

		std::shared_ptr<camera_model> camera_;
		std::shared_ptr<transform_model> transform_;

		void put_pixel(int x, int y, unsigned int c) const;
		void draw_tri(const vertex& v1, const vertex& v2, const vertex& v3) const;
		void bresenham_line(int x1, int y1, int x2, int y2) const;
		void bresenham_line(const vertex& v1, const vertex& v2) const;
		
	public:
		rasterizer(unsigned int height, unsigned int width, 
			std::shared_ptr<camera_model> camera, std::shared_ptr<transform_model> model);
		
		~rasterizer();

		void set_shader(std::function<void(int)> vertex, std::function<void(int)> fragment);
		void set_buffers(std::vector<vertex> vertices, std::vector<unsigned int> indices);

		unsigned int get_width() const { return width_; }
		unsigned int get_height() const { return height_; }
		matrix4 get_mvp() const { return camera_->view() * transform_->model(); }

		void set_vertex_shader(std::function<void(int)> shader) { vertex_shader_ = std::move(shader); }
		void set_fragment_shader(std::function<void(int)> shader) { fragment_shader_ = std::move(shader); }
		
		void render();
	};
}