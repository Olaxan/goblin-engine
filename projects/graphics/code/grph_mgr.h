#pragma once

#include "manager.h"

#include "trfm_mgr.h"

#include <vector>
#include <memory>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace efiilj
{
	typedef int graphics_id;

	class graphics_manager : public manager<graphics_id>
	{
		private:

			struct GraphicsData
			{
				std::vector<fs::path> uri;
				std::vector<transform_id> transform;
				std::vector<bool> loaded;
			} _data;

			std::shared_ptr<transform_manager> _transforms;

		public:

			graphics_manager(std::shared_ptr<transform_manager> trf_mgr);
			~graphics_manager();

			graphics_id register_entity(entity_id eid);
			bool unregister_entity(graphics_id idx);

			void load(graphics_id idx);

			const fs::path& get_uri(graphics_id idx);
			void set_uri(graphics_id idx, const fs::path& uri);

			const transform_id& get_transform(graphics_id idx);
			void set_transform(graphics_id idx, transform_id trf);
	};
}
