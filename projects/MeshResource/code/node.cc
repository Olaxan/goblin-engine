#include "node.h"

namespace efiilj
{
	GraphicsNode::GraphicsNode()
	{
	}

	GraphicsNode::GraphicsNode(MeshResource& mesh, TextureResource& texture, ShaderResource& shader)
	{
		this->mesh = std::make_unique<MeshResource>(mesh);
		this->texture = std::make_unique<TextureResource>(texture);
		this->shader = std::make_unique<ShaderResource>(shader);
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

