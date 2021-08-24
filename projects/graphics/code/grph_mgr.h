#pragma once

#include "manager.h"
#include "trfm_mgr.h"

#include <vector>
#include <memory>
#include <filesystem>
#include <string>

namespace efiilj
{
	typedef int graphics_id;

	class graphics_manager : public manager<graphics_id>, public registrable
	{
		private:

			struct GraphicsData
			{
				std::vector<std::filesystem::path> model_uri;
				std::vector<std::filesystem::path> geometry_shader_uri;
				std::vector<std::filesystem::path> lighting_shader_uri;
				std::vector<transform_id> transform;
				std::vector<bool> loaded;
			} _data;

			std::shared_ptr<transform_manager> _transforms;

		public:

			graphics_manager();
			~graphics_manager();

			graphics_id register_entity(entity_id eid) override;
			bool unregister_entity(graphics_id idx) override;

			void draw_gui() override;
			void draw_gui(graphics_id idx) override;

			void on_register(std::shared_ptr<manager_host> host) override;

			void load(graphics_id idx);

			const std::filesystem::path& get_model_uri(graphics_id idx);
			void set_model_uri(graphics_id idx, const std::filesystem::path& uri);

			const std::filesystem::path& get_geometry_shader_uri(graphics_id idx);
			void set_geometry_shader_uri(graphics_id idx, const std::filesystem::path& uri);

			const std::filesystem::path& get_lighting_shader_uri(graphics_id idx);
			void set_lighting_shader_uri(graphics_id idx, const std::filesystem::path& uri);

			const transform_id& get_transform(graphics_id idx);
			void set_transform(graphics_id idx, transform_id trf);
	};
}
