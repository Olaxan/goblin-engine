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
		_data.com.emplace_back();
		_data.inertia.emplace_back(1.0f);
		_data.inverse_inertia.emplace_back(0.1f);
		_data.mass.emplace_back(1.0f);
		_data.inverse_mass.emplace_back(0.1f);

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

		ImGui::DragFloat("Gravity", &gravity_mult, 0.01f);
		ImGui::DragFloat("Air drag", &air_drag_mult, 0.01f);

		if (ImGui::DragFloat3("CoM", &_data.com[idx].x, 0.01f))
			set_com(idx, _data.com[idx]);

		if (ImGui::DragFloat3("Momentum", &state.momentum.x, 0.01f)
		| ImGui::DragFloat3("Velocity", &state.velocity.x, 0.01f)
		| ImGui::DragFloat3("Angular momentum", &state.angular_momentum.x, 0.01f)
		| ImGui::DragFloat3("Angular velocity", &state.angular_velocity.x, 0.01f))
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
	
	vector3 simulator::calculate_com(entity_id eid) const
	{
		vector3 com;

		// Get the transform of this entity
		transform_id trf_id = _transforms->get_component(eid);

		// This shouldn't happen on recursions, 
		// since a transform is required for parenting
		if (!_transforms->is_valid(trf_id))
			return com;

		auto range = _mesh_instances->get_components(eid);
		size_t mesh_count = std::distance(range.first, range.second);

		if (mesh_count > 0)
		{
			// Sum up all mesh centers of this entity
			for (auto it = range.first; it != range.second; it++)
			{
				mesh_id mid = _mesh_instances->get_mesh(it->second);
				com += _meshes->get_center(mid);
			}

			// Divide by number of meshes
			com /= static_cast<float>(mesh_count);
		}

		const auto& children = _transforms->get_children(trf_id);
		const size_t child_count = children.size();

		if (child_count > 0)
		{
			// Recurse, find CoM for transform children as well
			for (const auto& child : children)
			{
				entity_id child_eid = _transforms->get_entity(child);
				com += calculate_com(child_eid);	
			}

			// Divide by number of children
			com /= static_cast<float>(children.size());
		}

		return com;
	}

	void simulator::recalculate_com(physics_id idx)
	{
		entity_id eid = _entities[idx];
		set_com(idx, calculate_com(eid));
	}

	void simulator::set_com(physics_id idx, const vector3& com)
	{
		entity_id eid = _entities[idx];

		_data.com[idx] = com;

		transform_id trf_id = _transforms->get_component(eid);
		if (_transforms->is_valid(trf_id))
			_transforms->set_offset(trf_id, -_data.com[idx]);
	}

	void simulator::recalculate_state(physics_id idx, PhysicsState& state)
	{
		state.velocity = state.momentum *
			_data.inverse_mass[idx];

		state.angular_velocity = state.angular_momentum * 
			_data.inverse_inertia[idx];

		state.orientation.normalize();

		const vector3& av = state.angular_velocity;

		quaternion q(av.x, av.y, av.z, 0);

		state.spin = (0.5f * q) * state.orientation;
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
				//read_transform(idx, current);
				integrate(idx, current, t, dt);
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

		recalculate_state(idx, state);

	}

	vector3 simulator::acceleration(physics_id idx, const PhysicsState& state, float t) //NOLINT
	{
		return vector3();
		return vector3(0, -9.82f * gravity_mult, 0) * _data.mass[idx] - state.velocity * air_drag_mult;
	}

	vector3 simulator::torque(physics_id idx, const PhysicsState& state, float t) //NOLINT
	{
		return vector3(1, 0, 0) * 0.01f - state.angular_velocity * 0.1f;
	}

	void simulator::end_frame()
	{
		float alpha = accumulator / dt;
		// interpolate and write to transform
	}
}
