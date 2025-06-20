#pragma once
#ifndef KYRNNESS_SHADERS_HPP
#define KYRNNESS_SHADERS_HPP

#include <string>
#include <unordered_map>

#define SHADER_DEFAULT  "default"
#define SHADER_DEBUG    "debugShader"
#define SHADER_UI       "uiShader"
#define SHADER_UI_DEBUG "uiShaderDebug"
#define SHADER_UI_TEXT  "uiShaderText"

class FShaderOpenGLComponent;

struct FShaderAsset
{
public:

	std::string Name;
	std::string VertexShaderPath;
	std::string FragmentShaderPath;

	FShaderOpenGLComponent* ShaderComponent = nullptr;
};
class UShaders
{
public:
	static void Initialize();
	static void Register(const FShaderAsset& ShaderAsset);
	static FShaderOpenGLComponent* GetShader(const std::string& shaderName);
	static bool Unbind(const std::string& shaderName);

private:
	static std::unordered_map<std::string, FShaderAsset> m_Shaders;

	static void CompileShaders();
};

#endif // KYRNNESS_SHADERS_HPP