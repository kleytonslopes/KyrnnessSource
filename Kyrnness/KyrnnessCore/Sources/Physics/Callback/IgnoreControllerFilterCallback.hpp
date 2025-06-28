/*********************************************************************
 *   File: IgnoreControllerFilterCallback.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_IGNORE_CONTROLLER_FILTER_CALLBACK_HPP
#define K_IGNORE_CONTROLLER_FILTER_CALLBACK_HPP

#include <PxPhysicsAPI.h>

class IgnoreControllerFilterCallback : public physx::PxQueryFilterCallback
{
public:
    physx::PxShape* IgnoredShape = nullptr;

    explicit IgnoreControllerFilterCallback(physx::PxShape* shapeToIgnore)
        : IgnoredShape(shapeToIgnore)
    {}

    // Pr�-filtro chamado para cada shape que o sweep intersecta
    physx::PxQueryHitType::Enum preFilter(
        const physx::PxFilterData& filterData,
        const physx::PxShape* shape,
        const physx::PxRigidActor* actor,
        physx::PxHitFlags& queryFlags) override
    {
        if (shape == IgnoredShape)
            return physx::PxQueryHitType::eNONE; // ignora

        return physx::PxQueryHitType::eBLOCK;
    }
    //const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor
    // P�s-filtro � raramente usado aqui
    physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor)
    {
        return physx::PxQueryHitType::eBLOCK;
    }
};

#endif //K_IGNORE_CONTROLLER_FILTER_CALLBACK_HPP
