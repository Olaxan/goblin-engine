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
		vector3 color {};
		float ambient_intensity {};
		float diffuse_intensity {};
	};

	struct attenuation
	{
		float constant {};
		float linear {};
		float exponential {};
	};

	struct cutoff
	{
		float inner_angle {};
		float outer_angle{};
	};

	class light_source
	{
		private:

			std::shared_ptr<transform_model> transform_;

			light_type type_;
			light_base base_;
			attenuation falloff_;
			cutoff cutoff_;

			void update_falloff()
			{
				float max_channel = fmax(fmax(base_.color.x, base_.color.y), base_.color.z);

				float ret = (-falloff_.linear + 
						sqrtf(falloff_.linear * falloff_.linear - 
							4 * falloff_.exponential * (
									falloff_.constant - 256 * 
									max_channel * base_.diffuse_intensity
								)
							)
						) / (2 * falloff_.exponential);

				transform_->set_scale(ret * 2.0f);
			}

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

			std::shared_ptr<transform_model> get_transform() const { return transform_; }

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
			
			void set_uniforms(std::shared_ptr<shader_program> program) const
			{
				program->set_uniform("source.type", static_cast<int>(type_));
				program->set_uniform("source.base.color", base_.color);	
				program->set_uniform("source.base.ambient_intensity", base_.ambient_intensity);
				program->set_uniform("source.base.diffuse_intensity", base_.diffuse_intensity);
				program->set_uniform("source.position", transform_->get_position());
				program->set_uniform("source.direction", transform_->forward());
				program->set_uniform("source.falloff.constant", falloff_.constant);
				program->set_uniform("source.falloff.linear", falloff_.linear);
				program->set_uniform("source.falloff.exponential", falloff_.exponential);
				program->set_uniform("source.cutoff.inner", cutoff_.inner_angle);
				program->set_uniform("source.cutoff.outer", cutoff_.outer_angle);
			}
	};
}
