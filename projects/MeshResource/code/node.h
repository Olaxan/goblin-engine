#pragma once

#include "mesh_res.h"
#include "tex_res.h"
#include "shader_res.h"
#include "transform.h"
#include "camera.h"

#include <memory>

namespace efiilj
{
	/**
	 * \brief Class to represent a graphics object in 3D space.
	 */
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
		/**
		 * \brief Creates a new Graphics Node instance.
		 * \param mesh_ptr Shared pointer to Mesh Resource of node
		 * \param texture_ptr Shared pointer to Texture Resource of node
		 * \param shader_ptr Shared pointer to Shader Resource of node
		 * \param transform_ptr Shared pointer to Transform Model of node
		 * \param camera_ptr Shared pointer to Camera Model of node
		 */
		graphics_node(
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<texture_resource> texture_ptr,
			std::shared_ptr<shader_resource> shader_ptr,
			std::shared_ptr<transform_model> transform_ptr,
			std::shared_ptr<camera_model> camera_ptr);

		const mesh_resource& mesh() const { return *this->mesh_; }
		void mesh(std::shared_ptr<mesh_resource>& mesh) { this->mesh_ = std::move(mesh); }

		const texture_resource& texture() const { return *this->texture_; }
		void texture(std::shared_ptr<texture_resource>& texture) { this->texture_ = std::move(texture); }

		const shader_resource& shader() const { return *this->shader_; }
		void shader(std::shared_ptr<shader_resource>& shader) { this->shader_ = std::move(shader); }

		transform_model& transform() const { return *this->transform_; }
		void transform(std::shared_ptr<transform_model>& transform) { this->transform_ = std::move(transform); }

		camera_model& camera() const { return *this->camera_; }
		void camera(std::shared_ptr<camera_model>& camera) { this->camera_ = std::move(camera); }

		/**
		 * \brief Prepares the node for drawing, binding mesh, texture, and shader resources.
		 */
		void bind() const;
		/**
		 * \brief Releases the node from binding on the GPU
		 */
		void unbind() const;
		/**
		 * \brief Performs a draw call. View/perspective + model matrices are pushed shader uniforms "u_camera" and "u_model" respectively.
		 */
		void draw() const;
	};
}
