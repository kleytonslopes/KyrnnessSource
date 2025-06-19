#pragma once
#ifndef KYRNESS_UI_UIELEMENT_HPP
#define KYRNESS_UI_UIELEMENT_HPP

#include "Core/Core.hpp"

struct FShaderOpenGLComponent;

enum class EAnchor
{

	/// <summary>
	/// <para>*----</para>
	/// <para>-----</para>
	/// <para>-----</para>
	/// </summary>
	TopLeft,

	/// <summary>
	/// <para>--*--</para>
	/// <para>-----</para>
	/// <para>-----</para>
	/// </summary>
	TopCenter,

	/// <summary>
	/// <para>----*</para>
	/// <para>-----</para>
	/// <para>-----</para>
	/// </summary>
	TopRight,

	/// <summary>
	/// <para>-----</para>
	/// <para>*----</para>
	/// <para>-----</para>
	/// </summary>
	CenterLeft,

	/// <summary>
	/// <para>-----</para>
	/// <para>--*--</para>
	/// <para>-----</para>
	/// </summary>
	Center,

	/// <summary>
	/// <para>-----</para>
	/// <para>----*</para>
	/// <para>-----</para>
	/// </summary>
	CenterRight,

	/// <summary>
	/// <para>-----</para>
	/// <para>-----</para>
	/// <para>*----</para>
	/// </summary>
	BottomLeft,

	/// <summary>
	/// <para>-----</para>
	/// <para>-----</para>
	/// <para>--*--</para>
	/// </summary>
	BottomCenter,

	/// <summary>
	/// <para>-----</para>
	/// <para>-----</para>
	/// <para>----*</para>
	/// </summary>
	BottomRight
};

class UUIElement 
{
protected:
	enum class EMouseFocusState
	{
		MFS_None,
		MFS_MouseEnter,
		MFS_MouseLeave
	};

	EMouseFocusState m_MouseFocusState = EMouseFocusState::MFS_None;
public:
    float x, y, width, height, scale;

	EAnchor Anchor = EAnchor::TopLeft;
	float OffsetX = 0.0f;
	float OffsetY = 0.0f;

	UUIElement();
    virtual ~UUIElement() = default;

    virtual void Draw();
	virtual void UpdateLayout(); 
    virtual void HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp) = 0;
    virtual void OnMouseEnter(double mouseX, double mouseY) = 0;
    virtual void OnMouseLeave(double mouseX, double mouseY) = 0;
	virtual void OnUpdateMouseFocus(double mouseX, double mouseY) = 0;
	
	void SetTextureId(unsigned int value) { m_TextureID = value; }
	void SetVAO(unsigned int value) { m_VAO = value; }

protected:
	unsigned int m_TextureID;
	unsigned int m_VAO;
	unsigned int m_VBO;

	unsigned int m_VAO_Debug;
	unsigned int m_VBO_Debug;

	FShaderOpenGLComponent* m_Shader = nullptr;
#if(DEBUG)
	FShaderOpenGLComponent* m_ShaderDebug = nullptr;
#endif

	glm::mat4 GetProjetion();
	glm::mat4 GetModel();

	//void DrawDebugLine(float xPosA, float xPosB, float yPosA, float yPosB);
};

#endif// KYRNESS_UI_UIELEMENT_HPP
