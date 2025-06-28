/*********************************************************************
 *   File: MemoryManager.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_MEMORY_MEMORYMANAGER_HPP
#define K_MEMORY_MEMORYMANAGER_HPP

#include <unordered_map>
#include <functional>
#include <vector>
#include <mutex>
#include <typeindex>
#include "Core/Logger.hpp"
#include "Core/Core.hpp"

class FMemoryManager
{
public:
	static FMemoryManager& Get()
	{
		static FMemoryManager instance;
		return instance;
	}

	// Alocação global
	template<typename T, typename... Args>
	static T* Allocate(Args&&... args)
	{
		T* ptr = new T(std::forward<Args>(args)...);

		FLogger::Log("Allocated: %p (%s)", ptr, typeid(T).name());

		Get().Track(ptr, [](void* p) { delete static_cast<T*>(p); }, typeid(T));
		return ptr;
	}

	static void SetPendingDestroy(void* ptr)
	{
		Get().AddPendingDestroy(ptr);
	}

	template<typename T>
	static bool Deallocate(T* ptr)
	{
		return Get().Untrack(ptr);
	}

	template<typename T>
	static std::vector<T*> GetAll()
	{
		return Get().GetAllOfType<T>();
	}

	static void Cleanup()
	{
		FLogger::Warning("FMemoryManager::Cleanup");
		Get().FreeAll();
	}

	static void ProcessPendingDestroy()
	{
		Get().ProcessPendingDestroyInternal();
	}

private:
	using Deleter = TFunction<void(void*)>;

	TMap<void*, Deleter> m_Allocations;

	std::unordered_multimap<std::type_index, void*> m_TypeMap;
	std::mutex mtx;;
	std::vector<void*> m_PendingDestroy;

	FMemoryManager() = default;
	~FMemoryManager() { FreeAll(); }

	FMemoryManager(const FMemoryManager&) = delete;
	FMemoryManager& operator=(const FMemoryManager&) = delete;

	void Track(void* ptr, Deleter deleter, std::type_index type)
	{
		std::lock_guard<std::mutex> lock(mtx);
		m_Allocations[ptr] = deleter;
		m_TypeMap.emplace(type, ptr);
	}

	template<typename T>
	bool Untrack(T* ptr)
	{
		std::lock_guard<std::mutex> lock(mtx);

		auto it = m_Allocations.find(ptr);
		if (it != m_Allocations.end())
		{
			it->second(ptr);
			m_Allocations.erase(it);

			auto range = m_TypeMap.equal_range(typeid(T));
			for (auto iter = range.first; iter != range.second; ++iter)
			{
				if (iter->second == ptr)
				{
					FLogger::Log("Untrack pointer: %p", ptr);
					m_TypeMap.erase(iter);
					break;
				}
			}

			return true;
		}
		return false;
	}

	template<typename T>
	std::vector<T*> GetAllOfType()
	{
		std::lock_guard<std::mutex> lock(mtx);
		std::vector<T*> result;

		auto range = m_TypeMap.equal_range(typeid(T));
		for (auto it = range.first; it != range.second; ++it)
		{
			result.push_back(static_cast<T*>(it->second));
		}

		return result;
	}

	void FreeAll()
	{
		std::lock_guard<std::mutex> lock(mtx);

		for (auto& [ptr, deleter] : m_Allocations)
		{
			try
			{
				if (ptr == nullptr || reinterpret_cast<uintptr_t>(ptr) < 0x1000)
				{
					FLogger::Fatal("Skipping invalid pointer: %p", ptr);
					continue;
				}

				if (deleter)
				{
					FLogger::Log("Freeing pointer: %p", ptr);
					deleter(ptr);
				}
			}
			catch (const std::exception& e)
			{
				FLogger::Fatal("Exception while deleting pointer: %p - %s", ptr, e.what());
			}
			catch (...)
			{
				FLogger::Fatal("Unknown exception while deleting pointer: %p", ptr);
			}
		}

		m_Allocations.clear();
		m_TypeMap.clear();
		m_PendingDestroy.clear();
	}

	void AddPendingDestroy(void* ptr)
	{
		std::lock_guard<std::mutex> lock(mtx);

		// Não adicionar duplicados
		if (std::find(m_PendingDestroy.begin(), m_PendingDestroy.end(), ptr) == m_PendingDestroy.end())
		{
			m_PendingDestroy.push_back(ptr);
			FLogger::Log("Marked for pending destroy: %p", ptr);
		}
	}

	void ProcessPendingDestroyInternal()
	{
		std::vector<void*> toDelete;

		{
			std::lock_guard<std::mutex> lock(mtx);
			if (m_PendingDestroy.empty())
				return;

			toDelete.swap(m_PendingDestroy);
		}

		for (void* ptr : toDelete)
		{
			if (!Deallocate(ptr))
			{
				FLogger::Warning("Tried to destroy untracked pointer: %p", ptr);
			}
		}
	}
};

#endif // K_MEMORY_MEMORYMANAGER_HPP
