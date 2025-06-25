#include "CorePCH.hpp"
#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
#include "Graphics/Shaders.hpp"
#include "Runtime/Application.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Components/MeshRenderer_OpenGLComponent.hpp"
#include "Runtime/WindowGFLW.hpp"
#include "GameFramework/Scene.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

UGraphicsApi_OpenGL::UGraphicsApi_OpenGL(UApplication* application)
	: UGraphicsApi(application)
	, m_EnttRegistry(application->GetEnttRegistry())
{
	FLogger::Log("Initializing OpenGL Graphics API...");

	if (application->GetGraphicsApiType() != EGraphicsApi::GA_OpenGL)
	{
		ThrowRuntimeError("Invalid Graphics API type for OpenGL");
	}
}

UGraphicsApi_OpenGL::~UGraphicsApi_OpenGL()
{

}

void UGraphicsApi_OpenGL::Destroy()
{

}

void UGraphicsApi_OpenGL::Initialize()
{
	uint32 w = m_Application->GetWidth();
	uint32 h = m_Application->GetHeight();
}

void UGraphicsApi_OpenGL::DrawFrame(float deltaTime)
{
	StartFrame();

#ifdef DEBUG
	DrawDebugLine(glm::vec3{ 0, 0, 0 }, glm::vec3{ 1000, 0, 0 }, glm::vec4{ 0.f, 0.f, 1.f, 1.f });
	DrawDebugLine(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1000, 0 }, glm::vec4{ 0.f, 1.f, 0.f, 1.f });
	DrawDebugLine(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 1000 }, glm::vec4{ 1.f, 0.f, 0.f, 1.f });
#endif // DEBUG

	float xpos, ypos;

	m_Application->GetWindow()->GetMousePosition(xpos, ypos);

#ifdef DEBUG
	DebugDrawLine2D(glm::vec3{ 0, 0, 0 }, glm::vec3{ xpos, ypos, 0 }, glm::vec4{ 1.f, 1.f, 1.f, 1.f });
	DebugDrawLine2D(glm::vec3{ 0, 0, 0 }, glm::vec3{ m_Application->GetWidth() , m_Application->GetHeight(), 0 }, glm::vec4{ 0.1f, 0.1f, 0.1f, 1.f });
	DebugDrawLine2D(glm::vec3{ m_Application->GetWidth(), 0, 0 }, glm::vec3{ 0 , m_Application->GetHeight(), 0 }, glm::vec4{ 0.1f, 0.1f, 0.1f, 1.f });
#endif // DEBUG
}

#ifdef DEBUG
void UGraphicsApi_OpenGL::DebugDrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color)
{
	if (UShaderOpenGLComponent* debugShader = UShaders::GetShader(SHADER_DEBUG))
	{
		unsigned int VBO, VAO, CVBO;
		GLfloat points[6];

		points[0] = from.x;
		points[1] = from.y;
		points[2] = from.z;
		points[3] = to.x;
		points[4] = to.y;
		points[5] = to.z;

		GLfloat sColor[6];
		sColor[0] = color.x;
		sColor[1] = color.y;
		sColor[2] = color.z;
		sColor[3] = color.x;
		sColor[4] = color.y;
		sColor[5] = color.z;

		auto entities = m_EnttRegistry.view<UTransformComponent, UCameraComponent>();

		glm::vec3 location = World::ZeroVector;
		glm::vec3 forwardVector = World::ForwardVector;//{ 0.f, 0.f, -1.f };
		glm::vec3 upVector = World::UpVector; // { 0.f, 1.f, 0.f };
		glm::mat4 project{ 0.f };

		entities.each([&](const auto entity, auto& transform, auto& camera)
			{
				// Update transform properties
				location = transform.Location;
				forwardVector = transform.ForwardVector;
				upVector = transform.UpVector;
				// Update camera properties
				project = camera.GetProjectionMatrix();
			});


		glm::mat4 view = glm::lookAt(location, location + forwardVector, upVector);

		glm::mat4 modelMatrix = glm::mat4{ 1.f };//veresse
		glm::vec3 loc = World::ZeroVector;
		glm::vec3 sca{ 1.0f, 1.0f, 1.0f };

		modelMatrix = glm::translate(modelMatrix, loc);
		modelMatrix = glm::scale(modelMatrix, sca);


		debugShader->Bind();
		debugShader->SetMatrix4("view", view);
		debugShader->SetMatrix4("projection", project);
		debugShader->SetMatrix4("model", modelMatrix);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &CVBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, CVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sColor), &sColor, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_LINES, 0, 2);

		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &CVBO);
		glDeleteVertexArrays(1, &VAO);

		debugShader->Unbind();
	}
}

void UGraphicsApi_OpenGL::DebugDrawLine2D(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color)
{
	if (UShaderOpenGLComponent* debugShader = UShaders::GetShader(SHADER_UI_DEBUG))
	{
		unsigned int VBO, VAO, CVBO;
		GLfloat points[6];

		points[0] = from.x;
		points[1] = from.y;
		points[2] = from.z;
		points[3] = to.x;
		points[4] = to.y;
		points[5] = to.z;

		GLfloat sColor[6];
		sColor[0] = color.x;
		sColor[1] = color.y;
		sColor[2] = color.z;
		sColor[3] = color.x;
		sColor[4] = color.y;
		sColor[5] = color.z;


		glm::mat4 view = glm::ortho(0.0f
			, static_cast<float>(UApplication::Get().GetWidth())
			, static_cast<float>(UApplication::Get().GetHeight())
			, 0.0f
			, -1.0f
			, 1.0f);

		glm::mat4 modelMatrix = glm::mat4{ 1.f };//veresse
		glm::vec3 loc = World::ZeroVector;
		glm::vec3 sca{ 1.0f, 1.0f, 1.0f };

		modelMatrix = glm::translate(modelMatrix, loc);
		modelMatrix = glm::scale(modelMatrix, sca);


		debugShader->Bind();
		debugShader->SetMatrix4("uProjection", view);
		debugShader->SetVector4("uColor", color);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &CVBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, CVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sColor), &sColor, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_LINES, 0, 2);

		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &CVBO);
		glDeleteVertexArrays(1, &VAO);

		debugShader->Unbind();
	}
}
#endif // DEBUG

void UGraphicsApi_OpenGL::WaitIdle()
{

}

void UGraphicsApi_OpenGL::StartFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void UGraphicsApi_OpenGL::EndFrame()
{
	glUseProgram(0);
}

#ifdef DEBUG
void UGraphicsApi_OpenGL::DrawDebugLine(const glm::vec3& origin, const glm::vec3& target, const glm::vec4& color)
{
	DebugDrawLine(origin, target, color);
}
#endif // DEBUG