/*********************************************************************
 *   File: EventFunction.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_EVENT_FUNCTION_HPP
#define K_EVENT_FUNCTION_HPP

#include "Core/TypesDefinition.hpp"

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
	size_t m_ID = 0;
};

template<typename... Args>
class FFunctionEvent
{
public:
	using HandlerType = TFunction<void(Args...)>;

	struct FListenerEntry
	{
		HandlerType Callback;
		void* ObjectPtr = nullptr;
		void* MethodPtr = nullptr;
	};

	FDelegateHandle AddListener(const HandlerType& listener)
	{
		FDelegateHandle handle(++m_LastID);
		FListenerEntry entry;
		entry.Callback = listener;
		m_Listeners[handle.GetID()] = entry;
		return handle;
	}

	template<typename ClassType>
	FDelegateHandle AddListener(ClassType* instance, void (ClassType::* method)(Args...))
	{
		FDelegateHandle handle(++m_LastID);
		FListenerEntry entry;
		entry.ObjectPtr = static_cast<void*>(instance);
		entry.MethodPtr = *reinterpret_cast<void**>(&method);
		entry.Callback = [=](Args... args) {
			(instance->*method)(args...);
			};
		m_Listeners[handle.GetID()] = entry;
		return handle;
	}

	template<typename ClassType>
	FDelegateHandle AddListener(const ClassType* instance, void (ClassType::* method)(Args...) const)
	{
		FDelegateHandle handle(++m_LastID);
		FListenerEntry entry;
		entry.ObjectPtr = const_cast<void*>(static_cast<const void*>(instance));
		entry.MethodPtr = *reinterpret_cast<void**>(&method);
		entry.Callback = [=](Args... args) {
			(instance->*method)(args...);
			};
		m_Listeners[handle.GetID()] = entry;
		return handle;
	}

	bool RemoveListener(FDelegateHandle handle)
	{
		return m_Listeners.erase(handle.GetID()) > 0;
	}

	template<typename ClassType>
	bool RemoveListener(ClassType* instance, void (ClassType::* method)(Args...))
	{
		void* objPtr = static_cast<void*>(instance);
		void* methodPtr = *reinterpret_cast<void**>(&method);

		for (auto it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
		{
			if (it->second.ObjectPtr == objPtr && it->second.MethodPtr == methodPtr)
			{
				m_Listeners.erase(it);
				return true;
			}
		}
		return false;
	}

	template<typename ClassType>
	bool RemoveListener(const ClassType* instance, void (ClassType::* method)(Args...) const)
	{
		void* objPtr = const_cast<void*>(static_cast<const void*>(instance));
		void* methodPtr = *reinterpret_cast<void**>(&method);

		for (auto it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
		{
			if (it->second.ObjectPtr == objPtr && it->second.MethodPtr == methodPtr)
			{
				m_Listeners.erase(it);
				return true;
			}
		}
		return false;
	}

	void Broadcast(Args... args) const
	{
		for (const auto& [id, listener] : m_Listeners)
		{
			if (listener.Callback)
				listener.Callback(args...);
		}
	}

	void Clear()
	{
		m_Listeners.clear();
	}

private:
	size_t m_LastID = 0;
	TMap<size_t, FListenerEntry> m_Listeners;
};

#endif// K_EVENT_FUNCTION_HPP
