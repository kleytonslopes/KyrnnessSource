#include "GamePCH.hpp"
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
#include "nlohmann/json.hpp"

UGameHUD::UGameHUD(UApplication* application) : UHUD(application)
{
}

#define TEM

void UGameHUD::Initialize()
{
	UHUD::Initialize();

#ifndef TEMP


	nlohmann::json uiJson = UAssetManager::LoadJson("Content/UI/UI_MainMenu.json");

	UUIElement* rootElement = UApplication::Get().GetUIManager()->CreateElementFromJson(uiJson);
	UApplication::Get().GetUIManager()->AddElement(rootElement);

#else


	//UUIScaleBox* ScaleBox = FMemoryManager::Allocate<UUIScaleBox>();
	UUIBorder* Root = FMemoryManager::Allocate<UUIBorder>();
	UUIBorder* Border = FMemoryManager::Allocate<UUIBorder>();
	UUIButton* NewGameButton = FMemoryManager::Allocate<UUIButton>();
	UUIButton* HostGameButton = FMemoryManager::Allocate<UUIButton>();
	UUIButton* JoinGameButton = FMemoryManager::Allocate<UUIButton>();
	UUIButton* Button2 = FMemoryManager::Allocate<UUIButton>();

	UUIFont* myFont = new UUIFont("Assets/Fonts/Roboto-Regular.ttf", 18);
	UUIText* myText = FMemoryManager::Allocate<UUIText>();

	if (myText && myFont)
	{
		myText->LocalX = 0.f;
		myText->LocalY = 0.f;
		myText->width = 1.f;
		myText->height = 1.f;
		myText->Anchor = EAnchor::Center;

		myText->SetFont(myFont);
		myText->SetText("Hello World!");
		myText->SetColor(glm::vec4(1, 1, 1, 1));
		myText->Initialize();

		//m_UIManager->AddElement(myText);
	}

	if (NewGameButton)
	{
		NewGameButton->LocalX = -615.f;
		NewGameButton->LocalY = -260.f;
		NewGameButton->width = 330;
		NewGameButton->height = 52.f;
		NewGameButton->Anchor = EAnchor::Center;

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_button_03_normal.png");
		NewGameButton->SetTextureId(textureId);

		NewGameButton->OnClick = [NewGameButton]() {
			auto objectsScene = UApplication::Get().GetRegistry().view<UTransformComponent, UAudioSourceComponent>();

			objectsScene.each([&](const auto entity, auto& transform, UAudioSourceComponent& audioComp)
				{
					audioComp.PlaySoundByName("music");
				});
			};

		NewGameButton->Initialize();
		//m_UIManager->AddElement(NewGameButton);
	}

	if (HostGameButton)
	{
		HostGameButton->LocalX = -615.f;
		HostGameButton->LocalY = -260.f;
		HostGameButton->width = 330;
		HostGameButton->height = 52.f;
		HostGameButton->Anchor = EAnchor::Center;

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_button_03_normal.png");
		HostGameButton->SetTextureId(textureId);

		HostGameButton->OnClick = [HostGameButton]() {
			auto objectsScene = UApplication::Get().GetRegistry().view<UTransformComponent, UAudioSourceComponent>();

			objectsScene.each([&](const auto entity, auto& transform, UAudioSourceComponent& audioComp)
				{
					audioComp.PlaySoundByName("music");
				});
			};

		HostGameButton->Initialize();
		//m_UIManager->AddElement(HostGameButton);
	}

	if (JoinGameButton)
	{
		JoinGameButton->LocalX = -615.f;
		JoinGameButton->LocalY = -140.f;
		JoinGameButton->width = 330;
		JoinGameButton->height = 52.f;
		JoinGameButton->Anchor = EAnchor::Center;

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_button_03_normal.png");
		JoinGameButton->SetTextureId(textureId);

		JoinGameButton->OnClick = [JoinGameButton]() {
			auto objectsScene = UApplication::Get().GetRegistry().view<UTransformComponent, UAudioSourceComponent>();

			objectsScene.each([&](const auto entity, auto& transform, UAudioSourceComponent& audioComp)
				{
					audioComp.PlaySoundByName("music");
				});
			};

		JoinGameButton->Initialize();
		//m_UIManager->AddElement(JoinGameButton);
	}

	if (Button2)
	{
		Button2->LocalX = 300.f;
		Button2->LocalY = 100.f;
		Button2->width = 162.f;
		Button2->height = 52.f;
		Button2->Anchor = EAnchor::Center;

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_button_02_normal_r.png");
		Button2->SetTextureId(textureId);

		Button2->OnClick = [Button2]() {
			auto objectsScene = UApplication::Get().GetRegistry().view<UTransformComponent, UAudioSourceComponent>();

			objectsScene.each([&](const auto entity, auto& transform, UAudioSourceComponent& audioComp)
				{
					audioComp.FadeOutSoundByName("music", 3);
				});
			};

		Button2->Initialize();
		//m_UIManager->AddElement(Button2);
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

		Border->AddChild(Button2);
		Border->AddChild(NewGameButton);
		//Border->AddChild(myText);

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_ui_menu_panel_bg.png");
		Border->SetTextureId(textureId);

		Border->Initialize();
		//m_UIManager->AddElement(Border);
	}
	if (Root)
	{
		Root->x = 0;
		Root->y = 0.f;
		Root->width = 2560;
		Root->height = 1080;
		Root->Anchor = EAnchor::Center;
		Root->OffsetX = 0.0f;
		Root->OffsetY = 0.0f;

		Root->AddChild(Border);
		//Border->AddChild(myText);

		GLuint textureId = UAssetManager::LoadTextureOpenGL("Assets/Textures/UI/tex_bg_mainmenu.png");
		Root->SetTextureId(textureId);

		Root->Initialize();
		m_UIManager->AddElement(Root);
	}
	

	auto allButtons = FMemoryManager::GetAll<UUIButton>();
	for (UUIButton* btn : allButtons)
	{
		FLogger::Log(TEXT("Button at: %p", btn));
	}
#endif // TEMP	


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

