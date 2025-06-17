#pragma once

#ifndef KYRNESS_TRANSFORM_HPP
#define KYRNESS_TRANSFORM_HPP

#include "Components/Component.hpp"
#include "glm/glm.hpp"

class FTransformComponent : public FComponent
{
public:
	FVector Location{ 0.f };
	FVector Rotation{ 0.f };
	FVector Scale{ 1.f };
	FVector ForwardVector{ 1.f, 0.f, 0.f };
	FVector UpVector{ 0.f, 1.f, 0.f };
	FVector RightVector = { 0.f, 0.f, 1.f };
	//glm::vec3 RightVector = glm::normalize(glm::cross(ForwardVector, UpVector));
};

#endif // KYRNESS_TRANSFORM_HPP