#pragma once

#include "swrast.h"
#include "shader_res.h"
#include "tex_res.h"

namespace efiilj
{
	class buffer_renderer
	{
	private:
		std::shared_ptr<rasterizer> rasterizer_;
		std::shared_ptr<shader_resource> shader_;
		std::shared_ptr<texture_resource> texture_;
		unsigned int width_, height_, vao_{};

		std::string fs_vertex_shader_ = R"glsl(
		#version 430
		layout (location = 0) out vec2 uv;
		
		void main() 
		{
		    float x = float(((uint(gl_VertexID) + 2u) / 3u) % 2u); 
		    float y = float(((uint(gl_VertexID) + 1u) / 3u) % 2u); 

		    gl_Position = vec4(-1.0f + x * 2.0f, -1.0f + y * 2.0f, 0.0f, 1.0f);
		    uv = vec2(x, y);
		})glsl";

		std::string fs_fragment_shader_ = R"glsl(
		#version 430
		layout (location = 0) in vec2 uv;

		uniform sampler2D t;

		out vec4 Color;
		
		void main()
		{
		   Color = texture2D(t, uv);
		})glsl";

	public:
		explicit buffer_renderer(std::shared_ptr<rasterizer> rasterizer);
		
		void draw() const;
	};
}