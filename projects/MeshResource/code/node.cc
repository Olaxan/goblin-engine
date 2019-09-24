#include "node.h"

namespace efiilj
{
	GraphicsNode::GraphicsNode()
	{
	}

	GraphicsNode::GraphicsNode(MeshResource& mesh, TextureResource& texture, ShaderResource& shader)
	{
		this->mesh = std::unique_ptr<MeshResource>(&mesh);
		this->texture = std::unique_ptr<TextureResource>(&texture);
		this->shader = std::unique_ptr<ShaderResource>(&shader);
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
		mesh->DrawElements();
		Unbind();
	}
}

