#pragma once

#include "vertex.h"
#include "camera.h"
#include "rnode.h"
#include "line.h"

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
		
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} rgba;

		operator unsigned int() const { return c; }
	};
	
	class rasterizer
	{
	private:
		unsigned height_, width_, color_;
		unsigned* buffer_;
		unsigned short* depth_;
		
		std::vector<std::shared_ptr<rasterizer_node>> nodes_;
		std::shared_ptr<camera_model> camera_;

		void normalize(vector4& vec, transform_model& transform);
		
		void put_pixel(int x, int y, unsigned int c);
		void fill_line(const point_data& start, const point_data& end);
		void draw_tri(rasterizer_node& node, unsigned int index);

		static point_data point_on_line(line_data& line);
		void bresenham_line(line_data& line, unsigned c = 0xFFFFFFFF);
		
	public:
		rasterizer(unsigned int height, unsigned int width, std::shared_ptr<camera_model> camera, unsigned int color = 0);
		~rasterizer();

		void add_node(std::shared_ptr<rasterizer_node> node) { nodes_.emplace_back(std::move(node)); }

		unsigned int get_width() const { return width_; }
		unsigned int get_height() const { return height_; }
		
		unsigned int* get_frame_buffer() const { return buffer_; }
		unsigned short* get_depth_buffer() const { return depth_; }

		void clear();
		void render();
	};
}