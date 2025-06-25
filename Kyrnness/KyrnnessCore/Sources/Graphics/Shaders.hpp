/*********************************************************************
 *   File: Shaders.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_SHADERS_HPP
#define K_SHADERS_HPP

#include <string>
#include <unordered_map>

#define SHADER_DEFAULT  "default"
#define SHADER_DEBUG    "debugShader"
#define SHADER_UI       "uiShader"
#define SHADER_UI_DEBUG "uiShaderDebug"
#define SHADER_UI_TEXT  "uiShaderText"

class UShaderOpenGLComponent;

struct FShaderAsset
{
public:

	std::string Name;
	std::string VertexShaderPath;
	std::string FragmentShaderPath;

	UShaderOpenGLComponent* ShaderComponent = nullptr;
};
class UShaders
{
public:
	static void Initialize();
	static void Register(const FShaderAsset& ShaderAsset);
	static UShaderOpenGLComponent* GetShader(const std::string& shaderName);
	static bool Unbind(const std::string& shaderName);

private:
	static TMap<std::string, FShaderAsset> m_Shaders;

	static void CompileShaders();
};

#endif // K_SHADERS_HPP