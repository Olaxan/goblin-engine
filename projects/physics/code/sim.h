#include "manager.h"
#include "ifmgr.h"

#include <chrono>
#include <vector>
#include <memory>

#include "phys_data.h"

namespace efiilj
{
	typedef int physics_id;

	class simulator : public manager<physics_id>, public registrable
	{
		private:

		std::vector<std::shared_ptr<physics_node>> _bodies;

		typedef std::chrono::duration<float> duration;
		typedef std::chrono::high_resolution_clock frame_timer; 
		typedef std::chrono::time_point<frame_timer> frame_timer_point;

		frame_timer_point last_frame_;
		duration delta_time_;

	public:

		physics_id register_entity(entity_id eid) override;
		bool unregister_entity(physics_id idx) override;

		void draw_gui() override;
		void draw_gui(physics_id) override;

		void add_rigidbody(std::shared_ptr<physics_node> body)
		{
			this->_bodies.push_back(body);
		}

		void add_rigidbody(const std::vector<std::shared_ptr<physics_node>>& bodies)
		{
			for (auto& body : bodies)
				add_rigidbody(body);
		}

		const std::vector<std::shared_ptr<physics_node>>& get_rigidbodies() const 
		{ 
			return _bodies;
		}
	};
}
