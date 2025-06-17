#pragma once

#ifndef KYRNESS_CAMERA_HPP
#define KYRNESS_CAMERA_HPP

#include "Components/Component.hpp"

struct FTransformComponent;

class FCameraComponent : public FComponent
{
public:
	float Fov = 45.0f;
	float Near = 0.0001f;
	float Far = 100000.f;

	float Yaw = 0.0f;
	float Pitch = 0.0f;

	glm::vec3 LocationOffset{0.f};

	float GetAspectRatio() const;

	FMatrix GetProjectionMatrix() const;
	FMatrix GetViewMatrix() const;
	
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;

	void UpdateAspectRatio(float width, float height);
	void UpdateDirectionFromYawPitch(float yaw, float pitch);

	virtual nlohmann::json GetJsonData() override;

private:
	float AspectRatio = 1.0f;
};

#endif // KYRNESS_CAMERA_HPP