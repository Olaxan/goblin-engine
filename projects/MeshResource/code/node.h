#pragma once

#include "mesh_res.h"
#include "tex_res.h"
#include "shader_res.h"
#include "transform.h"
#include "camera.h"

#include <memory>

namespace efiilj
{
	class graphics_node
	{
	private:

		std::shared_ptr<mesh_resource> mesh_;
		std::shared_ptr<texture_resource> texture_;
		std::shared_ptr<shader_resource> shader_;
		std::shared_ptr<transform_model> transform_;
		std::shared_ptr<camera_model> camera_;

	public:

		graphics_node();
		graphics_node(
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<texture_resource> texture_ptr,
			std::shared_ptr<shader_resource> shader_ptr,
			std::shared_ptr<transform_model> transform_ptr,
			std::shared_ptr<camera_model> camera_ptr);

		const mesh_resource& mesh() const { return *this->mesh_; }
		//void Mesh(MeshResource& mesh) { this->mesh = std::make_shared<MeshResource>(mesh); }

		const texture_resource& texture() const { return *this->texture_; }
		//void Texture(TextureResource& texture) { this->texture = std::make_shared<TextureResource>(texture); }

		const shader_resource& shader() const { return *this->shader_; }
		//void Shader(ShaderResource& shader) { this->shader = std::make_shared<ShaderResource>(shader); }

		transform_model& transform() const { return *this->transform_; }
		//void Transform(TransformModel& transform) { this->transform = std::make_shared<TransformModel>(transform); }

		camera_model& camera() const { return *this->camera_; }
		//void Camera(CameraModel& camera) { this->camera = std::make_shared<CameraModel>(camera); }

		void bind() const;
		void unbind() const;
		void draw() const;
	};
}
