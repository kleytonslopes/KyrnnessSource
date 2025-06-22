#pragma once

#ifndef KYRNNES_CORE_RUNTIME_WINDOW_HPP
#define KYRNNES_CORE_RUNTIME_WINDOW_HPP

#include "Core/Core.hpp"
#include "Input/InputKey.hpp"
#include "SDL3/SDL.h"

class UApplication;

class UWindow
{
protected:

public:
    

    UWindow(UApplication* application);
    virtual ~UWindow();

    UWindow(const UWindow&) = default;
    UWindow& operator=(const UWindow&) = default;
    UWindow(UWindow&&) = default;
    UWindow& operator=(UWindow&&) = default;

    virtual void Initialize() = 0;
    virtual void Destroy() = 0;
    virtual void PollEvents() = 0;
	virtual void EndFrame() = 0;
    virtual void WaitIdle() = 0;
    virtual bool ShouldClose() const = 0;
	virtual void SetTitle(const std::string& title) = 0;
    virtual void OnMouseMove(float x, float y) = 0;
    virtual void OnMouseButton(int button, int action, int mods) = 0;
	virtual void GetMousePosition(float& x, float& y) const = 0;
    virtual bool GetMouseButtonState(int button) const = 0;

    uint32 GetWidth() const;
    uint32 GetHeight() const;

    virtual void SetShowMouseCursor(bool show) { bShowMouseCursor = show; };
    bool GetShowMouseCursor() const { return bShowMouseCursor; }

    void SetShouldClose(bool value) { bShouldClose = true; }

protected:
    UApplication* m_Application = nullptr;
    bool bShouldClose = false;
	bool bShowMouseCursor = true;

    //void FramebufferResizeCallback(int32 width, int32 height);
};

#endif // KYRNNES_CORE_RUNTIME_WINDOW_HPP