#include "node.h"

namespace efiilj
{
	GraphicsNode::GraphicsNode() { }

	GraphicsNode::GraphicsNode(
		std::shared_ptr<MeshResource> mesh_ptr,
		std::shared_ptr<TextureResource> texture_ptr,
		std::shared_ptr<ShaderResource> shader_ptr,
		std::shared_ptr<TransformModel> transform_ptr,
		std::shared_ptr<CameraModel> camera_ptr) 
		: mesh(mesh_ptr), texture(texture_ptr), shader(shader_ptr), transform(transform_ptr), camera(camera_ptr) 
	{ 
		std::cout << mesh->IndexCount() << " / " << transform->Model().to_mem_string();
	}

	void GraphicsNode::Bind()
	{
		mesh->Bind();
		texture->Bind();
		shader->Use();
	}

	void GraphicsNode::Unbind()
	{
		mesh->Unbind();
		texture->Unbind();
		shader->Drop();
	}

	void GraphicsNode::Draw()
	{
		Bind();
		shader->SetUniformMatrix4fv("u_mvp", camera->View() * transform->Model());
		mesh->DrawElements();
		Unbind();
	}
}

