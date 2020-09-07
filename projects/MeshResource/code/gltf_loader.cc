#include "gltf_loader.h"

#include <stdio.h>

#define TINYGLTF_IMPLEMENTATION

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif


using namespace tinygltf;

namespace efiilj
{
	gltf_model_loader::gltf_model_loader()
	{

	}

	gltf_model_loader::gltf_model_loader(char* path)
	{
		int err = load_from_file(path);
	}

	int gltf_model_loader::load_from_file(char* path)
	{
		Model model;
		TinyGLTF loader;
		std::string err;
		std::string warn;

		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
		//bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		if (!ret) {
			printf("Failed to parse glTF\n");
			return -1;
		}
	}

	gltf_model_loader::~gltf_model_loader()
	{
		printf("GLTF loader exited");
	}
}

