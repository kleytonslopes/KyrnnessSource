#include "pch.hpp"
#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
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
	LOG(Log, "Initializing OpenGL Graphics API...");

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


	///glViewport(0, 0, w, h);
	///glEnable(GL_MULTISAMPLE);
	///glEnable(GL_DEPTH_TEST);
	///glDepthFunc(GL_LESS);
	///glEnable(GL_CULL_FACE);
	///glCullFace(GL_BACK);
	///glEnable(GL_STENCIL_TEST);
	///glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	///glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	///glEnable(GL_BLEND);
	///glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	///glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	/// 
	/// 
//	
//// build and compile our shader program
//	// ------------------------------------
//	// vertex shader
//	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//	glCompileShader(vertexShader);
//	// check for shader compile errors
//	int success;
//	char infoLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//	// fragment shader
//	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(fragmentShader);
//	// check for shader compile errors
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//	// link shaders
//	shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//	glLinkProgram(shaderProgram);
//	// check for linking errors
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	// ------------------------------------------------------------------
//	//float vertices[] = {
//	//	 0.5f,  0.5f, 0.0f,  // top right
//	//	 0.5f, -0.5f, 0.0f,  // bottom right
//	//	-0.5f, -0.5f, 0.0f,  // bottom left
//	//	-0.5f,  0.5f, 0.0f   // top left 
//	//};
//
//	std::vector<Vertex> vertices = {
//		{ { 0.5f,  0.5f, 0.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f} }, // bottom left
//		{ { 0.5f, -0.5f, 0.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f} }, // bottom right
//		{ {-0.5f, -0.5f, 0.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f} }, // top right
//		{ {-0.5f,  0.5f, 0.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f} }  // top left 
//	};
//
//	std::vector<uint32> indices = {
//		0, 1, 3,  // first Triangle
//		1, 2, 3   // second Triangle
//	};
//	
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
//	glEnableVertexAttribArray(0);
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);

}

void UGraphicsApi_OpenGL::DrawFrame(float deltaTime)
{
	StartFrame();

	DrawDebugLine(FVector{ 0.f }, FVector{ 100, 100, 100 }, FVector4{ 1.f, 0.f, 1.f, 1.f });

}

void UGraphicsApi_OpenGL::DebugDrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color)
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

	auto entities = m_EnttRegistry.view<FTransformComponent, FCameraComponent>();

	glm::vec3 location{ 0.f, 0.f, 5.f };
	glm::vec3 forwardVector{ 1.f, 0.f, 0.f };
	glm::vec3 upVector{ 0.f, 0.f, 1.f };
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
	glm::vec3 loc{ 0.0f, 0.0f, 0.0f };
	glm::vec3 sca{ 1.0f, 1.0f, 1.0f };

	modelMatrix = glm::translate(modelMatrix, loc);
	modelMatrix = glm::scale(modelMatrix, sca);

	auto shaderEntities = m_EnttRegistry.view<FShaderOpenGLComponent>();
	shaderEntities.each([&](const auto entity, auto& shader)
		{
			if (shader.GetShaderName() == "debugShader")
			{
				shader.Bind();
				shader.SetMatrix4("view", view);
				shader.SetMatrix4("projection", project);
				shader.SetMatrix4("model", modelMatrix);
			}

		});


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

	shaderEntities.each([&](const auto entity, auto& shader)
		{
			if (shader.GetShaderName() != "debugShader")
			{
				shader.Unbind();
			}

		});
}


void UGraphicsApi_OpenGL::WaitIdle()
{

}

void UGraphicsApi_OpenGL::StartFrame()
{
	glDisableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void UGraphicsApi_OpenGL::EndFrame()
{
	glUseProgram(0);
}

void UGraphicsApi_OpenGL::DrawDebugLine(const glm::vec3& origin, const glm::vec3& target, const glm::vec4& color)
{
	DebugDrawLine(origin, target, color);
}
