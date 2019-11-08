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
		int height_, width_;
		unsigned color_;
		unsigned* buffer_;
		float* depth_;

		std::vector<std::shared_ptr<rasterizer_node>> nodes_;
		std::shared_ptr<camera_model> camera_;

		std::function<bool(const vertex_data& a, const vertex_data& b)> vertex_comparator_ = [this](const vertex_data& a, const vertex_data& b)
		{
			return static_cast<int>(a.pos.x()) + width_ * static_cast<int>(a.pos.y()) < static_cast<int>(b.pos.x()) + width_ * static_cast<int>(b.pos.y());
		};
		
		void convert_screenspace(vertex_data& vertex) const;
		bool cull_backface(const vector4& p0, const vector4& face_normal, const vector4& camera_local) const;
		bool depth_test(int x, int y, float z) const;
		
		void put_pixel(int x, int y, unsigned c) const;
		void fill_line(const point_data& start, const point_data& end, const vector4& face_normal, const rasterizer_node& node, vertex_data* data) const;
		void draw_tri(rasterizer_node& node, const matrix4& local, unsigned index) const;
		void bresenham_line(line_data& line, unsigned c = 0xFFFFFFFF) const;

		static vector3 get_barycentric(float x, float y, const vector4& face_normal, vertex_data* data);
		static vector3 get_barycentric(const vector4& point, const vector4& face_normal, vertex_data* data);
		static vector3 get_barycentric(const point_data& point, const vector4& face_normal, vertex_data* data);

		static vertex_data interpolate_fragment(const vector3& barycentric, vertex_data* data);

		static vector4 get_face_normal(const vector4& a, const vector4& b, const vector4& c);
		static point_data get_point_on_line(line_data& line);
		static float get_winding_order(const vector4& a, const vector4& b, const vector4& c);
		
	public:
		rasterizer(int height, int width, std::shared_ptr<camera_model> camera, unsigned color = 0);
		~rasterizer();

		void add_node(std::shared_ptr<rasterizer_node> node) { nodes_.emplace_back(std::move(node)); }

		int get_width() const { return width_; }
		int get_height() const { return height_; }
		
		unsigned* get_frame_buffer() const { return buffer_; }
		float* get_depth_buffer() const { return depth_; }

		void clear() const;
		void render();
	};
}
