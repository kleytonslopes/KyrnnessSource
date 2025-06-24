#pragma once

#ifndef KYRNNES_CORE_GRAPHICS_GRAPHICS_API_HPP
#define KYRNNES_CORE_GRAPHICS_GRAPHICS_API_HPP  

#include "Core/Core.hpp"
#include "Core/Vertex.hpp"
#include "Runtime/Window.hpp"

class UApplication;
class UScene;

class UGraphicsApi
{
public:
    UGraphicsApi(UApplication* application);
    virtual ~UGraphicsApi();

    UGraphicsApi(const UGraphicsApi&) = default;
    UGraphicsApi& operator=(const UGraphicsApi&) = default;
    UGraphicsApi(UGraphicsApi&&) = default;
    UGraphicsApi& operator=(UGraphicsApi&&) = default;

    virtual void Initialize() = 0;
    virtual void Destroy() = 0;
    virtual void DrawFrame(float deltaTime) = 0;
    virtual void WaitIdle() = 0;
    virtual void StartFrame() = 0;
    virtual void EndFrame() = 0;

#ifdef DEBUG
    virtual void DrawDebugLine(const glm::vec3& origin, const glm::vec3& target, const glm::vec4& color) = 0;
#endif // DEBUG

protected:
    UApplication* m_Application = nullptr;
	//UScene* m_Scene = nullptr;
};

#endif // KYRNNES_CORE_GRAPHICS_GRAPHICS_API_HPP