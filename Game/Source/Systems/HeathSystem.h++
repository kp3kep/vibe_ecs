// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "ArchetypeManager.h++"
#include "ComponentsList.h++"
#include "SystemBase.h++"

class EHeathSystem final : public ISystemBase
{
public:
    EHeathSystem() = default;

    void Update(float InDeltaTime) override
    {
        ArchetypeManager->Query<ECS::Health>(
                    [InDeltaTime](ECS::Entity e, ECS::Health hp)
                    {
                        hp.hp -= InDeltaTime;
                    }
                );
    }
};