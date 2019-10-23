#include "rnode.h"

namespace efiilj
{
	rasterizer_node::rasterizer_node(std::vector<vertex> vertices, std::vector<unsigned> indices, std::shared_ptr<transform_model> transform)
	: vertices_(std::move(vertices)), indices_(std::move(indices)), transform_(std::move(transform))
	{ }
}
