#pragma once

#include "server.h"
#include "mgr_host.h"

#include "shdr_mgr.h"

namespace efiilj
{
	typedef int material_id;

	class material_server : public server<material_id>, public registrable
	{
		private:

			struct MaterialData
			{
				std::vector<shader_id> shader;
				std::vector<int> textures;
			} _data;
			
		public:

			material_server();
			~material_server();

			material_id create() override;
			bool destroy(material_id idx) override;

	};
}
