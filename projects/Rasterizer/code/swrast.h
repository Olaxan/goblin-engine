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
		float x_offset_, y_offset_;

		std::vector<std::shared_ptr<rasterizer_node>> nodes_;
		std::shared_ptr<camera_model> camera_;

		std::function<bool(const vertex_data& a, const vertex_data& b)> vertex_comparator_ = [this](const vertex_data& a, const vertex_data& b)
		{
			return static_cast<int>(a.pos.x()) + width_ * static_cast<int>(a.pos.y()) < static_cast<int>(b.pos.x()) + width_ * static_cast<int>(b.pos.y());
		};

		/**
		 * \brief Convert clip space (-1, 1) coordinates to raster coordinates.
		 * \param vertex The vertex which should be transformed
		 */
		void convert_screenspace(vertex_data& vertex) const;

		/**
		 * \brief Checks whether or not the face can be culled without consequence.
		 * \param p0 A point on the triangle
		 * \param face_normal The face normal of the triangle
		 * \param camera_local The camera transform in object space
		 * \return True if the specified face points away from the camera, false otherwise
		 */
		bool cull_backface(const vector4& p0, const vector4& face_normal, const vector4& camera_local) const;

		/**
		 * \brief Tests a pixel against the Z-buffer, and stores the new value if it's closer to the camera.
		 * \param x Placement on the X-axis
		 * \param y Placement on the Y-axis
		 * \param z Placement on the Z-axis
		 * \return True if the pixel passes depth testing and should be drawn, false otherwise
		 */
		bool depth_test(int x, int y, float z) const;

		/**
		 * \brief Places a pixel on the specified location, with the specified color
		 * \param x Placement on the X-axis
		 * \param y Placement on the Y-axis
		 * \param c Color in RGBA format (1 byte per channel)
		 */
		void put_pixel(int x, int y, unsigned c);
		
		/**
		 * \brief Fills a single scanline in the raster between two points, running the fragment shader for each.
		 * \param start Start point of line
		 * \param end End point of line
		 * \param face_normal The face normal which should be used to calculate the barycentric coordinates
		 * \param node The graphics node currently being rendered
		 * \param data The current vertex data (3 vertices in raster space)
		 */
		void fill_scanline(const point_data& start, const point_data& end, const vector4& face_normal, const rasterizer_node& node, vertex_data* data);

		/**
		 * \brief Draws an isolated face of the specified node, using three vertices starting with the specified index
		 * \param node The graphics node which is currently being rendered
		 * \param local The node world-to-local matrix used for backface culling
		 * \param index The first index of the face (out of 3)
		 */
		void draw_tri(rasterizer_node& node, const matrix4& local, unsigned index);

		/**
		 * \brief Debug function for drawing a line of the specified color directly on the raster.
		 * \param line The line which should be drawn
		 * \param c The color of the line
		 */
		void draw_line(line_data& line, unsigned c = 0xFFFFFFFF);

		/**
		 * \brief Calculates the barycentric weights of a single point inside (or outside) a face.
		 * \param x Placement on the X-axis
		 * \param y Placement on the Y-axis
		 * \param face_normal The face normal of the triangle
		 * \param data The vertex data in transformed raster space
		 * \return A vector3 with the vertex corresponding barycentric weights
		 */
		static vector3 get_barycentric(float x, float y, const vector4& face_normal, vertex_data* data);

		/**
		 * \brief Calculates the barycentric weights of a single point inside (or outside) a face.
		 * \param point Placement on the X, Y axes
		 * \param face_normal The face normal of the triangle
		 * \param data The vertex data in transformed raster space
		 * \return A vector3 with the vertex corresponding barycentric weights
		 */
		static vector3 get_barycentric(const vector4& point, const vector4& face_normal, vertex_data* data);

		/**
		 * \brief Calculates the barycentric weights of a single point inside (or outside) a face.
		 * \param point Placement on the X, Y axes
		 * \param face_normal The face normal of the triangle
		 * \param data The vertex data in transformed raster space
		 * \return A vector3 with the vertex corresponding barycentric weights
		 */
		static vector3 get_barycentric(const point_data& point, const vector4& face_normal, vertex_data* data);

		/**
		 * \brief Creates an interpolated vertex_data object using barycentric weights and face vertex data.
		 * \param barycentric The barycentric weight vector of the point-on-face
		 * \param data The vertex data of the triangle
		 * \return A vertex data object representing a single, interpolated fragment on a face
		 */
		static vertex_data interpolate_fragment(const vector3& barycentric, vertex_data* data);

		/**
		 * \brief Returns the face normal of a face represented by three vectors.
		 * \param a The first face vertex position
		 * \param b The second face vertex position
		 * \param c The third face vertex position
		 * \return The face normal of the face, calculated by cross product
		 */
		static vector4 get_face_normal(const vector4& a, const vector4& b, const vector4& c);

		/**
		 * \brief Returns the winding order of a face represented by three vectors.
		 * \param a The first face vertex position
		 * \param b The second face vertex position
		 * \param c The third face vertex position
		 * \return A float representing winding order, with negative values indicating CCW (?)
		 */
		static float get_winding_order(const vector4& a, const vector4& b, const vector4& c);

		/**
		 * \brief Moves a line data object one step in its major axis and returns the resulting point.
		 * \param line The line currently being traversed
		 * \return The next point along the line's major axis.
		 */
		static point_data get_point_on_line(line_data& line);

		
	public:
		
		/**
		 * \brief Creates a new rasterizer instance.
		 * \param height The height of the rasterizer canvas in pixels
		 * \param width The width of the rasterizer canvas in pixels
		 * \param camera A pointer to an active camera instance
		 * \param color The background color of the canvas
		 */
		rasterizer(int height, int width, std::shared_ptr<camera_model> camera, unsigned color = 0);
		~rasterizer();

		void add_node(std::shared_ptr<rasterizer_node> node) { nodes_.emplace_back(std::move(node)); }

		int get_width() const { return width_; }
		int get_height() const { return height_; }
		
		unsigned* get_frame_buffer() const { return buffer_; }
		float* get_depth_buffer() const { return depth_; }

		/**
		 * \brief Clears the raster and depth buffer using the background color.
		 */
		void clear();

		/**
		 * \brief Renders all nodes in render queue to the raster.
		 */
		void render();
	};
}
