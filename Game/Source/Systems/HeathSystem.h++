// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "ArchetypeManager.h++"
#include "EngineComponents.h++"
#include "SystemBase.h++"

class SHeathSystem final : public ISystemBase
{
public:
    SHeathSystem() = default;

    void Update(float InDeltaTime) override
    {
        ArchetypeManager->Query<ECS::CHealth>(
                    [InDeltaTime](ECS::Entity e, ECS::CHealth hp)
                    {
                        hp.hp -= InDeltaTime;
                    }
                );
    }
};