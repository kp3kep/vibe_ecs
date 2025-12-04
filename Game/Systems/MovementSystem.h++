// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cmath>
#include <iostream>

#include "ArchetypeManager.h++"
#include "ComponentsList.h++"
#include "SystemBase.h++"

class EMovementSystem final : public ISystemBase
{
public:
    EMovementSystem() = default;

    void Update(float InDeltaTime) override
    {
        ArchetypeManager->Query<ECS::Transform, ECS::Velocity>(
                    [InDeltaTime](ECS::Entity, ECS::Transform& Transform, const ECS::Velocity& Velocity)
                    {
                        Transform.x += Velocity.dx * InDeltaTime;
                        Transform.y += Velocity.dy * InDeltaTime;
                    }
                );
    }

private:
};