#pragma once

#include "matrix4.h"
#include "transform.h"
#include "program.h"

#include <memory>

namespace efiilj
{

	enum class light_type
	{
		directional = 0,
		pointlight = 1,
		spotlight = 2
	};

	struct light_base
	{
		vector3 color;
		float ambient_intensity;
		float diffuse_intensity;
	};

	struct attenuation
	{
		float constant;
		float linear;
		float exponential;
	};

	struct cutoff
	{
		float inner_angle;
		float outer_angle;
	};

	class light_source
	{
		private:

			std::shared_ptr<transform_model> transform_;

			light_type type_;
			light_base base_;
			attenuation falloff_;
			cutoff cutoff_;

			void update_falloff();

		public:

			light_source(std::shared_ptr<transform_model> transform, light_type type = light_type::pointlight)
			: 
				base_({vector3(1, 1, 1), 0.1f, 0.2f}),
				transform_(std::move(transform)),
				falloff_({0, 0, 0.3f}),
				cutoff_({0, 0}),
				type_(type) 
			{
				update_falloff();
			}

			light_source(light_type type = light_type::pointlight)
				: light_source(std::make_shared<transform_model>(), type) 
			{}

			light_type get_type() const { return type_; }

			void set_base(const light_base& base) { base_ = base; update_falloff(); }
			void set_base(const vector3& color, float ambient, float diffuse) { set_base( {color, ambient, diffuse} ); }

			light_base get_base() const { return base_; }

			void set_falloff(const attenuation& falloff) { falloff_ = falloff; update_falloff(); }
			void set_falloff(float constant, float linear, float exponential) { set_falloff( {constant, linear, exponential} ); }

			attenuation get_falloff() const { return falloff_; }

			void set_cutoff(const cutoff& cutoff) { cutoff_ = cutoff; update_falloff(); }
			void set_cutoff(float inner, float outer) { set_cutoff( { inner, outer }); }

			cutoff get_cutoff() const { return cutoff_; }

			void set_uniforms(std::shared_ptr<shader_program> program) const;

			std::shared_ptr<transform_model> get_transform() const { return transform_; }

			void draw_light_gui();
	};
}
