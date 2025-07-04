/*********************************************************************
 *   File: Shaders.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Graphics/Shaders.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "MemoryManager.hpp"

TMap<std::string, FShaderAsset> UShaders::m_Shaders;

void UShaders::Initialize()
{
    CompileShaders();
}

void UShaders::Register(const FShaderAsset& ShaderAsset)
{
    if (m_Shaders.find(ShaderAsset.Name) == m_Shaders.end())
    {
        m_Shaders[ShaderAsset.Name] = ShaderAsset;
    }
}

void UShaders::CompileShaders()
{
    for (auto& shaderPair : m_Shaders)
    {
        const std::string& shaderName = shaderPair.first;
        FShaderAsset& shaderAsset = shaderPair.second;

        // Create an OpenGL shader component for compilation
        UShaderOpenGLComponent* shaderComponent = FMemoryManager::Allocate<UShaderOpenGLComponent>();
        shaderComponent->SetShaderName(shaderName);
        shaderComponent->SetVertShaderFile(shaderAsset.VertexShaderPath);
        shaderComponent->SetFragShaderFile(shaderAsset.FragmentShaderPath);
           
        // Initialize and compile the shader
        shaderComponent->Initialize();
        shaderComponent->Bind();

        // Optionally, log the success or failure of compilation
        std::cout << "Compiled and bound shader: " << shaderName << std::endl;

        // Unbind after compilation
        shaderComponent->Unbind();

        shaderAsset.ShaderComponent = shaderComponent;
    }
}

UShaderOpenGLComponent* UShaders::GetShader(const std::string& shaderName)
{
    for (const auto& shaderPair : m_Shaders)
    {
        if (shaderPair.first == shaderName)
        {
            return shaderPair.second.ShaderComponent;
        }
    }

    return nullptr;
}

bool UShaders::Unbind(const std::string& shaderName)
{
    for (const auto& shaderPair : m_Shaders)
    {
        if (shaderPair.first == shaderName)
        {
            if (shaderPair.second.ShaderComponent)
                shaderPair.second.ShaderComponent->Unbind();

            return true;
        }
    }

    return false;
}
