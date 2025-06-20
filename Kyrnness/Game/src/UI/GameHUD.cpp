#include "pch.hpp"
#include "UI/GameHUD.hpp"
#include "Runtime/Application.hpp"
#include "UI/UIManager.hpp"
#include "UI/Elements/UIButton.hpp"
#include "UI/Elements/UIBorder.hpp"
#include "UI/Elements/UIScaleBox.hpp"
#include "UI/Elements/UUIText.hpp"
#include "UI/UIFont.hpp"
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

	UUIScaleBox* ScaleBox = FMemoryManager::Allocate<UUIScaleBox>();
	UUIBorder* Border = FMemoryManager::Allocate<UUIBorder>();
	UUIButton* Button = FMemoryManager::Allocate<UUIButton>();
	UUIButton* Button2 = FMemoryManager::Allocate<UUIButton>();

	//UUIFont* myFont = new UUIFont("Assets/Fonts/Roboto-Regular.ttf", 32);
	//UUIText* myText = FMemoryManager::Allocate<UUIText>();
	//if (myText && myFont)
	//{
	//	myText->SetFont(myFont);
	//	myText->SetText("Hello World!");
	//	myText->SetColor(glm::vec4(1, 1, 1, 1));
	//}

	if (Button)
	{
		Button->LocalX = 100.f;
		Button->y = 0.f;
		Button->width = 162.f;
		Button->height = 52.f;
		Button->Anchor = EAnchor::TopLeft;

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
		//m_UIManager->AddElement(Button);
	}

	if (Button2)
	{
		Button2->LocalX = 300.f;
		Button2->y = 100.f;
		Button2->width = 162.f;
		Button2->height = 52.f;
		Button2->Anchor = EAnchor::Center;

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
	}

	if (Border)
	{
		Border->x = 0;
		Border->y = 0.f;
		Border->width = 1602.f;
		Border->height = 802.f;
		Border->Anchor = EAnchor::Center;
		Border->OffsetX = 0.0f;
		Border->OffsetY = 0.0f;

		Border->AddChild(Button);
		Border->AddChild(Button2);
		//Border->AddChild(myText);

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_menu_panel_bg.png");
		Border->SetTextureId(textureId);

		Border->Initialize();
		m_UIManager->AddElement(Border);
	}


	if (ScaleBox)
	{
		ScaleBox->x = 0;
		ScaleBox->y = 0.f;
		ScaleBox->width = UApplication::Get().GetWidth();
		ScaleBox->height = UApplication::Get().GetHeight();
		ScaleBox->Anchor = EAnchor::Stretch;
		ScaleBox->OffsetX = 0.0f;
		ScaleBox->OffsetY = 0.0f;
		ScaleBox->ScaleMode = EScaleMode::ScaleToFitBoth;

		ScaleBox->AddChild(Border);

		m_UIManager->AddElement(ScaleBox);
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

#ifdef DEBUG
	api->DebugDrawLine2D(fromX, fromX2, colorX);
	api->DebugDrawLine2D(fromY, fromY2, colorY);
#endif // DEBUG
}

