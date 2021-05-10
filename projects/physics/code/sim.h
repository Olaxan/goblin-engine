#include <chrono>
#include <vector>
#include <memory>

#include "phys_data.h"

namespace efiilj
{
	class simulator
	{
		private:

		std::vector<std::shared_ptr<physics_data>> _bodies;

	private:

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		frame_timer_point last_frame_;
		duration delta_time_;

	public:

		void add_rigidbody(std::shared_ptr<physics_data> body)
		{
			this->_bodies.push_back(body);
		}

		void add_rigidbody(const std::vector<std::shared_ptr<physics_data>>& bodies)
		{
			for (auto& body : bodies)
				add_rigidbody(body);
		}

		const std::vector<std::shared_ptr<physics_data>>& get_rigidbodies() const 
		{ 
			return _bodies;
		}
	};
}
