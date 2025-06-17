#pragma once
#ifndef KYRNNES_RENDER_PARAMETERS_HPP
#define KYRNNES_RENDER_PARAMETERS_HPP

#include "glm/glm.hpp"

struct TRenderParameters
{
	float deltaTime = 0.0f;
	
	glm::vec3 location = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
};

#endif// KYRNNES_RENDER_PARAMETERS_HPP
