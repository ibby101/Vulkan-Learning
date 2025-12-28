#pragma once

#include "CommonHeaders.h"
#include <unordered_map>

class VulkanModelLoader {

public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void loadModel();
};