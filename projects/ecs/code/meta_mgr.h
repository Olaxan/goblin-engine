#pragma once

#include "manager.h"
#include "mgr_host.h"

#include <string>

namespace efiilj
{

	typedef int meta_id;

	class meta_manager : public manager<meta_id>
	{
		private:

			struct MetaData
			{
				ComponentData<std::string> name;
				ComponentData<std::string> description;
			} _data;

		public:

			meta_manager();
			~meta_manager();

			void on_editor_gui() override;
			void on_editor_gui(meta_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			const std::string& get_name(meta_id idx)
			{
				return _data.name[idx];
			}

			void set_name(meta_id idx, const std::string& name)
			{
				_data.name[idx] = name;
			}

			const std::string& get_description(meta_id idx) const
			{
				return _data.description[idx];
			}

			void set_description(meta_id idx, const std::string& desc)
			{
				_data.description[idx] = desc;
			}
	};
}
