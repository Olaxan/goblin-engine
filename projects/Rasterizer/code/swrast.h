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
		
		void convert_screenspace(vertex_data& vertex) const;
		bool cull_backface(const vector4& p0, const vector4& face_normal, const vector4& camera_local) const;
		bool depth_test(unsigned x, unsigned y, unsigned short z) const;
		
		void put_pixel(int x, int y, unsigned c) const;
		void fill_line(const point_data& start, const point_data& end, const vector4& face_normal, const rasterizer_node& node, vertex_data* data) const;
		void draw_tri(rasterizer_node& node, const matrix4& local, unsigned index);
		void bresenham_line(line_data& line, unsigned c = 0xFFFFFFFF) const;

		static vector3 get_barycentric(float x, float y, const vector4& face_normal, vertex_data* data);
		static vector3 get_barycentric(const vector4& point, const vector4& face_normal, vertex_data* data);
		static vector3 get_barycentric(const point_data& point, const vector4& face_normal, vertex_data* data);

		static vertex_data interpolate_fragment(const vector3& barycentric, vertex_data* data);

		static vector4 get_face_normal(const vector4& a, const vector4& b, const vector4& c);
		static point_data get_point_on_line(line_data& line);
		static float get_winding_order(const vector4& a, const vector4& b, const vector4& c);
		
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
