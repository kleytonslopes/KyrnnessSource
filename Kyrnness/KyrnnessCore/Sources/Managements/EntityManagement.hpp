/*********************************************************************
 *   File: EntityManagement.hpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: junho 2025
 * 
 * Copyright (c) 2025 Kyrnness. All rights reserved. 
 *********************************************************************/

#pragma once

#ifndef K_MANAGEMENTS_ENTITYMANAGEMENT_HPP
#define K_MANAGEMENTS_ENTITYMANAGEMENT_HPP

#include "Class.hpp"

class UEntityManagement : public UClass
{
    using Super = UClass;
public:
    UEntityManagement();
    virtual ~UEntityManagement();

    UEntityManagement(const UEntityManagement& other) = delete;
    UEntityManagement(UEntityManagement&& other) noexcept = delete;
    UEntityManagement& operator=(const UEntityManagement& other) = delete;
    UEntityManagement& operator=(UEntityManagement&& other) noexcept = delete;

    entt::registry& GetRegistry();

    FEntity CreateEntity();

    bool IsValidEntity(FEntity entity);

    template<typename T, typename... Args>
    T& AddComponent(FEntity entity, Args &&...args )
    {
        return m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

    template<typename T>
    T& GetComponent(FEntity entity)
    {
        return m_Registry.get<T>(entity);
    }

protected:
    void PreInitialize() override;


private:
    entt::registry m_Registry;
};

#endif // K_MANAGEMENTS_ENTITYMANAGEMENT_HPP
