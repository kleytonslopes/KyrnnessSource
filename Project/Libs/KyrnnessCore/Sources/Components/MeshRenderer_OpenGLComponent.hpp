/*********************************************************************
 *   File: MeshRenderer_OpenGLComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_MESH_RENDERER_OPENGL_COMPONENT_HPP
#define K_MESH_RENDERER_OPENGL_COMPONENT_HPP

#include "Components/Component.hpp"
//#include "Core/Vertex.hpp"
#include "Content/MeshAsset.hpp"
#include "Graphics/RenderParameters.hpp"

class UShaderOpenGLComponent;

class UMeshRenderer_OpenGLComponent : public UComponent
{
	using Super = UComponent;
public:
	UMeshRenderer_OpenGLComponent(FMeshAsset& meshAsset);
	virtual ~UMeshRenderer_OpenGLComponent() = default;

	UMeshRenderer_OpenGLComponent(const UMeshRenderer_OpenGLComponent&) = delete;
	UMeshRenderer_OpenGLComponent& operator=(const UMeshRenderer_OpenGLComponent&) = delete;
	UMeshRenderer_OpenGLComponent(UMeshRenderer_OpenGLComponent&&) = delete;
	UMeshRenderer_OpenGLComponent& operator=(UMeshRenderer_OpenGLComponent&&) = delete;

	void Render(const TRenderParameters& renderParameters);
	void Bind();

	virtual nlohmann::json GetJsonData() override;

protected:
	std::string m_ShaderName;

	void OnInitialize() override;

private:
	FMeshAsset& m_MeshAsset;
	
	//UShaderOpenGLComponent* m_Shader = nullptr;

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

#endif// K_MESH_RENDERER_OPENGL_COMPONENT_HPP
