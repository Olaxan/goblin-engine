#pragma once

#include "mesh_res.h"
#include "transform.h"
#include "material.h"
#include "camera.h"
#include "ray.h"

#include "trfm_mgr.h"

#include <memory>

namespace efiilj
{
	/**
	 * \brief Class to represent a graphics object in 3D space.
	 */
	class graphics_node
	{
	protected:

		std::shared_ptr<mesh_resource> mesh_;
		std::shared_ptr<material_base> material_;

		std::shared_ptr<transform_manager> _transforms;
		transform_id _transform;

		bool is_absolute_;

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
				std::shared_ptr<transform_manager> trf_mgr,
				std::shared_ptr<mesh_resource> mesh_ptr,
				std::shared_ptr<material_base> material_ptr,
				transform_id transform
			);

		std::string name;

		std::shared_ptr<mesh_resource> get_mesh() const { return mesh_; }
		void set_mesh(std::shared_ptr<mesh_resource> mesh) { this->mesh_ = std::move(mesh); }

		std::shared_ptr<material_base> get_material() const { return material_; }
		void set_material(std::shared_ptr<material_base> material) { this->material_ = std::move(material); }

		transform_id get_transform() const { return _transform; }
		void set_transform(transform_id idx) { _transform = idx; }
		
		void set_absolute(bool a) { is_absolute_ = a; }

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

		void draw_node_gui();
	};
}
