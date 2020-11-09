#pragma once

#include "swrast.h"
#include "program.h"
#include "tex_res.h"

namespace efiilj
{
	/**
	 * \brief Class for drawing a uint buffer directly to the screen using internal shader code.
	 */
	class buffer_renderer
	{
	private:
		std::shared_ptr<rasterizer> rasterizer_;
		std::shared_ptr<shader_program> shader_;
		std::shared_ptr<texture_resource> texture_;
		unsigned int width_, height_, vao_{};

		/**
		 * \brief The default vertex shader - creates a quad across the screen.
		 * Invoke with a draw call for 6 vertices.
		 */
		std::string vertex_shader_ = R"glsl(
		#version 430
		layout (location = 0) out vec2 uv;
		
		void main() 
		{
		    float x = float(((uint(gl_VertexID) + 2u) / 3u) % 2u); 
		    float y = float(((uint(gl_VertexID) + 1u) / 3u) % 2u); 

		    gl_Position = vec4(-1.0f + x * 2.0f, -1.0f + y * 2.0f, 0.0f, 1.0f);
		    uv = vec2(x, y);
		})glsl";

		/**
		 * \brief The default fragment shader - samples the buffer/raster texture for color.
		 */
		std::string fragment_shader_ = R"glsl(
		#version 430
		layout (location = 0) in vec2 uv;

		uniform sampler2D t;

		out vec4 Color;
		
		void main()
		{
		   Color = texture2D(t, uv);
		})glsl";

	public:
		/**
		 * \brief Creates a new buffer renderer instance.
		 * \param rasterizer A pointer to the rasterizer containing the buffer to be drawn
		 */
		explicit buffer_renderer(std::shared_ptr<rasterizer> rasterizer);
		
		/**
		 * \brief Draw the buffer to the screen.
		 * Invokes the render() method in the connected rasterizer.
		 */
		void draw() const;
	};
}
