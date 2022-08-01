#pragma once

#define GLM_FORCE_RADIANS

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/ext.hpp>

constexpr auto Y_MIN = -90;
constexpr auto Y_MAX = 90;

struct Movement {
	glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 dirN = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
};