// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "ArchetypeManager.h++"
#include "EngineComponents.h++"
#include "SystemBase.h++"

class SVelocitySystem final : public ISystemBase
{
public:
    SVelocitySystem() = default;

    void Update(float InDeltaTime) override
    {
        ArchetypeManager->Query<ECS::CVelocity>(
                    [InDeltaTime](ECS::Entity e, ECS::CVelocity& velocity)
                    {
                    }
                );
    }
};