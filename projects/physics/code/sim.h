#pragma once

#include "manager.h"
#include "ifmgr.h"

#include <chrono>
#include <vector>
#include <memory>
#include <queue>

#include "phys_data.h"

namespace efiilj
{
	typedef int physics_id;

	typedef int physics_state;

	typedef std::chrono::duration<float> duration;
	typedef std::chrono::high_resolution_clock frame_timer;
	typedef std::chrono::time_point<frame_timer> frame_time;

	struct PointForce
	{
		vector3 p;
		vector3 force;

		PointForce()
			: p(vector3(0)), force(vector3(0)) { }

		PointForce(const vector3& p, const vector3& force)
			: p(p), force(force) { }

	};

	class simulator : public manager<physics_id>
	{
		private:

			struct Derivative
			{
				vector3 velocity; 	// dx/dt (position change over time)
				vector3 force;		// dv/dt (velocity change over time)
				quaternion spin;	// dr/dt (orientation change over time)
				vector3 torque;		// dw/dt (angular velocity change over time)
			};

			struct PhysicsState
			{
				// PRIMARY
				// === Linear
				vector3 position;
				vector3 momentum;

				// === Angular
				quaternion orientation;
				vector3 angular_momentum;

				// SECONDARY
				// === Linear
				vector3 velocity;

				// === Angular
				quaternion spin;
				vector3 angular_velocity;
			};

			struct PhysicsData
			{
				ComponentData<PhysicsState> current;
				ComponentData<PhysicsState> previous;

				ComponentData<std::vector<PointForce>> impulses;
				ComponentData<std::vector<PointForce>> forces;

				ComponentData<vector3> com;
				ComponentData<float> mass;
				ComponentData<float> inverse_mass;
				ComponentData<float> inertia;
				ComponentData<float> inverse_inertia;
				ComponentData<float> restitution;
				ComponentData<float> friction;
			} _data;

			float t = 0.0f;
			float dt = 0.01f;

			float gravity_mult = 0.0f;
			float air_drag_mult = 0.0f;

			float response_force_mult = 1.0f;

			bool collision_penalty = true;
			float penalty_force_mult = 0.01f;
			float penalty_force_damp = 0.1f;

			bool collision_rejection = false;
			float reject_pen_depth = 0.1f;

			bool friction = true;

			frame_time current_time = frame_timer::now();

			float accumulator = 0.0f;

			std::shared_ptr<transform_manager> _transforms;
			std::shared_ptr<collider_manager> _colliders;
			std::shared_ptr<mesh_manager> _mesh_instances;
			std::shared_ptr<mesh_server> _meshes;

			void apply_impulses(physics_id, PhysicsState& state);

	public:

		simulator();
		~simulator();

		// Virtual
		void on_editor_gui() override;
		void on_editor_gui(physics_id) override;

		void on_register(std::shared_ptr<manager_host> host) override;
		void on_activate(physics_id idx) override;
		void on_begin_frame() override;
		void on_frame() override;
		void on_end_frame() override;

		// Main
		void simulate();
		bool step(physics_id idx, PhysicsState& state, const float t, const float dt);

		// Utility
		vector3 calculate_com(entity_id eid) const;
		void recalculate_com(physics_id idx);
		void recalculate_state(physics_id idx, PhysicsState& state);

		void read_transform(physics_id, PhysicsState& state);
		void write_transform(physics_id, PhysicsState& state);

		// Integrator
		Derivative evaluate(physics_id idx, const PhysicsState& state, const Derivative& d, float t, float dt);
		void integrate(physics_id idx, PhysicsState& state, const float t, const float dt);

		vector3 acceleration(physics_id idx, const PhysicsState& state, float t);
		vector3 torque(physics_id idx, const PhysicsState& state, float t);

		// Getters and Setters
		void add_impulse(physics_id idx, const PointForce& force)
		{
			_data.impulses[idx].emplace_back(force);
		}

		void add_force(physics_id idx, const PointForce& force)
		{
			_data.forces[idx].emplace_back(force);
		}

		const vector3& get_com(physics_id idx) const
		{
			return _data.com[idx];
		}
		
		const std::vector<PointForce>& get_impulses(physics_id idx) const
		{
			return _data.impulses[idx];
		}

		void set_com(physics_id idx, const vector3& com);

		void set_mass(physics_id idx, float mass)
		{
			_data.mass[idx] = mass;
			_data.inverse_mass[idx] = 1.0f / mass;
		}

		void set_inertia(physics_id idx, float inertia)
		{
			_data.inertia[idx] = inertia;
			_data.inverse_inertia[idx] = 1.0f / inertia;
		}

		void set_static(physics_id idx, bool is_static)
		{
			// TODO: fix
			_data.inverse_mass[idx] = is_static ? 0 : 1.0f / _data.mass[idx];
			_data.inverse_inertia[idx] = is_static ? 0 : 1.0f / _data.inertia[idx];
		}

		void set_inertia_as_cube(physics_id idx, float length)
		{
			set_inertia(idx, 1.0f / 6.0f * (length * length) * _data.mass[idx]);
		};
	};
}
