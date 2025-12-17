// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SystemBase.h++"
#include "ArchetypeManager.h++"
#include "EngineComponents.h++"

class SAIInputSystem final : public ISystemBase
{
public:
    void Update(float InDeltaTime) override
    {
        if (!ArchetypeManager)
        {
            return;
        }

        // Ищем всех, у кого есть компонент ввода (обычно это только игрок)
        ArchetypeManager->Query<ECS::CAIInput>(
            [](ECS::Entity e, ECS::CAIInput& Input)
            {
                Input.TempTimer += 0.01;
                if (Input.TempTimer >= 0.5)
                {
                    Input.MoveX *= -1;
                    Input.TempTimer = 0;
                }

            }
        );
    }
};