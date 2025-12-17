// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cmath>
#include <iostream>

#include "ArchetypeManager.h++"
#include "EngineComponents.h++"
#include "SystemBase.h++"

class SMovementSystem final : public ISystemBase
{
public:
    SMovementSystem() = default;

    void Update(float InDeltaTime) override
    {
        ArchetypeManager->Query<ECS::CTransform, ECS::CVelocity>(
                    [InDeltaTime](ECS::Entity, ECS::CTransform& Transform, const ECS::CVelocity& Velocity)
                    {
                        Transform.x += Velocity.dx * InDeltaTime;
                        Transform.y += Velocity.dy * InDeltaTime;
                    }
                );
    }

private:
};