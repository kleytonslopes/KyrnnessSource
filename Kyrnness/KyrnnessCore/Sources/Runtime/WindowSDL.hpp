/*********************************************************************
 *   File: WindowSDL.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef KYRNNES_CORE_RUNTIME_WINDOW_SDL_HPP 
#define KYRNNES_CORE_RUNTIME_WINDOW_SDL_HPP

#include "Runtime/Window.hpp"
#include <SDL3/SDL.h>

class UWindowSDL : public UWindow
{
public:
    UWindowSDL(UApplication* application);

    UWindowSDL(const UWindowSDL&) = default;
    UWindowSDL& operator=(const UWindowSDL&) = default;
    UWindowSDL(UWindowSDL&&) = default;
    UWindowSDL& operator=(UWindowSDL&&) = default;

    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void PollEvents() override;
    virtual void EndFrame() override;
    virtual void WaitIdle() override;
    virtual bool ShouldClose() const override;
    virtual void SetTitle(const std::string& title) override;
    virtual void OnMouseMove(float x, float y) override;
    virtual void OnMouseButton(int button, int action, int mods) override { }
    virtual void GetMousePosition(float& x, float& y) const override {}
    virtual bool GetMouseButtonState(int button) const { return false; }
    virtual void SetShouldClose(bool value) override {}

private:
    SDL_Event m_SdlEvent;
    SDL_Window* m_SdlWindow = nullptr;
    SDL_Surface* m_SdlSurface = nullptr;
    SDL_Renderer* m_SdlRenderer = nullptr;

    SDL_GLContext m_SdlGLContext;

    void InitializeForVulkan();
    void InitializeForOpenGL();

    uint32 GetFlagsForVulkan();
    uint32 GetFlagsForOpenGL();
};

#endif // KYRNNES_CORE_RUNTIME_WINDOW_SDL_HPP