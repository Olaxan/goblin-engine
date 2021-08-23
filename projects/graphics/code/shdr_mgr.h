#pragma once

#include "manager.h"

#include <string>

namespace efiilj
{
	typedef int shader_id;

	class shader_manager : public manager<shader_id>
	{
		private:

			struct ShaderData
			{
				std::vector<std::string> uri;
				std::vector<unsigned int> program_id;
				std::vector<unsigned int> type;
				std::vector<bool> state;
			} _data;

		public:

			shader_manager();
			~shader_manager();

			shader_id register_entity(entity_id eid) override;
			bool unregister_entity(shader_id idx) override;

			void draw_gui() override;
			void draw_gui(shader_id idx) override;



	};
}
