#include "sim.h"

#include "imgui.h"
#include "vector3.h"
#include <cassert>

#include <algorithm>

namespace efiilj
{

	simulator::simulator()
	{
		printf("Init simulator...\n");
		_name = "Simulator";
	}

	simulator::~simulator()
	{}

	void simulator::on_editor_gui()
	{}

	void simulator::on_editor_gui(physics_id idx)
	{

		ImVec4 yellow(1.0f, 1.0f, 0, 1.0f);

		ImGui::TextColored(yellow, "Rigidbody %d", idx);	
		ImGui::Text("Simulation time: %f", t);

		ImGui::SliderFloat("Time step", &dt, 0.01f, 0.25f);

		if (!is_valid(idx))
			return;

		PhysicsState& state = _data.current[idx];

		ImGui::TextColored(yellow, "Global settings");
		ImGui::DragFloat("Gravity", &gravity_mult, 0.01f);
		ImGui::DragFloat("Air drag", &air_drag_mult, 0.01f);
		ImGui::InputFloat("Response force mult", &response_force_mult);
		ImGui::Checkbox("Collision penalty", &collision_penalty);
		ImGui::InputFloat("Penalty force mult", &penalty_force_mult);
		ImGui::Checkbox("Collision rejection", &collision_rejection);
		ImGui::InputFloat("Rejection depth", &reject_pen_depth);
		ImGui::Checkbox("Friction", &friction);

		ImGui::TextColored(yellow, "Object settings");

		if (ImGui::DragFloat("Mass", &_data.mass[idx], 0.01f))
		{
			set_mass(idx, _data.mass[idx]);
			set_inertia_as_cube(idx, 1.0f);
			recalculate_state(idx, state);
		}

		if (ImGui::DragFloat3("CoM", &_data.com[idx].x, 0.01f))
			set_com(idx, _data.com[idx]);

		ImGui::DragFloat("Restitution", &_data.restitution[idx], 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Friction", &_data.friction[idx], 0.01f, 0.0f, 1.0f);

		ImGui::TextColored(yellow, "Primary");

		if (ImGui::DragFloat3("Momentum", &state.momentum.x, 0.01f)
		| ImGui::DragFloat3("Angular momentum", &state.angular_momentum.x, 0.01f))
		{
			recalculate_state(idx, state);
		}

		ImGui::TextColored(yellow, "Secondary");

		ImGui::Text("Velocity");
		ImGui::Text("%f, %f, %f", 
				state.velocity.x,
				state.velocity.y,
				state.velocity.z);
		ImGui::Text("Angular velocity");
		ImGui::Text("%f, %f, %f", 
				state.angular_velocity.x,
				state.angular_velocity.y,
				state.angular_velocity.z);
	}
	
	void simulator::on_register(std::shared_ptr<manager_host> host)
	{
		_transforms = host->get_manager_from_fcc<transform_manager>('TRFM');
		_colliders = host->get_manager_from_fcc<collider_manager>('RAYS');
		_meshes = host->get_manager_from_fcc<mesh_server>('MESR');
		_mesh_instances = host->get_manager_from_fcc<mesh_manager>('MEMR');

		add_data({
			&_data.current,
			&_data.previous,
			&_data.com,
			&_data.impulses,
			&_data.inertia,
			&_data.inverse_inertia,
			&_data.mass,
			&_data.inverse_mass,
			&_data.restitution,
			&_data.friction});

		_data.mass.set_default(1.0f);
		_data.inverse_mass.set_default(0.1f);
		_data.inertia.set_default(1.0f);
		_data.inverse_inertia.set_default(0.1f);
		_data.friction.set_default(0.5f);
		_data.restitution.set_default(0.5f);
	}

	void simulator::on_activate(physics_id idx) 
	{
		set_mass(idx, 1.0f);
		set_inertia_as_cube(idx, 1.0f);
		recalculate_com(idx);
	}

	void simulator::on_begin_frame()
	{
		frame_time new_time = frame_timer::now();
		duration delta_time = new_time - current_time;

		float dt_seconds = delta_time.count();

		if (dt_seconds > 0.25f)
			dt_seconds = 0.25f;

		accumulator += dt_seconds;

		for (const auto& idx : get_instances())
			read_transform(idx, _data.current[idx]);
	}

	void simulator::on_frame() 
	{
		simulate();
	}

	void simulator::on_end_frame() 
	{

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

		const auto& mesh_instances = _mesh_instances->get_components(eid);
		size_t mesh_count = mesh_instances.size();

		if (mesh_count > 0)
		{
			// Sum up all mesh centers of this entity
			for (auto miid : mesh_instances)
			{
				mesh_id mid = _mesh_instances->get_mesh(miid);
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
		entity_id eid = get_entity(idx);
		set_com(idx, calculate_com(eid));
	}

	void simulator::set_com(physics_id idx, const vector3& com)
	{
		entity_id eid = get_entity(idx);

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
		entity_id eid = get_entity(idx);
		transform_id trf_id = _transforms->get_component(eid);

		if (_transforms->is_valid(trf_id))
		{
			state.position = _transforms->get_position(trf_id);
			state.orientation = _transforms->get_rotation(trf_id);
		}
	}

	void simulator::write_transform(physics_id idx, PhysicsState& state)
	{
		entity_id eid = get_entity(idx);
		transform_id trf_id = _transforms->get_component(eid);

		if (_transforms->is_valid(trf_id))
		{
			_transforms->set_position(trf_id, state.position);
			_transforms->set_rotation(trf_id, state.orientation);
		}
	}

#define MAX_SEEK_ITERATIONS 32
#define FRICTION_TOLERANCE 0.00001f

	void simulator::simulate()
	{

		while (accumulator >= dt)
		{
			// March entire simulation forward one timestep
			for (const auto& idx : get_instances())
			{
				apply_impulses(idx, _data.current[idx]);
				_data.previous[idx] = _data.current[idx];
				integrate(idx, _data.current[idx], t, dt);	
				write_transform(idx, _data.current[idx]);
			}

			// Update narrow collision (detect all collisions since last frame)
			_colliders->test_scene();

			// Iterate through colliders
			for (const auto& col_a : _colliders->get_instances())
			{

				entity_id e_a = _colliders->get_entity(col_a);
				physics_id phys_a = get_component(e_a);

				PhysicsState& state_a = _data.current[phys_a];

				if (!is_valid(phys_a))
					continue;

				// Iterate through all collisions for this collider
				for (const auto& col: _colliders->get_collisions(col_a))
				{

					collider_id col_b = col.object1;

					assert(("Colliders should not be same!", col_a != col_b)); //NOLINT

					entity_id e_b = _colliders->get_entity(col_b);
					physics_id phys_b = get_component(e_b);

					PhysicsState& state_b = _data.current[phys_b];

					vector3 ra = col.point1 - (_data.com[phys_a] + state_a.position);
					vector3 rb = col.point2 - (_data.com[phys_b] + state_b.position);

					// Response force

					vector3 vpa = state_a.velocity + vector3::cross(state_a.angular_velocity, ra);
					vector3 vpb = state_b.velocity + vector3::cross(state_b.angular_velocity, rb);

					float vrel = vector3::dot(col.normal, (vpa - vpb));

					if (vrel > 0)
						continue;

					float rest = std::min(_data.restitution[phys_a], _data.restitution[phys_b]);
					float num = -(1.0f + rest) * vrel;

					float m1 = _data.inverse_mass[phys_a];
					float m2 = _data.inverse_mass[phys_b];

					vector3 r1 = _data.inverse_inertia[phys_a] * vector3::cross(ra, col.normal);
					vector3 ra1 = vector3::cross(r1, ra);

					vector3 r2 = _data.inverse_inertia[phys_b] * vector3::cross(rb, col.normal);
					vector3 rb2 = vector3::cross(r2, rb);

					float denom = m1 + m2 + vector3::dot(ra1 + rb2, col.normal);

					float j = std::max(num / denom, 0.0f) * response_force_mult;

					PointForce response(col.point1, j * col.normal);
					add_impulse(phys_a, response);

					// Penalty force
					
					if (collision_penalty)
					{
						float k = col.depth * penalty_force_mult;

						PointForce penalty(col.point1, k * col.normal);
						add_impulse(phys_a, penalty);
					}

					// Coloumb friction
					
					vector3 tv = vpa - col.normal * vector3::dot(vpa, col.normal);

					if (friction && tv.square_magnitude() > FRICTION_TOLERANCE)
					{
						vector3 tangent = tv.norm();

						float vt = vector3::dot(vpa, tangent);
						float kt = _data.inverse_mass[phys_a] + 
							vector3::dot(vector3::cross(ra, tangent), 
									_data.inverse_inertia[phys_a] * vector3::cross(ra, tangent));

						float u = std::min(_data.friction[phys_a], _data.friction[phys_b]);

						float jf = std::clamp(-vt / kt, -j * u, j * u);

						PointForce friction(col.point1, jf * tangent);

						add_impulse(phys_a, friction);
					}

					// Rejection

					if (collision_rejection 
							&& _data.inverse_mass[phys_a] > 0.0001f
							&& col.depth > reject_pen_depth)
						state_a.position += col.normal * col.depth;
				}
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

	void simulator::apply_impulses(physics_id idx, PhysicsState& state)
	{
		auto& impulses = _data.impulses[idx];

		for (const auto& impulse : impulses)
		{
			state.momentum += impulse.force;
			state.angular_momentum += vector3::cross(impulse.p - (_data.com[idx] + state.position), impulse.force);
		}

		impulses.clear();

		recalculate_state(idx, state);
	}

	vector3 simulator::acceleration(physics_id idx, const PhysicsState& state, float t)
	{
		vector3 gravity = _data.mass[idx] * 0.0982f * vector3(0, -1, 0) * gravity_mult;
		vector3 air_drag = state.velocity * air_drag_mult;

		return gravity - air_drag;
	}

	vector3 simulator::torque(physics_id idx, const PhysicsState& state, float t)
	{
		return vector3(0);
	}
}
