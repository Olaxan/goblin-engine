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
	class rasterizer
	{
	private:
		unsigned height_, width_, color_;
		unsigned* buffer_;
		unsigned short* depth_;
		
		std::vector<std::shared_ptr<rasterizer_node>> nodes_;
		std::shared_ptr<camera_model> camera_;

		rasterizer_node* active_node_{};

		void convert_screenspace(vertex_data& vertex) const;
		
		void put_pixel(int x, int y, unsigned c) const;
		void fill_line(const point_data& start, const point_data& end, vertex_data* data) const;
		void draw_tri(unsigned index);

		static vector4 get_barycentric(const vector4& point, const vector4& face_normal, vertex_data* data);
		static vector4 face_normal(const vector4& a, const vector4& b, const vector4& c);
		static point_data point_on_line(line_data& line);
		bool cull_backface(const vector4& face_normal) const;
		void bresenham_line(line_data& line, unsigned c = 0xFFFFFFFF) const;
		
	public:
		rasterizer(unsigned height, unsigned width, std::shared_ptr<camera_model> camera, unsigned color = 0);
		~rasterizer();

		void add_node(std::shared_ptr<rasterizer_node> node) { nodes_.emplace_back(std::move(node)); }

		unsigned get_width() const { return width_; }
		unsigned get_height() const { return height_; }
		
		unsigned* get_frame_buffer() const { return buffer_; }
		unsigned short* get_depth_buffer() const { return depth_; }

		void clear() const;
		void render();
	};
}