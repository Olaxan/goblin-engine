#pragma once

#include "mesh_res.h"
#include "tex_res.h"
#include "shader_res.h"
#include "transform.h"
#include "camera.h"

#include <memory>

namespace efiilj
{
	class GraphicsNode
	{
	private:

		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<TextureResource> texture;
		std::shared_ptr<ShaderResource> shader;
		std::shared_ptr<TransformModel> transform;
		std::shared_ptr<CameraModel> camera;

	public:

		GraphicsNode();
		GraphicsNode(
			std::shared_ptr<MeshResource> mesh_ptr,
			std::shared_ptr<TextureResource> texture_ptr,
			std::shared_ptr<ShaderResource> shader_ptr,
			std::shared_ptr<TransformModel> transform_ptr,
			std::shared_ptr<CameraModel> camera_ptr);

		const MeshResource& Mesh() const { return *this->mesh; }
		//void Mesh(MeshResource& mesh) { this->mesh = std::make_shared<MeshResource>(mesh); }

		const TextureResource& Texture() const { return *this->texture; }
		//void Texture(TextureResource& texture) { this->texture = std::make_shared<TextureResource>(texture); }

		const ShaderResource& Shader() const { return *this->shader; }
		//void Shader(ShaderResource& shader) { this->shader = std::make_shared<ShaderResource>(shader); }

		TransformModel& Transform() { return *this->transform; }
		//void Transform(TransformModel& transform) { this->transform = std::make_shared<TransformModel>(transform); }

		CameraModel& Camera() { return *this->camera; }
		//void Camera(CameraModel& camera) { this->camera = std::make_shared<CameraModel>(camera); }

		void Bind();
		void Unbind();
		void Draw();

	};
}