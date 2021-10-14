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
		_data.impulse.emplace_back();
		_data.inertia.emplace_back(1.0f);
		_data.inverse_inertia.emplace_back(0.1f);
		_data.mass.emplace_back(1.0f);
		_data.inverse_mass.emplace_back(0.1f);
		_data.restitution.emplace_back(0.9f);

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
		_colliders = host->get_manager_from_fcc<collider_manager>('RAYS');
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

		for (const auto& idx : _instances)
			read_transform(idx, _data.current[idx]);

	}

#define MAX_SEEK_ITERATIONS 32

	void simulator::simulate()
	{

		// TODO: Variable max pen depth
		const float max_pen_depth = 0.01;
		const float max_sqr_pen_depth = max_pen_depth * max_pen_depth;

		while (accumulator >= dt)
		{
			for (const auto& idx : _instances)
			{
				_data.previous[idx] = _data.current[idx];
				integrate(idx, _data.current[idx], t, dt);	
				write_transform(idx, _data.current[idx]);
			}

			_colliders->update();

			for (const auto& col_a : _colliders->get_instances())
			{

				float h = dt * 0.5f;

				physics_id phys_a = get_component(_colliders->get_entity(col_a));
				PhysicsState& state_a = _data.current[phys_a];

				if (!is_valid(phys_a))
					continue;

				for (const auto& col_b: _colliders->get_collisions(col_a))
				{
					vector3 epa;

					physics_id phys_b = get_component(_colliders->get_entity(col_b));

					PhysicsState& state_b = _data.current[phys_b];

					while (true)
					{
						// We'll always step from the last physics state
						state_a = _data.previous[phys_a];

						integrate(phys_a, state_a, t, h);
						write_transform(phys_a, state_a);

						if (_colliders->test_collision(col_a, col_b, epa))
						{
							vector3 p = state_a.position + epa;
							vector3 n = epa.norm();

							vector3 ra = p - _data.com[phys_a];
							vector3 rb = p - _data.com[phys_b];

							vector3 vpa = vector3::cross(state_a.velocity + state_a.angular_velocity, ra);
							vector3 vpb = vector3::cross(state_b.velocity + state_b.angular_velocity, rb);

							float vrel = vector3::dot(n, (vpa - vpb));

							if (vrel > 0)
								break;

							if (epa.square_magnitude() < max_sqr_pen_depth)
							{


								float rest = std::min(_data.restitution[phys_a], _data.restitution[phys_b]);
								float num = -(1.0f + rest) * vrel;

								float denom = _data.inverse_mass[phys_a] + _data.inverse_mass[phys_b] 
									+ vector3::dot(n, vector3::cross(_data.inverse_inertia[phys_a] * vector3::cross(ra, n), ra))
									+ vector3::dot(n, vector3::cross(_data.inverse_inertia[phys_b] * vector3::cross(rb, n), rb));

								float j = num / denom;

								PointForce response_a(p, j * n);
								PointForce response_b(p, -(j * n));

								add_impulse(phys_a, response_a);
								add_impulse(phys_b, response_b);

								integrate(phys_a, state_a, t + h, dt - h);
								integrate(phys_b, state_b, t + h, dt - h);

								break;
							}

							h *= 0.5f;
						}
						else
						{
							h *= 1.5;
						}
					}
				}
			}

			t += dt;
			accumulator -= dt;
		}
	}

	void simulator::end_frame()
	{
		//float alpha = accumulator / dt;

		for (const auto& idx : _instances)
		{
			_data.impulse[idx] = PointForce();
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
	
	void simulator::add_impulse(physics_id idx, const PointForce& force)
	{
		_data.impulse[idx] = force;	
	}

	vector3 simulator::acceleration(physics_id idx, const PhysicsState& state, float t)
	{
		const PointForce& impulse = _data.impulse[idx];
		return impulse.force;
	}

	vector3 simulator::torque(physics_id idx, const PhysicsState& state, float t)
	{
		const PointForce& impulse = _data.impulse[idx];
		return vector3::cross(impulse.p - (_data.com[idx] + state.position), impulse.force);
	}
}
