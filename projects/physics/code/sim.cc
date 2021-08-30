#include "sim.h"

#include "vector3.h"

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

		_constants.inertia.emplace_back(1.0f);
		_constants.inverse_inertia.emplace_back(0.1f);
		_constants.mass.emplace_back(1.0f);
		_constants.inverse_mass.emplace_back(0.1f);
	}

	void simulator::draw_gui()
	{}

	void simulator::draw_gui(physics_id idx)
	{
		ImGui::Text("Rigidbody %d", idx);	
	}
	
	void simulator::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
	}

	void simulator::extend_state(PhysicsState& state, physics_id)
	{
		state.velocity.emplace_back();
		state.momentum.emplace_back();
		state.position.emplace_back();
		state.orientation.emplace_back();
		state.angular_momentum.emplace_back();
		state.spin.emplace_back();
		state.angular_velocity.emplace_back();
	}

	void simulator::recalculate_state(PhysicsState& state, physics_id idx)
	{
		state.velocity[idx] = state.momentum[idx] *
			_constants.inverse_mass[idx];

		state.angular_velocity[idx] = state.angular_momentum[idx] * 
			_constants.inverse_inertia[idx];

		state.orientation[idx].normalize();

		const vector3& av = state.angular_velocity[idx];

		quaternion q(av.x, av.y, av.z, 0);

		state.spin[idx] = (q * state.orientation[idx]) * 0.5f;
	}

	void simulator::swap_state(physics_id idx)
	{
		_previous.position[idx] = _current.position[idx];
		_previous.velocity[idx] = _current.velocity[idx];
		_previous.momentum[idx] = _current.momentum[idx];
		_previous.orientation[idx] = _current.orientation[idx];
		_previous.angular_momentum[idx] = _current.angular_momentum[idx];
		_previous.angular_velocity[idx] = _current.angular_velocity[idx];
		_previous.spin[idx] = _current.spin[idx];
	}

	void simulator::read_transform(physics_id idx)
	{
		entity_id eid = _entities[idx];
		transform_id trf_id = _transforms->get_component(eid);

		if (_transforms->is_valid(trf_id))
		{
			_current.position[idx] = _transforms->get_position(trf_id);
			_current.orientation[idx] = _transforms->get_position(trf_id);
		}
	}

	void simulator::write_transform(physics_id idx)
	{
		entity_id eid = _entities[idx];
		transform_id trf_id = _transforms->get_component(eid);

		if (_transforms->is_valid(trf_id))
		{
			_transforms->set_position(trf_id, _current.position[idx]);
			_transforms->set_rotation(trf_id, _current.orientation[idx]);
		}
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
				read_transform(idx);
				swap_state(idx);
				integrate(idx, t, dt);
				write_transform(idx);
			}

			t += dt;
			accumulator -= dt;
		}
	}
	
	simulator::Derivative simulator::evaluate(physics_id idx, const Derivative& d, float t, float dt)
	{
		Derivative output;

		vector3 pos = _current.position[idx] + d.velocity * dt;
		vector3 vel = _current.velocity[idx] + d.force * dt;
		//quaternion spin = _current.orientation[idx] + d.spin * dt;

		output.velocity = vel;
		output.force = acceleration(idx, t + dt);

		return output;
	}
	
	void simulator::integrate(physics_id idx, const float t, const float dt)
	{
		Derivative a, b, c, d;

		a = evaluate(idx, Derivative(), t, 0.0f);
		b = evaluate(idx, a, t, dt * 0.5f);
		c = evaluate(idx, b, t, dt * 0.5f);
		d = evaluate(idx, c, t, dt);

		vector3 dxdt = 1.0f / 6.0f * 
			(a.velocity + 2.0f * (b.velocity + c.velocity) + d.velocity);

		vector3 dvdt = 1.0f / 6.0f * 
			(a.force + 2.0f * (b.force + c.force) + d.force);

		quaternion dsdt = 1.0f / 6.0f * 
			(a.spin + 2.0f * (b.spin + c.spin)  + d.spin);

		vector3 dtdt = 1.0f / 6.0f *
			(a.torque + 2.0f * (b.torque + c.torque) + d.torque);

		_current.position[idx] += dxdt * dt;
		_current.velocity[idx] += dvdt * dt;
		_current.orientation[idx] += dsdt * dt;
		_current.angular_velocity[idx] += dtdt * dt;

	}

	vector3 simulator::acceleration(physics_id idx, float t)
	{
		return vector3(0, -1, 0) * 9.82f * 0.0001f;
	}

	vector3 simulator::torque(physics_id idx, float t)
	{
		return vector3();
	}

	void simulator::end_frame()
	{
		float alpha = accumulator / dt;
		// interpolate and write to transform
	}
}
