#include "tiny_gltf.h"

namespace efiilj 
{
	class gltf_model_loader
	{
	private:

	public:

		gltf_model_loader();
		gltf_model_loader(char* path);

		gltf_model_loader(gltf_model_loader& copy)
			= default;

		gltf_model_loader(gltf_model_loader&& move)
			= default;
		
		int load_from_file(char* path);

		~gltf_model_loader();
	};
}
