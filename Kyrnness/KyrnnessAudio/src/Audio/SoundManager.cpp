#include "pch.hpp"
#include "Audio/SoundManager.hpp"
#include <iostream>

USoundManager& USoundManager::Get()
{
	USoundManager Instance;
	return Instance;
}

USoundManager::USoundManager()
{

}

USoundManager::~USoundManager()
{
}

bool USoundManager::Initialize()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ")" << std::endl;
		return false;
	}

	result = m_System->init(512, FMOD_INIT_3D_RIGHTHANDED, nullptr);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD init error! (" << result << ")" << std::endl;
		return false;
	}

	m_System->getMasterChannelGroup(&m_MasterGroup);

	m_System->createChannelGroup("Music", &m_MusicGroup);
	m_System->createChannelGroup("SFX", &m_SFXGroup);
	m_System->createChannelGroup("Ambient", &m_AmbientGroup);
	m_System->createChannelGroup("Voice", &m_VoiceGroup);
	m_System->createChannelGroup("UI", &m_UIGroup);

	// Anexar os grupos filhos no Master
	m_MasterGroup->addGroup(m_MusicGroup);
	m_MasterGroup->addGroup(m_SFXGroup);
	m_MasterGroup->addGroup(m_AmbientGroup);
	m_MasterGroup->addGroup(m_VoiceGroup);
	m_MasterGroup->addGroup(m_UIGroup);

	return true;
}

void USoundManager::Shutdown()
{
	for (auto& pair : m_Sounds)
	{
		pair.second->release();
	}
	m_Sounds.clear();

	if (m_System)
	{
		m_System->close();
		m_System->release();
		m_System = nullptr;
	}
}

bool USoundManager::LoadSound(const std::string& name, const std::string& filepath, bool is3D, bool loop, bool stream)
{
	if (!m_System)
		return false;

	FMOD_MODE mode = FMOD_DEFAULT;
	if (is3D) mode |= FMOD_3D;
	else mode |= FMOD_2D;

	if (loop) mode |= FMOD_LOOP_NORMAL;
	else mode |= FMOD_LOOP_OFF;

	if (stream) mode |= FMOD_CREATESTREAM;

	FMOD::Sound* sound = nullptr;
	FMOD_RESULT result = m_System->createSound(filepath.c_str(), mode, nullptr, &sound);

	if (result != FMOD_OK)
	{
		std::cout << "Failed to load sound: " << filepath << " Error: " << result << std::endl;
		return false;
	}

	m_Sounds[name] = sound;
	return true;
}

void USoundManager::PlaySound(const std::string& name, ESoundCategory category, float volume, float posX, float posY, float posZ)
{
	if (!m_System)
		return;

	auto it = m_Sounds.find(name);
	if (it == m_Sounds.end())
		return;

	FMOD::Channel* channel = nullptr;
	m_System->playSound(it->second, nullptr, false, &channel);

	if (channel)
	{
		channel->setVolume(volume);

		FMOD_VECTOR pos = { posX, posY, posZ };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		channel->set3DAttributes(&pos, &vel);

		// Direcionar para o grupo correto
		switch (category)
		{
		case ESoundCategory::Music:
			channel->setChannelGroup(m_MusicGroup);
			break;
		case ESoundCategory::SFX:
			channel->setChannelGroup(m_SFXGroup);
			break;
		case ESoundCategory::Ambient:
			channel->setChannelGroup(m_AmbientGroup);
			break;
		case ESoundCategory::Voice:
			channel->setChannelGroup(m_VoiceGroup);
			break;
		case ESoundCategory::UI:
			channel->setChannelGroup(m_UIGroup);
			break;
		}
	}
}

void USoundManager::PauseAll()
{
	for (auto channel : m_ActiveChannels)
	{
		bool isPlaying = false;
		channel->isPlaying(&isPlaying);
		if (isPlaying)
		{
			channel->setPaused(true);
		}
	}
}

void USoundManager::PauseSoundByName(const std::string& soundName, bool pause)
{
	if (!m_System)
		return;

	FMOD::Sound* targetSound = GetSound(soundName);
	if (!targetSound)
		return;

	int numChannels = 0;
	m_System->getChannelsPlaying(&numChannels);

	for (int i = 0; i < 512; ++i)
	{
		FMOD::Channel* channel = nullptr;
		if (m_System->getChannel(i, &channel) == FMOD_OK && channel)
		{
			FMOD::Sound* currentSound = nullptr;
			if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
			{
				channel->setPaused(pause);
			}
		}
	}
}

void USoundManager::ResumeAll()
{
	for (auto channel : m_ActiveChannels)
	{
		bool paused = false;
		channel->getPaused(&paused);
		if (paused)
		{
			channel->setPaused(false);
		}
	}
}

void USoundManager::StopAll()
{
	for (auto channel : m_ActiveChannels)
	{
		channel->stop();
	}
	m_ActiveChannels.clear();
}

void USoundManager::SetVolumeAll(float volume)
{
	for (auto channel : m_ActiveChannels)
	{
		channel->setVolume(volume);
	}
}

void USoundManager::SetListenerPosition(float x, float y, float z, float forwardX, float forwardY, float forwardZ, float upX, float upY, float upZ)
{
	if (!m_System) return;

	FMOD_VECTOR pos = { x, y, z };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f }; // velocidade do ouvinte
	FMOD_VECTOR forward = { forwardX, forwardY, forwardZ };
	FMOD_VECTOR up = { upX, upY, upZ };

	m_System->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
}


void USoundManager::SetVolume(ESoundCategory soundGroup, float volume)
{
	switch (soundGroup)
	{
	case ESoundCategory::Music:
		m_MusicGroup->setVolume(volume);
		break;
	case ESoundCategory::SFX:
		m_SFXGroup->setVolume(volume);
		break;
	case ESoundCategory::Ambient:
		m_AmbientGroup->setVolume(volume);
		break;
	case ESoundCategory::Voice:
		m_VoiceGroup->setVolume(volume);
		break;
	case ESoundCategory::UI:
		m_UIGroup->setVolume(volume);
		break;
	default:
		m_MasterGroup->setVolume(volume);
		break;
	}
	
}

void USoundManager::Update()
{
	if (m_System)
	{
		m_System->update();
	}
}

FMOD::System* USoundManager::GetSystem()
{
	return m_System;
}

FMOD::Sound* USoundManager::GetSound(const std::string& name)
{
	auto it = m_Sounds.find(name);
	return (it != m_Sounds.end()) ? it->second : nullptr;
}
