#pragma once

#ifndef KYRNNES_CORE_GRAPHICS_GRAPHICS_API_OPENGL_HPP
#define KYRNNES_CORE_GRAPHICS_GRAPHICS_API_OPENGL_HPP

#include "Graphics/GraphicsApi.hpp"
#include <entt/entt.hpp>

class UGraphicsApi_OpenGL : public UGraphicsApi
{
public:
    UGraphicsApi_OpenGL(UApplication* application);
    virtual ~UGraphicsApi_OpenGL();

    UGraphicsApi_OpenGL(const UGraphicsApi_OpenGL&) = default;
    UGraphicsApi_OpenGL& operator=(const UGraphicsApi_OpenGL&) = default;
    UGraphicsApi_OpenGL(UGraphicsApi_OpenGL&&) = default;
    UGraphicsApi_OpenGL& operator=(UGraphicsApi_OpenGL&&) = default;

    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void DrawFrame(float deltaTime) override;
    virtual void WaitIdle() override;
    virtual void StartFrame() override;
    virtual void EndFrame() override;
    virtual void DrawDebugLine(const glm::vec3& origin, const glm::vec3& target, const glm::vec4& color) override;
    void DebugDrawLine2D(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color);
    

private:
  
    void DebugDrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color);

	entt::registry& m_EnttRegistry;
};

#endif // KYRNNES_CORE_GRAPHICS_GRAPHICS_API_HPP