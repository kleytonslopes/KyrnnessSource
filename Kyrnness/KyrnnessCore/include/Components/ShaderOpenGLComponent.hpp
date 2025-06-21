#pragma once
#ifndef KYRNNESS_SHADER_OPENGL_HPP
#define KYRNNESS_SHADER_OPENGL_HPP

#include "Components/Component.hpp"

class UShaderOpenGLComponent : public UComponent
{
	using Super = UComponent;
public:
	UShaderOpenGLComponent(const std::string& shaderName, const char* vertShaderFile, const char* fragShaderFile);
	virtual ~UShaderOpenGLComponent() = default;

	UShaderOpenGLComponent(const UShaderOpenGLComponent&) = delete;
	UShaderOpenGLComponent& operator=(const UShaderOpenGLComponent&) = delete;
	UShaderOpenGLComponent(UShaderOpenGLComponent&&) = delete;
	UShaderOpenGLComponent& operator=(UShaderOpenGLComponent&&) = delete;

	
	virtual void Bind();
	virtual void Unbind();

	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVector2(const std::string& name, glm::vec2 value);
	void SetVector2(const std::string& name, float x, float y);
	void SetVector3(const std::string& name, glm::vec3 value);
	void SetVector3(const std::string& name, float x, float y, float z);
	void SetVector4(const std::string& name, glm::vec4 value);
	void SetVector4(const std::string& name, float x, float y, float z, float w);

	void SetMatrix2(const std::string& name, glm::mat2 value);
	void SetMatrix3(const std::string& name, glm::mat3 value);
	void SetMatrix4(const std::string& name, glm::mat4 value);

	std::string GetShaderName() const { return m_ShaderName; }

	uint32 GetProgramId() const { return m_ShaderProgramId; }

	virtual nlohmann::json GetJsonData() override;

private:
	std::string m_ShaderName;
	const char* m_VertShaderFile;
	const char* m_FragShaderFile;

	uint32 m_ShaderProgramId = 0;
	uint32 m_VertexShaderId = 0;
	uint32 m_FragmentShaderId = 0;

	void OnInitialize() override;
	void OnDestroy() override;

	void LoadShaderFiles();
	void CreateProgram();

	void CheckCompileErrors(uint32_t shader, const std::string& type);
};

#endif // !KYRNNESS_SHADER_OPENGL_HPP
