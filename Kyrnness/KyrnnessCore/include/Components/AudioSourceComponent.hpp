#pragma once
#ifndef KYRNNESS_AUDIO_SOURCE_COMPONENT_HPP
#define KYRNNESS_AUDIO_SOURCE_COMPONENT_HPP

#include "Components/Component.hpp"
#include <fmod.hpp>

enum class EFadeType
{
	FadeIn,
	FadeOut
};

struct FActiveFade
{
	FMOD::Channel* Channel = nullptr;
	float Duration = 0.0f;
	float Elapsed = 0.0f;
	float StartVolume = 0.0f;
	float TargetVolume = 1.0f;
	EFadeType Type = EFadeType::FadeIn;
};

class FAudioSourceComponent : public UComponent
{
public:
	FAudioSourceComponent() = default;

	FAudioSourceComponent(const FAudioSourceComponent&) = delete;
	FAudioSourceComponent& operator=(const FAudioSourceComponent&) = delete;
	FAudioSourceComponent(FAudioSourceComponent&&) = delete;
	FAudioSourceComponent& operator=(FAudioSourceComponent&&) = delete;

	virtual void Update(float deltaTime) override;

	void Play();
	void PlaySoundByName(const std::string& soundName, float volume = 1.0f);
	void Stop();
	void StopAll();
	void StopSoundByName(const std::string& soundName);
	void Pause(bool pause);
	void PauseAll(bool pause);
	void PauseSoundByName(const std::string& soundName, bool pause);
	void SetLoop(bool loop);
	void SetVolume(float volume);
	void SetPosition(const glm::vec3& position);
	void SetSoundName(const std::string& soundName);
	void SetVolumeBySoundName(const std::string& soundName, float volume);
	bool IsSoundPlayingByName(const std::string& soundName) const;
	void FadeInSoundByName(const std::string& soundName, float fadeTime, float targetVolume);
	void FadeOutSoundByName(const std::string& soundName, float fadeTime);
	void CrossFade(const std::string& fromSound, const std::string& toSound, float fadeTime, float targetVolume);

	virtual nlohmann::json GetJsonData() override;

private:
	std::string m_SoundName;
	bool bLoop = false;
	FMOD::Channel* m_Channel = nullptr;
	std::vector<FMOD::Channel*> m_ActiveChannels;
	float m_Volume = 1.0f;
	glm::vec3 m_Position;
	std::vector<FActiveFade> m_ActiveFades;
};

#endif // KYRNNESS_AUDIO_SOURCE_COMPONENT_HPP