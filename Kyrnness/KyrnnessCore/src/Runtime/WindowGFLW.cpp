#include "pch.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Runtime/WindowGFLW.hpp"
#include "Runtime/Application.hpp"
#include "Input/InputManager.hpp"
#include "GLFW/glfw3.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	GameConfig& cfg = UApplication::Get().GetGameConfig();
	cfg.m_Width = width;
	cfg.m_Height = height;
}

UWindowGLFW::UWindowGLFW(UApplication* application) : UWindow(application)
{

}

void UWindowGLFW::Initialize()
{
	if (!glfwInit())
	{
		ThrowRuntimeError("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Obter o monitor primário e seu modo de vídeo
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	// Criar janela em fullscreen com a resolução do monitor
	m_glfwWindow = glfwCreateWindow(mode->width, mode->height, "Kyrnness OpenGL", primaryMonitor, nullptr);

	if (!m_glfwWindow)
	{
		glfwTerminate();
		ThrowRuntimeError("Failed to create window");
	}

	// Atualizar as configurações do jogo com a resolução real
	GameConfig& cfg = UApplication::Get().GetGameConfig();
	cfg.m_Width = mode->width;
	cfg.m_Height = mode->height;

	glfwMakeContextCurrent(m_glfwWindow);
	glfwSetFramebufferSizeCallback(m_glfwWindow, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ThrowRuntimeError("Failed to initialize GLAD");
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	io.ConfigFlags != ImGuiConfigFlags_NavEnableSetMousePos;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//glfwSetInputMode(m_glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto* self = static_cast<UWindowGLFW*>(glfwGetWindowUserPointer(window));
			if (self)
				self->OnMouseButton(button, action, mods);

			ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		});

	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			///std::cout << "glfwSetCursorPosCallback:  " << xpos << " x " << ypos << std::endl;

			//auto* self = static_cast<UWindowGLFW*>(glfwGetWindowUserPointer(window));
			//if (self)
			//	self->OnMouseMove(static_cast<float>(xpos), static_cast<float>(ypos));

			ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
		});
}

void UWindowGLFW::Destroy()
{
	glfwTerminate();

	LOG(Warning, "UWindowGLFW::Destroy");
}

void UWindowGLFW::PollEvents()
{
	ProcessInput();

	glfwPollEvents();


}

void UWindowGLFW::EndFrame()
{
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Render platform windows if enabled
	//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//}
	glfwSwapBuffers(m_glfwWindow);
}

void UWindowGLFW::WaitIdle()
{

}

bool UWindowGLFW::ShouldClose() const
{
	return glfwWindowShouldClose(m_glfwWindow);
}

void UWindowGLFW::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(m_glfwWindow, title.c_str());
}

void UWindowGLFW::OnMouseMove(float x, float y)
{
	//std::cout << "UWindowGLFW::OnMouseMove: " << x << " x " << y << std::endl;

	if (bFirstMouse)
	{
		m_LastMouseX = x;
		m_LastMouseY = y;
		bFirstMouse = false;
	}

	float deltaX = x - m_LastMouseX;
	float deltaY = m_LastMouseY - y;

	m_LastMouseX = x;
	m_LastMouseY = y;

	deltaX *= 0.01f; // Adjust sensitivity as needed
	deltaY *= 0.01f; // Adjust sensitivity as needed

	//UInputManager::Get().OnMouseMove(deltaX, deltaY);
	/*UInputManager::Get().OnMousePosition(x, y);*/
}

void UWindowGLFW::SetShowMouseCursor(bool show)
{
	UWindow::SetShowMouseCursor(show);

	if (show)
	{
		bFirstMouse = true; // Reset first mouse flag to recenter on next move
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void UWindowGLFW::OnMouseButton(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		double x, y;
		glfwGetCursorPos(m_glfwWindow, &x, &y);

		// Converte coordenadas se necessário
		y = GetHeight() - y; // Inverte Y se seu sistema tem Y para baixo

		// Notifica o InputManager
		UInputManager::Get().OnMouseButton(x, y,
			action == GLFW_PRESS, action == GLFW_RELEASE);
	}
}

void UWindowGLFW::GetMousePosition(float& x, float& y) const
{
	double mouseX, mouseY;
	glfwGetCursorPos(m_glfwWindow, &mouseX, &mouseY);

	x = static_cast<float>(mouseX);
	y = static_cast<float>(mouseY);
}

bool UWindowGLFW::GetMouseButtonState(int button) const
{
	return glfwGetMouseButton(m_glfwWindow, button);
}

void UWindowGLFW::InitializeForOpenGL()
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_glfwWindow = glfwCreateWindow(GetWidth(), GetHeight(), "Kyrnness OpenGL", nullptr, nullptr);

	glfwMakeContextCurrent(m_glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ThrowRuntimeError("Failed to initialize GLAD with GLFW");
	}
}

void UWindowGLFW::ProcessInput()
{
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_glfwWindow, true);

	float x, y;	GetMousePosition(x,y);
		//std::cout << "ProcessInput:  " << x << " x " << y << std::endl;

	UInputManager::Get().OnMousePosition(x, y);

	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
	{
		int state = glfwGetKey(m_glfwWindow, key);

		if (state == GLFW_PRESS)	    UInputManager::Get().SetKeyPressed(key, EInputKeyStatus::IKS_Pressed);
		else if (state == GLFW_RELEASE) UInputManager::Get().SetKeyPressed(key, EInputKeyStatus::IKS_Released);

		
	}
}
