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

	void GraphicsNode::SetPosition(Vector3& pos, bool relative)
	{
		transform(3, 0) = (transform(3, 0) * relative)
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
		shader->SetUniformMatrix4fv("u_mvp", transform);
		mesh->DrawElements();
		Unbind();
	}
}

