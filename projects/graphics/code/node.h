#pragma once

#include "mesh_res.h"
#include "transform.h"
#include "material.h"
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
		std::shared_ptr<material_base> material_;
		std::shared_ptr<transform_model> transform_;

	public:

		graphics_node();
		/**
		 * \brief Creates a new Graphics Node instance.
		 * \param mesh_ptr Shared pointer to Mesh Resource of node
		 * \param shader_ptr Shared pointer to Shader Resource of node
		 * \param transform_ptr Shared pointer to Transform Model of node
		 * \param camera_ptr Shared pointer to Camera Model of node
		 */

		graphics_node(
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<material_base> material_ptr);

		graphics_node(
			std::shared_ptr<mesh_resource> mesh_ptr,
			std::shared_ptr<material_base> material_ptr,
			std::shared_ptr<transform_model> transform_ptr);

		const mesh_resource& mesh() const { return *this->mesh_; }
		void mesh(std::shared_ptr<mesh_resource> mesh) { this->mesh_ = std::move(mesh); }

		const material_base& material() const { return *this->material_; }
		void material(std::shared_ptr<material_base> material) { this->material_ = std::move(material); }

		transform_model& transform() const { return *this->transform_; }
		void transform(std::shared_ptr<transform_model> transform) { this->transform_ = std::move(transform); }
		
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
		virtual void draw() const;
	};
}
