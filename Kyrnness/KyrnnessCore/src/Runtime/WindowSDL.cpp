#include "pch.hpp"
#include "Runtime/WindowSDL.hpp"
#include "Runtime/Application.hpp"

UWindowSDL::UWindowSDL(UApplication* application) : UWindow(application)
{
}

void UWindowSDL::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		ThrowRuntimeError("Failed to Initialize SDL");
	}

	InitializeForOpenGL();
}

void UWindowSDL::Destroy()
{
	SDL_DestroyWindow(m_SdlWindow);
	SDL_Quit();

	LOG(Warning, "UWindowSDL::Destroy");
}

void UWindowSDL::PollEvents()
{
	SDL_GL_MakeCurrent(m_SdlWindow, m_SdlGLContext);

	while (SDL_PollEvent(&m_SdlEvent) != 0)
	{
		switch (m_SdlEvent.type)
		{
		case SDL_EVENT_WINDOW_RESIZED:
			if (m_SdlEvent.window.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
			{
				//FramebufferResizeCallback(m_SdlEvent.window.data1, m_SdlEvent.window.data2);
			}
			break;
		case SDL_EVENT_QUIT:
			bShouldClose = true;
			break;
		default:

			break;
		}
	}
}

void UWindowSDL::EndFrame()
{
}

void UWindowSDL::WaitIdle()
{
}

bool UWindowSDL::ShouldClose() const
{
    return bShouldClose;
}

void UWindowSDL::SetTitle(const std::string& title)
{
}

void UWindowSDL::OnMouseMove(float x, float y)
{
}

void UWindowSDL::InitializeForVulkan()
{
	uint32 windowFlags = GetFlagsForVulkan();

	m_SdlWindow = SDL_CreateWindow("Kyrnness Vulkan", m_Application->GetWidth(), m_Application->GetHeight(), windowFlags);
}

void UWindowSDL::InitializeForOpenGL()
{
	uint32 windowFlags = GetFlagsForOpenGL();
	int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef _DEBUG
	context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif // DEBUG

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	m_SdlWindow = SDL_CreateWindow("Kyrnness OpenGL", m_Application->GetWidth(), m_Application->GetHeight(), windowFlags);

	m_SdlRenderer = SDL_CreateRenderer(m_SdlWindow, "");
	SDL_SetRenderDrawColor(m_SdlRenderer, 50, 50, 50, 1);
	SDL_RenderClear(m_SdlRenderer);
	SDL_RenderPresent(m_SdlRenderer);

	m_SdlSurface = SDL_GetWindowSurface(m_SdlWindow);
	///SDL_FillSurfaceRect(m_SdlSurface, NULL, SDL_MapRGB(m_SdlSurface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(m_SdlWindow);

	m_SdlGLContext = SDL_GL_CreateContext(m_SdlWindow);


	SDL_GL_MakeCurrent(m_SdlWindow, m_SdlGLContext);
	SDL_GL_SetSwapInterval(1); // Enable vsync
}

uint32 UWindowSDL::GetFlagsForVulkan()
{
	return SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
}

uint32 UWindowSDL::GetFlagsForOpenGL()
{
	return SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
}
