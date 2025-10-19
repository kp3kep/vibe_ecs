// Copyright Vibe Coding. All Rights Reserved.

#pragma once

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
                    [InDeltaTime](ECS::Entity e, ECS::Transform& t, const ECS::Velocity& v)
                    {
                        t.x += v.dx * InDeltaTime;
                        t.y += v.dy * InDeltaTime;
                    }
                );
    }

private:
};