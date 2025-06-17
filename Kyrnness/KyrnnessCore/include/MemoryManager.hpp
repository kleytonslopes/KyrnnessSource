#pragma once
#ifndef KYRNESS_MEMORY_MEMORYMANAGER_HPP
#define KYRNESS_MEMORY_MEMORYMANAGER_HPP

#include <unordered_map>
#include <functional>
#include <vector>
#include <mutex>
#include <typeindex>
#include "Core/Logger.hpp"

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
        LOG(Log, TEXT("Allocated: %p (%s)", ptr, typeid(T).name()));
        Get().Track(ptr, [](void* p) { delete static_cast<T*>(p); }, typeid(T));
        return ptr;
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
        LOG(Warning, TEXT("FMemoryManager::Cleanup"));
        Get().FreeAll();
    }

private:
    using Deleter = std::function<void(void*)>;

    std::unordered_map<void*, Deleter> m_Allocations;
    std::unordered_multimap<std::type_index, void*> m_TypeMap;
    std::mutex mtx;;

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
                    LOG(Log, TEXT("Untrack pointer: %p", ptr));
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
                    LOG(Fatal, TEXT("Skipping invalid pointer: %p"), ptr);
                    continue;
                }

                if (deleter)
                {
                    LOG(Log, TEXT("Freeing pointer: %p"), ptr);
                    deleter(ptr);
                }
            }
            catch (const std::exception& e)
            {
                LOG(Fatal, TEXT("Exception while deleting pointer: %p - %s"), ptr, e.what());
            }
            catch (...)
            {
                LOG(Fatal, TEXT("Unknown exception while deleting pointer: %p"), ptr);
            }
        }

        m_Allocations.clear();
        m_TypeMap.clear();
    }
};

#endif // KYRNESS_MEMORY_MEMORYMANAGER_HPP
