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

		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<TextureResource> texture;
		std::shared_ptr<ShaderResource> shader;

	public:

		GraphicsNode();
		GraphicsNode(MeshResource& mesh, TextureResource& texture, ShaderResource& shader);

		const MeshResource& Mesh() const { return *this->mesh; }
		void Mesh(MeshResource& mesh) { this->mesh = std::make_shared<MeshResource>(mesh); }

		const TextureResource& Texture() const { return *this->texture; }
		void Texture(TextureResource& texture) { this->texture = std::make_shared<TextureResource>(texture); }

		const ShaderResource& Shader() const { return *this->shader; }
		void Shader(ShaderResource& shader) { this->shader = std::make_shared<ShaderResource>(shader); }

		const Matrix4& Transform() const { return this->transform; }
		void Transform(Matrix4& transform) { this->transform = transform; }

		void SetPosition(Vector3& pos, bool relative = false);

		void Bind();
		void Unbind();
		void Draw();
	};
}