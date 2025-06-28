/*********************************************************************
 *   File: MainCamera.hpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: junho 2025
 * 
 * Copyright (c) 2025 Kyrnness. All rights reserved. 
 *********************************************************************/

#pragma once

#ifndef K_COMPONENTS_MAINCAMERA_HPP
#define K_COMPONENTS_MAINCAMERA_HPP

#include "Components/Component.hpp"

class UMainCamera : public UComponent
{
    using Super = UComponent;
public:
    UMainCamera();
    virtual ~UMainCamera();

    UMainCamera(const UMainCamera& other) = default;
    UMainCamera(UMainCamera&& other) noexcept = default;
    UMainCamera& operator=(const UMainCamera& other) = default;
    UMainCamera& operator=(UMainCamera&& other) noexcept = default;

    bool GetIsActive() const { return bIsActive; }
    void SetIsActive(bool isActive) { bIsActive = isActive; }

    virtual nlohmann::json GetJsonData() override;

protected:
    bool bIsActive = true;

private:

};

#endif // K_COMPONENTS_MAINCAMERA_HPP
