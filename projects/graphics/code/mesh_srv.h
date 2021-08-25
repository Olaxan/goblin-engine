#pragma once

#include "server.h"
#include "mgr_host.h"

#include <filesystem>

namespace efiilj
{
	typedef int mesh_id;

	class mesh_server : public server<mesh_id>, public registrable
	{
		private:

			struct MeshData 
			{
			} _data;
			
			bool buffer(mesh_id idx);

		public:

			mesh_server();
			~mesh_server();

			mesh_id create() override;
			bool destroy(mesh_id idx) override;

			bool load(mesh_id idx);
			bool bind(mesh_id idx, unsigned int slot) const;

			const std::filesystem::path& get_uri(mesh_id idx) const;
			void set_uri(mesh_id idx, const std::filesystem::path& uri);

	};
}
