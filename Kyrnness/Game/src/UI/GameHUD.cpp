#include "pch.hpp"
#include "UI/GameHUD.hpp"
#include "Runtime/Application.hpp"
#include "UI/UIManager.hpp"
#include "UI/Elements/UIButton.hpp"
#include "Core/AssetManager.hpp"
#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
#include "Audio/SoundManager.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/AudioSourceComponent.hpp"

UGameHUD::UGameHUD(UApplication* application) : UHUD(application)
{
}

void UGameHUD::Initialize()
{
	UHUD::Initialize();

	UUIButton* ButtonT = FMemoryManager::Allocate<UUIButton>();
	UUIButton* Button = FMemoryManager::Allocate<UUIButton>();
	UUIButton* Button2 = FMemoryManager::Allocate<UUIButton>();

	FMemoryManager::Deallocate(ButtonT);
	if (Button)
	{
		Button->x = 100.f;
		Button->y = 100.f;
		Button->width = 162.f;
		Button->height = 52.f;

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_button_02_normal_r.png");
		Button->SetTextureId(textureId);

		Button->OnClick = [Button]() {
			auto objectsScene = UApplication::Get().GetRegistry().view<FTransformComponent, FAudioSourceComponent>();

			objectsScene.each([&](const auto entity, auto& transform, FAudioSourceComponent& audioComp)
				{
					audioComp.PlaySoundByName("music");
				});
			};

		Button->Initialize();
		m_UIManager->AddElement(Button);
	}

	if (Button2)
	{
		Button2->x = 300.f;
		Button2->y = 100.f;
		Button2->width = 162.f;
		Button2->height = 52.f;

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_button_02_normal_r.png");
		Button2->SetTextureId(textureId);

		Button2->OnClick = [Button2]() {
			auto objectsScene = UApplication::Get().GetRegistry().view<FTransformComponent, FAudioSourceComponent>();

			objectsScene.each([&](const auto entity, auto& transform, FAudioSourceComponent& audioComp)
				{
					audioComp.FadeOutSoundByName("music", 3);
				});
			};

		Button2->Initialize();
		m_UIManager->AddElement(Button2);
	}

	auto allButtons = FMemoryManager::GetAll<UUIButton>();
	for (UUIButton* btn : allButtons)
	{
		LOG(Log, TEXT("Button at: %p", btn));
	}
}

void UGameHUD::Draw(float deltaTime)
{
	UGraphicsApi_OpenGL* api = m_Application->GetGraphicsApi<UGraphicsApi_OpenGL>();

	UHUD::Draw(deltaTime);


	glm::vec4 colorX = { 1,0,0,1 };
	glm::vec4 colorY = { 0,1,0,1 };

	glm::vec3 fromX{ 1, 1.f, 0 };
	glm::vec3 fromX2{ 100, 1.f, 0 };

	glm::vec3 fromY{ 1, 1, 0 };
	glm::vec3 fromY2{ 1, 100.f, 0 };

	api->DebugDrawLine2D(fromX, fromX2, colorX);
	api->DebugDrawLine2D(fromY, fromY2, colorY);
}

