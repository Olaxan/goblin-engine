#pragma once

#include "matrix4.h"
#include "transform.h"

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

		light_base() : 
				color(vector3(1, 1, 1)),
				ambient_intensity(0.10f),
				diffuse_intensity(0.25f) {}
	};

	struct attenuation
	{
		float constant;
		float linear;
		float exponential;

		attenuation() : 
				constant(0),
				linear(0),
				exponential(0.3f) {}
	};

	class light_source
	{
	public:

		light_base base;
		transform_model transform;
		//vector3 position;
		//vector3 direction;
		attenuation falloff;
		light_type type;

		light_source() : 
			base(light_base()),
			transform(transform_model()),
			falloff(attenuation()),
			type(light_type::pointlight) 
			{
				update_falloff();
			}

		void update_falloff()
		{
			float max_channel = fmax(fmax(base.color.x(), base.color.y()), base.color.z());

			float ret = (-falloff.linear + 
					sqrtf(falloff.linear * 
							falloff.linear - 4 * 
							falloff.exponential * (
								falloff.exponential - 256 * 
								max_channel * base.diffuse_intensity
							)
						)
					) / (2 * falloff.exponential);

			transform.set_scale(ret);
		}
	};
}
