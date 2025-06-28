/*********************************************************************
 *   File: SoundManager.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_AUDIO_SOUNDMANAGER_HPP
#define K_AUDIO_SOUNDMANAGER_HPP

#include <fmod.hpp>
#include <string>
#include <unordered_map>

enum class ESoundCategory
{
    Music,
    SFX,
    Ambient,
    Voice,
    UI
};

struct SoundData
{
    std::vector<uint8_t> Buffer;
    FMOD::Sound* Sound = nullptr;
};

class USoundManager
{
public:
    static USoundManager& Get();

    USoundManager();
    virtual ~USoundManager();

    USoundManager(const USoundManager&) = delete;
    USoundManager& operator=(const USoundManager&) = delete;
    USoundManager(USoundManager&&) = delete;
    USoundManager& operator=(USoundManager&&) = delete;

    bool Initialize();
    void Shutdown();

    bool LoadSound(const std::string& name, const std::string& filepath, bool is3D = false, bool loop = false, bool stream = false);
    void PlaySound(const std::string& name, ESoundCategory category, float volume = 1.0f, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f);
    void PlayUISound(const std::string& name, float volume);
    void PauseAll();
    void PauseSoundByName(const std::string& soundName, bool pause);
    void ResumeAll();
    void StopAll();
    void SetVolumeAll(float volume);
    void SetListenerPosition(float x, float y, float z,
        float forwardX = 0.0f, float forwardY = 0.0f, float forwardZ = 1.0f,
        float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

    void Update();

    void SetVolume(ESoundCategory soundGroup, float volume);

    FMOD::System* GetSystem();
    FMOD::Sound* GetSound(const std::string& name);
private:
    FMOD::System* m_System = nullptr;
    std::vector<FMOD::Channel*> m_ActiveChannels;
    TMap<std::string, SoundData> m_SoundData;

    FMOD::Channel* m_ChannelUI = nullptr;

    FMOD::ChannelGroup* m_MasterGroup = nullptr;
    FMOD::ChannelGroup* m_UIGroup = nullptr;
    FMOD::ChannelGroup* m_MusicGroup = nullptr;
    FMOD::ChannelGroup* m_SFXGroup = nullptr;
    FMOD::ChannelGroup* m_AmbientGroup = nullptr;
    FMOD::ChannelGroup* m_VoiceGroup = nullptr;

    bool bIsInitialzied = false;
};

#endif // K_AUDIO_SOUNDMANAGER_HPP

