#include "manager.h"
#include "ifmgr.h"

#include <chrono>
#include <vector>
#include <memory>

#include "phys_data.h"

namespace efiilj
{
	typedef int physics_id;

	typedef int physics_state;

	typedef std::chrono::duration<float> duration;
	typedef std::chrono::high_resolution_clock frame_timer;
	typedef std::chrono::time_point<frame_timer> frame_time;

	class simulator : public manager<physics_id>
	{
		private:

			struct Derivative
			{
				vector3 velocity;
				vector3 force;
				quaternion spin;
				vector3 torque;
			};

			struct PhysicsState
			{
				// PRIMARY
				// === Linear
				std::vector<vector3> position;
				std::vector<vector3> momentum;

				// === Angular
				std::vector<quaternion> orientation;
				std::vector<vector3> angular_momentum;

				// SECONDARY
				// === Linear
				std::vector<vector3> velocity;

				// === Angular
				std::vector<quaternion> spin;
				std::vector<vector3> angular_velocity;

				// CONSTANT
			};

			struct PhysicsConstants
			{
				std::vector<float> mass;
				std::vector<float> inverse_mass;
				std::vector<float> inertia;
				std::vector<float> inverse_inertia;
			};

			float t = 0.0f;
			float dt = 0.01f;

			frame_time current_time = frame_timer::now();

			float accumulator = 0.0f;

			PhysicsState _current;
			PhysicsState _previous;
			PhysicsConstants _constants;

			std::shared_ptr<transform_manager> _transforms;

	public:

		simulator();
		~simulator();

		void extend_defaults(physics_id) override;
		void draw_gui() override;
		void draw_gui(physics_id) override;

		void on_register(std::shared_ptr<manager_host> host) override;

		void extend_state(PhysicsState& state, physics_id idx);
		void swap_state(physics_id idx);
		void recalculate_state(PhysicsState& state, physics_id idx);

		void read_transform(physics_id);
		void write_transform(physics_id);

		void begin_frame();
		void simulate();
		void end_frame();

		Derivative evaluate(physics_id idx, const Derivative& d, float t, float dt);
		void integrate(physics_id idx, const float t, const float dt);

		vector3 acceleration(physics_id idx, float t);
		vector3 torque(physics_id idx, float t);

		void set_mass(physics_id idx, float mass)
		{
			_constants.mass[idx] = mass;
			_constants.inverse_mass[idx] = 1.0f / mass;
		}

		void set_inertia(physics_id idx, float inertia)
		{
			_constants.inertia[idx] = inertia;
			_constants.inverse_inertia[idx] = 1.0f / inertia;
		}
	};
}
