// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "Systems/SystemBase.h++"
#include "ECS/ArchetypeManager.h++"
#include "ECS/ComponentsList.h++"

class EAIInputSystem final : public ISystemBase
{
public:
    void Update(float InDeltaTime) override
    {
        if (!ArchetypeManager)
        {
            return;
        }

        // Ищем всех, у кого есть компонент ввода (обычно это только игрок)
        ArchetypeManager->Query<ECS::AIInput>(
            [](ECS::Entity e, ECS::AIInput& Input)
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