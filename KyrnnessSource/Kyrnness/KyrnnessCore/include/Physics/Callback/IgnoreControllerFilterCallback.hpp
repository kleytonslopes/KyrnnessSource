#pragma once
#ifndef KYRNNESS_IGNORE_CONTROLLER_FILTER_CALLBACK_HPP
#define KYRNNESS_IGNORE_CONTROLLER_FILTER_CALLBACK_HPP

#include <PxPhysicsAPI.h>

class IgnoreControllerFilterCallback : public physx::PxQueryFilterCallback
{
public:
    physx::PxShape* IgnoredShape = nullptr;

    explicit IgnoreControllerFilterCallback(physx::PxShape* shapeToIgnore)
        : IgnoredShape(shapeToIgnore)
    {}

    // Pré-filtro chamado para cada shape que o sweep intersecta
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
    // Pós-filtro — raramente usado aqui
    physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor)
    {
        return physx::PxQueryHitType::eBLOCK;
    }
};

#endif //KYRNNESS_IGNORE_CONTROLLER_FILTER_CALLBACK_HPP
