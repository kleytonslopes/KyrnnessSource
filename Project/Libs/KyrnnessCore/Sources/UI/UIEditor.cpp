/*********************************************************************
 *   File: UIEditor.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "UI/UIEditor.hpp"
#include "Runtime/Application.hpp"
#include "Runtime/WindowGFLW.hpp"
#include "GameFramework/Scene.hpp"

#include "GLFW/glfw3.h" 

#include "UI/UIManager.hpp"
#include "UI/UIFont.hpp"
#include "UI/Elements/UIButton.hpp"
#include "Runtime/Application.hpp"
#include "Runtime/Window.hpp"
#include "Input/InputManager.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include <entt/entt.hpp>
#include <imgui.h>

//UUIEditor::UUIEditor(UApplication* application) : UHUD(application)
//{
//
//}

//void UUIEditor::Initialize()
//{
//	UHUD::Initialize();
//
//	//UUIFont* font = FMemoryManager::Allocate<UUIFont>();
//
//	//font->Load("Assets/Fonts/atlas.png", "Assets/Fonts/atlas.csv");
//	//
//	//UUIButton* button = FMemoryManager::Allocate<UUIButton>();
//	//button->text = "Close";
//	//button->x = 50.0f;
//	//button->y = 50.0f;
//	//button->width = 128.0f;
//	//button->height = 45.0f;
//	//button->scale = 1.f;
//	//button->SetFont(font);
//	//button->OnClick = [button]() { button->text = "Ok"; };
//	//button->OnHovered = [button](bool b) {
//	//	if(b)
//	//		button->text = ":)";
//	//	else
//	//		button->text = "T.T";
//
//	//	};
//
//	//m_UIManager->AddElement(button);
//}

void UUIEditor::Draw(float deltaTime)
{
  //  ImGui::Begin("Scene");
  //  if (ImGui::Button("Save"))
  //  {
		//m_Application->GetScene()->SaveScene();
  //      //auto it = std::find(m_Application->GetScene()->GetObjects().begin(), m_Application->GetScene()->GetObjects().end(), m_Application->GetScene()->m_SelectedObject);
  //      //if (it != m_Application->GetScene()->GetObjects().end()) {
  //      //    delete* it;
  //      //    m_Application->GetScene()->GetObjects().erase(it);
  //      //    m_Application->GetScene()->m_SelectedObject = nullptr;
  //      //}
  //  }
  //  ImGui::End();


  //  ImGui::Begin("Editor de Level");

  //  // Lista de objetos na cena
  //  if (ImGui::CollapsingHeader("Objetos na Cena"))
  //  {
  //      for (size_t i = 0; i < m_Application->GetScene()->GetObjects().size(); i++)
  //      {
  //          bool isSelected = (m_Application->GetScene()->GetObjects()[i] == m_Application->GetScene()->GetSelectedObject());
  //          if (ImGui::Selectable(m_Application->GetScene()->GetObjects()[i]->m_ObjectName.c_str(), isSelected))
  //          {
  //              m_Application->GetScene()->m_SelectedObject = m_Application->GetScene()->GetObjects()[i];
  //              // Desselecionar outros
  //              for (auto& obj : m_Application->GetScene()->GetObjects()) {
  //                  obj->bIsSelected = (obj == m_Application->GetScene()->m_SelectedObject);
  //              }
  //          }
  //      }
  //  }

  //  // Propriedades do objeto selecionado
  //  if (ImGui::CollapsingHeader("Propriedades") && m_Application->GetScene()->m_SelectedObject)
  //  {
		//glm::vec3 position = m_Application->GetScene()->m_SelectedObject->m_Position;
		//glm::vec3 rotation = m_Application->GetScene()->m_SelectedObject->m_Rotation;

  //      ImGui::Text("Nome: %s", m_Application->GetScene()->m_SelectedObject->m_ObjectName.c_str());
  //      
  //      ImGui::DragFloat3("Location", &position[0], 0.1f);
  //      ImGui::DragFloat3("Rotation", &rotation[0], 0.1f);
  //      
  //      ImGui::DragFloat3("Escala", &m_Application->GetScene()->m_SelectedObject->m_Scale[0], 0.05f);
  //      //ImGui::ColorEdit3("Cor", &m_Application->GetScene()->m_SelectedObject->color[0]);

  //      m_Application->GetScene()->m_SelectedObject->SetPosition(position);
  //      m_Application->GetScene()->m_SelectedObject->SetRotation(rotation);

  //      if (ImGui::Button("Remover Objeto"))
  //      {
  //          auto it = std::find(m_Application->GetScene()->GetObjects().begin(), m_Application->GetScene()->GetObjects().end(), m_Application->GetScene()->m_SelectedObject);
  //          if (it != m_Application->GetScene()->GetObjects().end()) {
  //              m_Application->GetRegistry().destroy(m_Application->GetScene()->m_SelectedObject->m_Entity);

  //              delete* it;
  //              m_Application->GetScene()->GetObjects().erase(it);
  //              m_Application->GetScene()->m_SelectedObject = nullptr;
  //          }
  //      }

  //      if (ImGui::Button("Duplicate Objeto"))
  //      {
  //          if (m_Application->GetScene()->m_SelectedObject)
  //          {
  //              m_Application->GetScene()->DuplicateObject(m_Application->GetScene()->m_SelectedObject);
  //          }
  //          /*auto it = std::find(m_Application->GetScene()->GetObjects().begin(), m_Application->GetScene()->GetObjects().end(), m_Application->GetScene()->m_SelectedObject);
  //          if (it != m_Application->GetScene()->GetObjects().end()) {
  //              m_Application->GetRegistry().destroy(m_Application->GetScene()->m_SelectedObject->m_Entity);

  //              delete* it;
  //              m_Application->GetScene()->GetObjects().erase(it);
  //              m_Application->GetScene()->m_SelectedObject = nullptr;
  //          }*/
  //      }
  //  }

  //  //// Adicionar novos objetos
  //  //if (ImGui::Button("Adicionar Cubo"))
  //  //{
  //  //    GameObject* newObj = new GameObject();
  //  //    newObj->name = "Cubo " + std::to_string(gameObjects.size());
  //  //    newObj->mesh = GetCubeMesh();
  //  //    gameObjects.push_back(newObj);
  //  //}

  //  ImGui::End();
}
