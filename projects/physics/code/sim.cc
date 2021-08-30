#include "sim.h"

namespace efiilj
{

	simulator::simulator()
	{
		printf("Init simulator...\n");
		_name = "Simulator";
	}

	simulator::~simulator()
	{}

	void simulator::extend_defaults(physics_id idx)
	{
		extend_state(_current, idx);
		extend_state(_previous, idx);
	}

	void simulator::draw_gui()
	{}

	void simulator::draw_gui(physics_id)
	{}
	
	void simulator::on_register(std::shared_ptr<manager_host>)
	{}

	void simulator::extend_state(PhysicsState& state, physics_id)
	{
		state.position.emplace_back();
		state.orientation.emplace_back();
		state.angular_momentum.emplace_back();
		state.spin.emplace_back();
		state.angular_velocity.emplace_back();
		state.inertia.emplace_back();
		state.inverse_inertia.emplace_back();
	}

	void simulator::recalculate_state(PhysicsState& state, physics_id idx)
	{
		state.angular_velocity[idx] = state.angular_momentum[idx] * 
			state.inverse_inertia[idx];

		state.orientation[idx].normalize();

		const vector3& av = state.angular_velocity[idx];

		quaternion q(av.x, av.y, av.z, 0);

		state.spin[idx] = (q * state.orientation[idx]) * 0.5f;
	}

	void simulator::swap_state(physics_id idx)
	{
		_previous.position[idx] = _current.position[idx];
	}

	void simulator::begin_frame()
	{
		frame_time new_time = frame_timer::now();
		duration delta_time = new_time - current_time;

		float dt_seconds = delta_time.count();

		if (dt_seconds > 0.25f)
			dt_seconds = 0.25f;

		accumulator += dt_seconds;
	}

	void simulator::simulate()
	{
		while (accumulator >= dt)
		{
			for (const auto& idx : _instances)
			{
				swap_state(idx);
				rk4(idx, t, dt);
				t += dt;
				accumulator -= dt;
			}
		}
	}
	
	void simulator::rk4(physics_id idx, const float t, const float dt)
	{

	}

	void simulator::end_frame()
	{
		float alpha = accumulator / dt;
		// interpolate and write to transform
	}
}
