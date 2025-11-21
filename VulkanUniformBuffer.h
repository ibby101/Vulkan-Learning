#pragma once
#include "CommonHeaders.h"
#include "VulkanBuffer.h"

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 project;
};