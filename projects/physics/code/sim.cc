#include "sim.h"

#include "imgui.h"
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
		_data.current.emplace_back();
		_data.previous.emplace_back();
		_data.inertia.emplace_back(1.0f);
		_data.inverse_inertia.emplace_back(0.1f);
		_data.mass.emplace_back(0.1f);
		_data.inverse_mass.emplace_back(1.0f / 0.1f);

		set_mass(idx, 1.0f);
		set_inertia_as_cube(idx, 1.0f);
	}

	void simulator::draw_gui()
	{}

	void simulator::draw_gui(physics_id idx)
	{
		ImGui::Text("Rigidbody %d", idx);	
		ImGui::Text("Simulation time: %f", t);

		ImGui::SliderFloat("Time step", &dt, 0.01f, 0.25f);

		if (!is_valid(idx))
			return;

		PhysicsState& state = _data.current[idx];

		if (ImGui::DragFloat("Mass", &_data.mass[idx], 0.01f))
		{
			set_mass(idx, _data.mass[idx]);
			set_inertia_as_cube(idx, 1.0f);
			recalculate_state(idx, state);
		}

		if (ImGui::DragFloat3("Momentum", &state.momentum.x, 0.01f)
		|| ImGui::DragFloat3("Velocity", &state.velocity.x, 0.01f)
		|| ImGui::DragFloat3("Angular momentum", &state.angular_momentum.x, 0.01f)
		|| ImGui::DragFloat3("Angular velocity", &state.angular_velocity.x, 0.01f))
		{
			recalculate_state(idx, state);
		}
	}
	
	void simulator::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');
	}

	void simulator::recalculate_state(physics_id idx, PhysicsState& state)
	{
		state.velocity = state.momentum *
			_data.inverse_mass[idx];

		state.angular_velocity = state.angular_momentum * 
			_data.inverse_inertia[idx];

		//state.orientation.normalize();

		const vector3& av = state.angular_velocity;

		quaternion q(av.x, av.y, av.z, 0);

		state.spin = 0.5f * q * state.orientation;
	}


	void simulator::read_transform(physics_id idx, PhysicsState& state)
	{
		entity_id eid = _entities[idx];
		transform_id trf_id = _transforms->get_component(eid);

		if (_transforms->is_valid(trf_id))
		{
			state.position = _transforms->get_position(trf_id);
			state.orientation = _transforms->get_rotation(trf_id);
		}
	}

	void simulator::write_transform(physics_id idx, PhysicsState& state)
	{
		entity_id eid = _entities[idx];
		transform_id trf_id = _transforms->get_component(eid);

		if (_transforms->is_valid(trf_id))
		{
			_transforms->set_position(trf_id, state.position);
			_transforms->set_rotation(trf_id, state.orientation);
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
				PhysicsState& current = _data.current[idx];
				_data.previous[idx] = current;
				read_transform(idx, current);
				integrate(idx, current, t, dt);
				recalculate_state(idx, current);
				write_transform(idx, current);
			}

			t += dt;
			accumulator -= dt;
		}
	}
	
	simulator::Derivative simulator::evaluate(physics_id idx, const PhysicsState& state, const Derivative& d, float t, float dt)
	{
		Derivative output;
		PhysicsState next_state;

		next_state.position = state.position + d.velocity * dt;
		next_state.momentum = state.momentum + d.force * dt;
		next_state.orientation = state.orientation + d.spin * dt;
		next_state.angular_momentum = state.angular_momentum + d.torque * dt;
		recalculate_state(idx, next_state);

		output.velocity = next_state.velocity;
		output.force = acceleration(idx, next_state, t + dt);
		output.spin = next_state.spin;
		output.torque = torque(idx, next_state, t + dt);

		return output;
	}
	
	void simulator::integrate(physics_id idx, PhysicsState& state, const float t, const float dt)
	{
		Derivative a, b, c, d;

		a = evaluate(idx, state, Derivative(), t, 0.0f);
		b = evaluate(idx, state, a, t, dt * 0.5f);
		c = evaluate(idx, state, b, t, dt * 0.5f);
		d = evaluate(idx, state, c, t, dt);

		vector3 dxdt = 1.0f / 6.0f * 
			(a.velocity + 2.0f * (b.velocity + c.velocity) + d.velocity);

		vector3 dvdt = 1.0f / 6.0f * 
			(a.force + 2.0f * (b.force + c.force) + d.force);

		quaternion drdt = 1.0f / 6.0f * 
			(a.spin + 2.0f * (b.spin + c.spin) + d.spin);

		vector3 dtdt = 1.0f / 6.0f *
			(a.torque + 2.0f * (b.torque + c.torque) + d.torque);

		// Position, momentum, orientation, angular momentum
		state.position += dxdt * dt;
		state.momentum += dvdt * dt;
		state.orientation += drdt * dt;
		state.angular_momentum += dtdt * dt;

	}

	vector3 simulator::acceleration(physics_id idx, const PhysicsState& state, float t) //NOLINT
	{
		return vector3();
		return vector3(0, -9.82, 0) * 0.001f - state.velocity * 0.1f;
	}

	vector3 simulator::torque(physics_id idx, const PhysicsState& state, float t) //NOLINT
	{
		return vector3(0, 1, 0) * 0.0001f - state.angular_velocity * 0.1f;
	}

	void simulator::end_frame()
	{
		float alpha = accumulator / dt;
		// interpolate and write to transform
	}
}
