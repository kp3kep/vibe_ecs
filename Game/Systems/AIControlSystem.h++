// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "Systems/SystemBase.h++"
#include "ECS/ArchetypeManager.h++"
#include "ECS/ComponentsList.h++"

class EAIControlSystem final : public ISystemBase
{
public:
    void Update(float InDeltaTime) override
    {
        if (!ArchetypeManager)
        {
            return;
        }

        constexpr float Speed = 200.0f; // Скорость пикселей в секунду

        // Читаем Input, Пишем в Velocity
        ArchetypeManager->Query<ECS::Velocity, ECS::AIInput>(
            [](ECS::Entity e, ECS::Velocity& Velocity, const ECS::AIInput& Input)
            {
                Velocity.dx = Input.MoveX * Speed;
                Velocity.dy = Input.MoveY * Speed;

                // Нормализация диагонального движения (чтобы по диагонали не бежал быстрее)
                if (Velocity.dx != 0 && Velocity.dy != 0)
                {
                    constexpr float Factor = 0.7071f; // 1 / sqrt(2)
                    Velocity.dx *= Factor;
                    Velocity.dy *= Factor;
                }
            }
        );
    }
};