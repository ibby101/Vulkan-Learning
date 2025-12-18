#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "VulkanModelLoader.h"

void VulkanModelLoader::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())){
		throw std::runtime_error(err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	size_t totalOriginalVertices = 0;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {

			// checking total vertices prior to deduplication
			totalOriginalVertices++;


			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	// Calculate bounding box
	glm::vec3 minBounds(FLT_MAX);
	glm::vec3 maxBounds(-FLT_MAX);

	for (const auto& vertex : vertices) {
		minBounds = glm::min(minBounds, vertex.pos);
		maxBounds = glm::max(maxBounds, vertex.pos);
	}

	// Calculate center and scale
	glm::vec3 center = (minBounds + maxBounds) * 0.5f;
	glm::vec3 size = maxBounds - minBounds;
	float maxDimension = std::max(std::max(size.x, size.y), size.z);
	float scale = 1.5f / maxDimension;  // Target size of 1.5 units

	// Apply transformations to all vertices
	for (auto& vertex : vertices) {
		vertex.pos = (vertex.pos - center) * scale;
	}

	std::cout << "Model centered and scaled by " << scale << std::endl;

	//std::cout << "Total number of vertices before deduplication: " << totalOriginalVertices << std::endl;
	//std::cout << "Total number of vertices after deduplication: " << vertices.size() << std::endl;
	//std::cout << "Duplicate Removal Ratio:  " << ((float)vertices.size() / totalOriginalVertices) * 100.0f <<  "%" << std::endl;
}