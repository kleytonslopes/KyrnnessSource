#include "pch.hpp"
#include "Components/AudioSourceComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Audio/SoundManager.hpp"
#include "Runtime/Application.hpp"

void UAudioSourceComponent::OnUpdate(float DeltaTime)
{
    UTransformComponent& transform = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

    SetPosition(transform.Location);

    // Limpar canais que j� terminaram
    m_ActiveChannels.erase(
        std::remove_if(m_ActiveChannels.begin(), m_ActiveChannels.end(),
            [](FMOD::Channel* channel)
            {
                bool isPlaying = false;
                if (channel)
                    channel->isPlaying(&isPlaying);
                return !isPlaying;
            }),
        m_ActiveChannels.end()
    );

    for (auto it = m_ActiveFades.begin(); it != m_ActiveFades.end(); )
    {
        FActiveFade& fade = *it;
        fade.Elapsed += DeltaTime;

        float t = glm::clamp(fade.Elapsed / fade.Duration, 0.0f, 1.0f);
        float newVolume = glm::mix(fade.StartVolume, fade.TargetVolume, t);

        if (fade.Channel)
        {
            fade.Channel->setVolume(newVolume);
        }

        if (t >= 1.0f)
        {
            if (fade.Type == EFadeType::FadeOut && fade.Channel)
            {
                fade.Channel->stop();
            }
            it = m_ActiveFades.erase(it);
        }
        else
        {
            ++it;
        }
    }

    Super::OnUpdate(DeltaTime);
}

void UAudioSourceComponent::Play()
{
    FMOD::Sound* sound = UApplication::Get().GetSoundManager()->GetSound(m_SoundName);
    if (!sound)
        return;

    FMOD::Channel* channel = nullptr;
    UApplication::Get().GetSoundManager()->GetSystem()->playSound(sound, nullptr, false, &channel);
    if (channel)
    {
        m_Channel = channel;
        m_Channel->setVolume(m_Volume);

        if (bLoop)
        {
            sound->setMode(FMOD_LOOP_NORMAL);
        }
        else
        {
            sound->setMode(FMOD_LOOP_OFF);
        }

        UTransformComponent& transform = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

        SetPosition(transform.Location);
    }
}

void UAudioSourceComponent::PlaySoundByName(const std::string& soundName, float volume)
{
    FMOD::Sound* sound = UApplication::Get().GetSoundManager()->GetSound(soundName);

    if (!sound)
        return;

    FMOD::Channel* channel = nullptr;
    UApplication::Get().GetSoundManager()->GetSystem()->playSound(sound, nullptr, false, &channel);

    if (channel)
    {
        channel->setVolume(volume);

        FMOD_VECTOR pos = { m_Position.x, m_Position.y, m_Position.z };
        FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
        channel->set3DAttributes(&pos, &vel);

        m_ActiveChannels.push_back(channel);
    }
}

void UAudioSourceComponent::Stop()
{
    if (m_Channel)
    {
        m_Channel->stop();
        m_Channel = nullptr;
    }
}

void UAudioSourceComponent::StopAll()
{
    for (auto* channel : m_ActiveChannels)
    {
        if (channel)
        {
            channel->stop();
        }
    }
    m_ActiveChannels.clear();
}

void UAudioSourceComponent::StopSoundByName(const std::string& soundName)
{
    FMOD::Sound* targetSound = UApplication::Get().GetSoundManager()->GetSound(soundName);
    if (!targetSound)
        return;

    for (auto it = m_ActiveChannels.begin(); it != m_ActiveChannels.end(); )
    {
        FMOD::Channel* channel = *it;
        if (channel)
        {
            FMOD::Sound* currentSound = nullptr;
            if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
            {
                channel->stop();
                it = m_ActiveChannels.erase(it);  // Remove da lista
                continue;
            }
        }
        ++it;
    }
}

void UAudioSourceComponent::Pause(bool pause)
{
    if (m_Channel)
    {
        m_Channel->setPaused(pause);
    }
}

void UAudioSourceComponent::PauseAll(bool pause)
{
    for (auto* channel : m_ActiveChannels)
    {
        if (channel)
        {
            channel->setPaused(pause);
        }
    }
}

void UAudioSourceComponent::PauseSoundByName(const std::string& soundName, bool pause)
{
    FMOD::Sound* targetSound = UApplication::Get().GetSoundManager()->GetSound(soundName);
    if (!targetSound)
        return;

    for (FMOD::Channel* channel : m_ActiveChannels)
    {
        if (channel)
        {
            FMOD::Sound* currentSound = nullptr;
            if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
            {
                channel->setPaused(pause);
            }
        }
    }
}

void UAudioSourceComponent::SetLoop(bool loop)
{
    bLoop = loop;
}

void UAudioSourceComponent::SetVolume(float volume)
{
    m_Volume = volume;
    if (m_Channel)
    {
        m_Channel->setVolume(volume);
    }
}

void UAudioSourceComponent::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    
    FMOD_VECTOR pos = { position.x, position.y, position.z };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

    for (auto* channel : m_ActiveChannels)
    {
        if (channel)
        {
            channel->set3DAttributes(&pos, &vel);
        }
    }
}

void UAudioSourceComponent::SetSoundName(const std::string& soundName)
{
    m_SoundName = soundName;
}

void UAudioSourceComponent::SetVolumeBySoundName(const std::string& soundName, float volume)
{
    FMOD::Sound* targetSound = UApplication::Get().GetSoundManager()->GetSound(soundName);
    if (!targetSound)
        return;

    for (FMOD::Channel* channel : m_ActiveChannels)
    {
        if (channel)
        {
            FMOD::Sound* currentSound = nullptr;
            if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
            {
                channel->setVolume(volume);
            }
        }
    }
}

bool UAudioSourceComponent::IsSoundPlayingByName(const std::string& soundName) const
{
    FMOD::Sound* targetSound = UApplication::Get().GetSoundManager()->GetSound(soundName);
    if (!targetSound)
        return false;

    for (FMOD::Channel* channel : m_ActiveChannels)
    {
        if (channel)
        {
            FMOD::Sound* currentSound = nullptr;
            bool isPlaying = false;
            if (channel->getCurrentSound(&currentSound) == FMOD_OK &&
                currentSound == targetSound &&
                channel->isPlaying(&isPlaying) == FMOD_OK &&
                isPlaying)
            {
                return true;
            }
        }
    }

    return false;
}

void UAudioSourceComponent::FadeInSoundByName(const std::string& soundName, float fadeTime, float targetVolume)
{
    FMOD::Sound* targetSound = UApplication::Get().GetSoundManager()->GetSound(soundName);
    if (!targetSound || fadeTime <= 0.0f)
        return;

    bool found = false;

    for (FMOD::Channel* channel : m_ActiveChannels)
    {
        if (channel)
        {
            FMOD::Sound* currentSound = nullptr;
            if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
            {
                found = true;

                channel->setVolume(0.0f);

                m_ActiveFades.push_back(FActiveFade{
                    channel,
                    fadeTime,
                    0.0f,
                    0.0f,
                    targetVolume,
                    EFadeType::FadeIn
                    });
            }
        }
    }

    // Se n�o achou nenhum canal tocando esse som, ent�o toca o som com volume 0 e inicia fade
    if (!found)
    {
        PlaySoundByName(soundName, 0.0f);  // Toca com volume zero

        // Agora aplicar o fade no novo canal que foi criado
        for (FMOD::Channel* channel : m_ActiveChannels)
        {
            if (channel)
            {
                FMOD::Sound* currentSound = nullptr;
                if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
                {
                    m_ActiveFades.push_back(FActiveFade{
                        channel,
                        fadeTime,
                        0.0f,
                        0.0f,
                        targetVolume,
                        EFadeType::FadeIn
                        });
                    break; // S� adiciona uma vez
                }
            }
        }
    }
}

void UAudioSourceComponent::FadeOutSoundByName(const std::string& soundName, float fadeTime)
{
    FMOD::Sound* targetSound = UApplication::Get().GetSoundManager()->GetSound(soundName);
    if (!targetSound || fadeTime <= 0.0f)
        return;

    for (FMOD::Channel* channel : m_ActiveChannels)
    {
        if (channel)
        {
            FMOD::Sound* currentSound = nullptr;
            if (channel->getCurrentSound(&currentSound) == FMOD_OK && currentSound == targetSound)
            {
                float currentVolume = 1.0f;
                channel->getVolume(&currentVolume);

                m_ActiveFades.push_back(FActiveFade{
                    channel,
                    fadeTime,
                    0.0f,
                    currentVolume,
                    0.0f,
                    EFadeType::FadeOut
                    });
            }
        }
    }
}

void UAudioSourceComponent::CrossFade(const std::string& fromSound, const std::string& toSound, float fadeTime, float targetVolume)
{
    // Iniciar Fade Out do som atual
    FadeOutSoundByName(fromSound, fadeTime);

    // Tocar o som novo com volume 0
    PlaySoundByName(toSound, 0.0f);  // Come�a no volume 0

    // Depois iniciar Fade In
    FadeInSoundByName(toSound, fadeTime, targetVolume);
}

nlohmann::json UAudioSourceComponent::GetJsonData()
{
    return nlohmann::json();
}
