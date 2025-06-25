#include "CorePCH.hpp"
#include "Components/MeshRenderer_OpenGLComponent.hpp"
#include "Components/ShaderOpenGLComponent.hpp"

UMeshRenderer_OpenGLComponent::UMeshRenderer_OpenGLComponent(FMeshAsset& meshAsset/*std::vector<Vertex> vertices, std::vector<uint32> indices*/, UShaderOpenGLComponent* shaderComponent)
	//: m_Vertices(vertices)
	//, m_Indices(indices)
	: m_MeshAsset(meshAsset)
	, m_Shader(shaderComponent)
{
	Initialize();
}

void UMeshRenderer_OpenGLComponent::OnInitialize()
{
	CreateVAO();
	CreateVBO();
	CreateEBO();
	CreateTexture();

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	glEnableVertexAttribArray(1);

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
	//glEnableVertexAttribArray(1);



	glBindVertexArray(0);

	Super::OnInitialize();

}

void UMeshRenderer_OpenGLComponent::Render(const TRenderParameters& renderParameters)
{
	uint32 diffuseNr = 1;
	uint32 specularNr = 1;
	uint32 normalNr = 1;
	uint32 heightNr = 1;

	glm::mat4 matrix = glm::mat4{ 1.f };

	matrix = glm::translate(matrix, renderParameters.location);
	matrix = glm::rotate(matrix, glm::radians(renderParameters.rotation.x), glm::vec3{ 1.f, 0.f, 0.f });
	matrix = glm::rotate(matrix, glm::radians(renderParameters.rotation.y), glm::vec3{ 0.f, 1.f, 0.f });
	matrix = glm::rotate(matrix, glm::radians(renderParameters.rotation.z), glm::vec3{ 0.f, 0.f, 1.f });
	matrix = glm::scale(matrix, renderParameters.scale);

	////glm::vec3 loc{2.f, 0.f, 5.f};
	///glm::mat4 view = glm::lookAt(
	///							glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	///							glm::vec3(0,0,0), // and looks at the origin
	///							glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
	///					   );//glm::lookAt(loc, location + glm::vec3{1.f, 0.f, 0.f}, glm::vec3{ 0.f, 0.f, 1.f });

	//glm::mat4 projection = glm::perspective(glm::radians(45.f), 1280.f / 720.f, 0.0001f, 10000.0f);

	m_Shader->Bind();
	m_Shader->SetMatrix4("projection", renderParameters.projectionMatrix);
	m_Shader->SetMatrix4("view", renderParameters.viewMatrix);
	m_Shader->SetMatrix4("model", matrix);
	m_Shader->SetVector3("position", renderParameters.location);

	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_MeshAsset.indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void UMeshRenderer_OpenGLComponent::Bind()
{
	glBindVertexArray(m_VAO);
}

nlohmann::json UMeshRenderer_OpenGLComponent::GetJsonData()
{
	return nlohmann::json();
}

void UMeshRenderer_OpenGLComponent::CreateVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
}

void UMeshRenderer_OpenGLComponent::CreateVBO()
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_MeshAsset.vertices.size() * sizeof(Vertex), &m_MeshAsset.vertices[0], GL_STATIC_DRAW);
}

void UMeshRenderer_OpenGLComponent::CreateEBO()
{
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MeshAsset.indices.size() * sizeof(unsigned int), &m_MeshAsset.indices[0], GL_STATIC_DRAW);
}

void UMeshRenderer_OpenGLComponent::CreateFBO()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glGenTextures(1, &m_FboTexture);
		glBindTexture(GL_TEXTURE_2D, m_FboTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTexture, 0);
	}
}

void UMeshRenderer_OpenGLComponent::CreateTexture()
{
	if (m_MeshAsset.images.size() == 0)
	{
		return;
	}
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	GLenum format = GL_RGBA;
	if (m_MeshAsset.images[0].component == 3) format = GL_RGB;
	else if (m_MeshAsset.images[0].component == 1) format = GL_RED;

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_MeshAsset.images[0].width, m_MeshAsset.images[0].height, 0, format, GL_UNSIGNED_BYTE, m_MeshAsset.images[0].image.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


