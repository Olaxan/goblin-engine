#pragma once

#include "mesh_res.h"
#include "tex_res.h"
#include "shader_res.h"

namespace efiilj
{
	class GraphicsNode
	{
	private:

		Matrix4 transform;

		std::unique_ptr<MeshResource> mesh;
		std::unique_ptr<TextureResource> texture;
		std::unique_ptr<ShaderResource> shader;

	public:

		GraphicsNode();
		GraphicsNode(MeshResource& mesh, TextureResource& texture, ShaderResource& shader);

		const MeshResource& Mesh() const { return *this->mesh; }
		void Mesh(std::unique_ptr<MeshResource> mesh) { this->mesh = std::move(mesh); }

		const TextureResource& Texture() const { return *this->texture; }
		void Texture(std::unique_ptr<TextureResource> mesh) { this->texture = std::move(texture); }

		const ShaderResource& Shader() const { return *this->shader; }
		void Shader(std::unique_ptr<ShaderResource> shader) { this->shader = std::move(shader); }

		void Bind();
		void Unbind();
		void Draw();
	};
}