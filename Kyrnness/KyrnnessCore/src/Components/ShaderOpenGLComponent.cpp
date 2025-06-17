#include "pch.hpp"
#include "Components/ShaderOpenGLComponent.hpp"

FShaderOpenGLComponent::FShaderOpenGLComponent(const std::string& shaderName, const char* vertShaderFile, const char* fragShaderFile)
	: m_ShaderName(shaderName), m_VertShaderFile(vertShaderFile), m_FragShaderFile(fragShaderFile)
{
}

void FShaderOpenGLComponent::Initialize()
{
	LoadShaderFiles();
	CreateProgram();
}

void FShaderOpenGLComponent::Destroy()
{
	if (m_ShaderProgramId != 0)
	{
		glDeleteProgram(m_ShaderProgramId);
		m_ShaderProgramId = 0;
	}

	if (m_VertexShaderId != 0)
	{
		glDeleteShader(m_VertexShaderId);
		m_VertexShaderId = 0;
	}

	if (m_FragmentShaderId != 0)
	{
		glDeleteShader(m_FragmentShaderId);
		m_FragmentShaderId = 0;
	}
}

void FShaderOpenGLComponent::Bind()
{
	if (m_ShaderProgramId != 0)
	{
		glUseProgram(m_ShaderProgramId);
	}
	else
	{
		ThrowInvalidArgument("Shader program ID is zero, cannot bind shader.");
	}
}

void FShaderOpenGLComponent::Unbind()
{
	if (m_ShaderProgramId != 0)
	{
		glUseProgram(0);
	}
	else
	{
		ThrowInvalidArgument("Shader program ID is zero, cannot unbind shader.");
	}
}

void FShaderOpenGLComponent::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramId, name.c_str()), (int)value);

}

void FShaderOpenGLComponent::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramId, name.c_str()), value);
}

void FShaderOpenGLComponent::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), value);

}

void FShaderOpenGLComponent::SetVector2(const std::string& name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), value.x, value.y);
}

void FShaderOpenGLComponent::SetVector2(const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), x, y);
}

void FShaderOpenGLComponent::SetVector3(const std::string& name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), value.x, value.y, value.z);
}

void FShaderOpenGLComponent::SetVector3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), x, y, z);
}

void FShaderOpenGLComponent::SetVector4(const std::string& name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), value.x, value.y, value.z, value.w);
}

void FShaderOpenGLComponent::SetVector4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(m_ShaderProgramId, name.c_str()), x, y, z, w);
}

void FShaderOpenGLComponent::SetMatrix2(const std::string& name, glm::mat2 value)
{
	glUniformMatrix2fv(glGetUniformLocation(m_ShaderProgramId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void FShaderOpenGLComponent::SetMatrix3(const std::string& name, glm::mat3 value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_ShaderProgramId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void FShaderOpenGLComponent::SetMatrix4(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

nlohmann::json FShaderOpenGLComponent::GetJsonData()
{
	return nlohmann::json();
}

void FShaderOpenGLComponent::LoadShaderFiles()
{
	std::string vertShaderSource = FFile::ReadFile(m_VertShaderFile);
	std::string fragShaderSource = FFile::ReadFile(m_FragShaderFile);

	const char* vertShaderCode = vertShaderSource.c_str();
	const char* fragShaderCode = fragShaderSource.c_str();

	m_VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShaderId, 1, &vertShaderCode, nullptr);
	glCompileShader(m_VertexShaderId);
	CheckCompileErrors(m_VertexShaderId, "VERTEX");

	m_FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShaderId, 1, &fragShaderCode, nullptr);
	glCompileShader(m_FragmentShaderId);
	CheckCompileErrors(m_FragmentShaderId, "FRAGMENT");

}

void FShaderOpenGLComponent::CreateProgram()
{
	m_ShaderProgramId = glCreateProgram();
	if (m_ShaderProgramId == 0)
	{
		ThrowRuntimeError("Failed to create shader program.");
	}

	glAttachShader(m_ShaderProgramId, m_VertexShaderId);
	glAttachShader(m_ShaderProgramId, m_FragmentShaderId);
	glLinkProgram(m_ShaderProgramId);

	CheckCompileErrors(m_ShaderProgramId, "PROGRAM");

	GLint result = GL_TRUE;
	glGetProgramiv(m_ShaderProgramId, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		ThrowRuntimeError("Failed to Link Program");
	}

	glDetachShader(m_ShaderProgramId, m_VertexShaderId);
	glDetachShader(m_ShaderProgramId, m_FragmentShaderId);

	glDeleteShader(m_VertexShaderId);
	glDeleteShader(m_FragmentShaderId);
}

void FShaderOpenGLComponent::CheckCompileErrors(uint32_t shader, const std::string& type)
{
	GLint result = GL_TRUE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE)
	{
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::vector<char> errorMessage(logLength);
			glGetShaderInfoLog(shader, logLength, nullptr, errorMessage.data());

			std::string errorStr(errorMessage.data());
			ThrowRuntimeError("Shader compilation error in " + type + ": " + errorStr);
		}
	}
}
