#include "manager.h"
#include "ifmgr.h"

#include <chrono>
#include <vector>
#include <memory>

#include "phys_data.h"

namespace efiilj
{
	typedef int physics_id;

	class simulator : public manager<physics_id>
	{
		private:

	public:

		simulator();
		~simulator();

		void extend_defaults(physics_id) override;
		void draw_gui() override;
		void draw_gui(physics_id) override;

		void on_register(std::shared_ptr<manager_host> host) override;
	};
}
