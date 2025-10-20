// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "ArchetypeManager.h++"
#include "ComponentsList.h++"
#include "SystemBase.h++"

class EVelocitySystem final : public ISystemBase
{
public:
    EVelocitySystem() = default;

    void Update(float InDeltaTime) override
    {
        ArchetypeManager->Query<ECS::Velocity>(
                    [InDeltaTime](ECS::Entity e, ECS::Velocity& velocity)
                    {
                        velocity.dx += 0.0001f;
                        velocity.dy += 0.0001f;
                    }
                );
    }
};