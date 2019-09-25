#include "node.h"

namespace efiilj
{
	GraphicsNode::GraphicsNode()
	{
	}

	GraphicsNode::GraphicsNode(MeshResource& mesh, TextureResource& texture, ShaderResource& shader)
	{
		this->mesh = std::make_shared<MeshResource>(mesh);
		this->texture = std::make_shared<TextureResource>(texture);
		this->shader = std::make_shared<ShaderResource>(shader);
	}

	void GraphicsNode::SetPosition(Vector3& pos, bool relative)
	{
		transform(3, 0) = relative ? transform(3, 0) + pos[0] : pos[0];
		transform(3, 1) = relative ? transform(3, 1) + pos[1] : pos[1];
		transform(3, 2) = relative ? transform(3, 2) + pos[2] : pos[2];
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

