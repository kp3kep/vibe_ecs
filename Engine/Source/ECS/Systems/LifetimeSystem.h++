// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <vector>

#include "SystemBase.h++"
#include "World.h++"

class SLifetimeSystem final : public ISystemBase
{
public:
    void Update(float InDeltaTime) override
    {
        if (!ArchManager || !World) return;

        // Чтобы не ломать итератор во время цикла (Правило №8),
        // мы соберем мертвецов в список и удалим ПОСЛЕ цикла.
        std::vector<ECS::Entity> ToDestroy;

        ArchManager->Query<ECS::C_Lifetime>(
            [&](ECS::Entity e, ECS::C_Lifetime& life)
            {
                life.TimeRemaining -= InDeltaTime;
                if (life.TimeRemaining <= 0.0f)
                {
                    ToDestroy.push_back(e);
                }
            }
        );

        // Deferred Execution (Отложенное выполнение) - соблюдаем инвариант!
        for (ECS::Entity e : ToDestroy)
        {
            World->DestroyEntity(e);
        }
    }
};