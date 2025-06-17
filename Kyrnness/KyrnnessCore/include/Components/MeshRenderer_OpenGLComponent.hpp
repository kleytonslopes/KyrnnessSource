#pragma once

#ifndef KYRNNESS_MESH_RENDERER_OPENGL_COMPONENT_HPP
#define KYRNNESS_MESH_RENDERER_OPENGL_COMPONENT_HPP

#include "Components/Component.hpp"
//#include "Core/Vertex.hpp"
#include "Content/MeshAsset.hpp"
#include "Graphics/RenderParameters.hpp"

class FShaderOpenGLComponent;

class FMeshRenderer_OpenGLComponent : public FComponent
{
public:
	FMeshRenderer_OpenGLComponent(FMeshAsset& meshAsset,/*std::vector<Vertex> vertices, std::vector<uint32> indices,*/ FShaderOpenGLComponent* shaderComponent);
	virtual ~FMeshRenderer_OpenGLComponent() = default;

	FMeshRenderer_OpenGLComponent(const FMeshRenderer_OpenGLComponent&) = delete;
	FMeshRenderer_OpenGLComponent& operator=(const FMeshRenderer_OpenGLComponent&) = delete;
	FMeshRenderer_OpenGLComponent(FMeshRenderer_OpenGLComponent&&) = delete;
	FMeshRenderer_OpenGLComponent& operator=(FMeshRenderer_OpenGLComponent&&) = delete;

	void Initialize();
	void Render(const TRenderParameters& renderParameters);
	void Destroy();
	void Bind();

	virtual nlohmann::json GetJsonData() override;

private:
	FMeshAsset& m_MeshAsset;

	FShaderOpenGLComponent* m_Shader = nullptr;

	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_EBO;
	uint32 m_FBO;

	uint32 m_FboTexture;

	uint32 m_TextureID;

	void CreateVAO();
	void CreateVBO();
	void CreateEBO();
	void CreateFBO();

	void CreateTexture();
};

#endif// KYRNNESS_MESH_RENDERER_OPENGL_COMPONENT_HPP
