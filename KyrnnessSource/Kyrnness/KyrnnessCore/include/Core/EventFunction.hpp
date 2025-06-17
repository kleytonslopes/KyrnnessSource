#pragma once
#ifndef KYRNNESS_EVENT_FUNCTION_HPP
#define KYRNNESS_EVENT_FUNCTION_HPP

#include <vector>
#include <functional>
#include <unordered_map>
#include <cstddef>

class FDelegateHandle
{
public:
	FDelegateHandle() = default;
	explicit FDelegateHandle(size_t id) : m_ID(id) {}

	bool IsValid() const { return m_ID != 0; }
	size_t GetID() const { return m_ID; }

	bool operator==(const FDelegateHandle& other) const { return m_ID == other.m_ID; }
	bool operator!=(const FDelegateHandle& other) const { return m_ID != other.m_ID; }

private:
	size_t m_ID = 0; // ID == 0 significa "inválido"
};

template<typename... Args>
class FFunctionEvent
{
public:
    using HandlerType = std::function<void(Args...)>;

    // Adiciona função lambda, std::function, etc
    FDelegateHandle AddListener(const HandlerType& listener)
    {
        FDelegateHandle handle(++m_LastID);
        m_Listeners[handle.GetID()] = listener;
        return handle;
    }

    // Adiciona membro de classe: obj e método
    template<typename ClassType>
    FDelegateHandle AddListener(ClassType* instance, void (ClassType::* method)(Args...))
    {
        return AddListener([=](Args... args) {
            (instance->*method)(args...);
            });
    }

    // Remove via handle
    bool RemoveListener(FDelegateHandle handle)
    {
        return m_Listeners.erase(handle.GetID()) > 0;
    }

    // Chamada para todos os listeners
    void Broadcast(Args... args) const
    {
        for (const auto& [id, listener] : m_Listeners)
        {
            if (listener)
                listener(args...);
        }
    }

    void Clear()
    {
        m_Listeners.clear();
    }

private:
    size_t m_LastID = 0;
    std::unordered_map<size_t, HandlerType> m_Listeners;
};

/*
template<typename... Args>
class FFunctionEvent
{
public:
    using HandlerType = std::function<void(Args...)>;

    FDelegateHandle AddListener(const HandlerType& listener)
    {
        FDelegateHandle handle(++m_LastID);
        m_Listeners[handle.GetID()] = listener;
        return handle;
    }

    bool RemoveListener(FDelegateHandle handle)
    {
        return m_Listeners.erase(handle.GetID()) > 0;
    }

    void Broadcast(Args... args) const
    {
        for (const auto& [id, listener] : m_Listeners)
        {
            if (listener)
                listener(args...);
        }
    }

    void Clear()
    {
        m_Listeners.clear();
    }

private:
    size_t m_LastID = 0;
    std::unordered_map<size_t, HandlerType> m_Listeners;
};
*/

#endif// KYRNNESS_EVENT_FUNCTION_HPP
