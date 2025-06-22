#pragma once
#ifndef GAME_PLAYER_COMPONENT_HPP
#define GAME_PLAYER_COMPONENT_HPP

#include "Components/PlayerComponent.hpp"

class FMyPlayerComponent : public UPlayerComponent
{
	using InputAction = TFunction<void(int)>;

public:
	FMyPlayerComponent() = default;
	virtual ~FMyPlayerComponent() = default;

	FMyPlayerComponent(const FMyPlayerComponent&) = delete;
	FMyPlayerComponent& operator=(const FMyPlayerComponent&) = delete;
	FMyPlayerComponent(FMyPlayerComponent&&) = delete;
	FMyPlayerComponent& operator=(FMyPlayerComponent&&) = delete;

	virtual void Initialize() override;
	virtual void SetupPlayerInput() override;
	virtual void Update(float deltaTime);
	virtual nlohmann::json GetJsonData() override;
};

#endif// GAME_PLAYER_COMPONENT_HPP
