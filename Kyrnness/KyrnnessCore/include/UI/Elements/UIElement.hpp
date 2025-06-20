#pragma once
#ifndef KYRNESS_UI_UIELEMENT_HPP
#define KYRNESS_UI_UIELEMENT_HPP

#include "Core/Core.hpp"

struct FShaderOpenGLComponent;

struct FMargin
{
	float Left = 0.0f;
	float Top = 0.0f;
	float Right = 0.0f;
	float Bottom = 0.0f;
};

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
	BottomRight,

	/// <summary>
    /// <para>*****</para>
    /// <para>*---*</para>
    /// <para>*****</para>
    /// </summary>
	Stretch
};

enum class EScaleMode
{
	None,
	ScaleToFitX,
	ScaleToFitY,
	ScaleToFitBoth
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
	UUIElement* Parent = nullptr;
	std::vector<UUIElement*> Children;

    //float scale;

	float x = 0.0f;
	float y = 0.0f;
	float width = 100.0f;
	float height = 100.0f;

	// Salva o tamanho/layout original (pré-scale)
	float BaseX = 0.0f;
	float BaseY = 0.0f;
	float BaseWidth = 100.0f;
	float BaseHeight = 100.0f;

	float LocalX = 0.0f;
	float LocalY = 0.0f;

	EAnchor Anchor = EAnchor::TopLeft;
	float OffsetX = 0.0f;
	float OffsetY = 0.0f;
	FMargin Margin;

	UUIElement();
    virtual ~UUIElement() = default;

	void AddChild(UUIElement* child);

	virtual void Initialize();
    virtual void Draw();
	virtual void UpdateLayout(); 
	virtual void HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp) {}
	virtual void OnMouseEnter(double mouseX, double mouseY) { }
	virtual void OnMouseLeave(double mouseX, double mouseY) { }
	virtual void OnUpdateMouseFocus(double mouseX, double mouseY) { }

	void PropagateInput(float mx, float my, bool isMouseDown, bool isMouseUp);
	void PropagateMouseEnter(double mouseX, double mouseY);
	void PropagateMouseLeave(double mouseX, double mouseY);
	void PropagateUpdateMouseFocus(double mouseX, double mouseY);

	
	void SetTextureId(unsigned int value) { m_TextureID = value; }
	void SetVAO(unsigned int value) { m_VAO = value; }

	virtual glm::mat4 GetWorldModel();

	glm::vec2 GetAccumulatedScale();

protected:

	std::vector<float> m_Vertices = {
		// Posições (x, y)   // UVs (u, v)
	 -0.5f, -0.5f,        0.0f, 0.0f,
	  0.5f, -0.5f,        1.0f, 0.0f,
	 -0.5f,  0.5f,        0.0f, 1.0f,
	  0.5f,  0.5f,        1.0f, 1.0f
	};

//	float vertices[] = {
//	// Posições (x, y)   // UVs (u, v)
// -0.5f, -0.5f,        0.0f, 0.0f,
//  0.5f, -0.5f,        1.0f, 0.0f,
// -0.5f,  0.5f,        0.0f, 1.0f,
//  0.5f,  0.5f,        1.0f, 1.0f
//};

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

	virtual void DrawSelf();

	//void DrawDebugLine(float xPosA, float xPosB, float yPosA, float yPosB);
};

#endif// KYRNESS_UI_UIELEMENT_HPP
