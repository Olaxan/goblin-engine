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

			struct PhysicsState 
			{
				// PRIMARY
				std::vector<vector3> position;
				std::vector<quaternion> orientation;
				std::vector<vector3> angular_momentum;

				// SECONDARY
				std::vector<quaternion> spin;
				std::vector<vector3> angular_velocity;

				std::vector<float> inertia;
				std::vector<float> inverse_inertia;
			};

			float t = 0.0f;
			float dt = 0.01f;

			frame_time current_time = frame_timer::now();

			float accumulator = 0.0f;


			PhysicsState _current;
			PhysicsState _previous;

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

		void begin_frame();
		void simulate();
		void end_frame();

		void rk4(physics_id idx, const float t, const float dt);
	};
}
