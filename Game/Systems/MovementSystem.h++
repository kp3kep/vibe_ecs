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
        float SumOfX = 0.0f;
        ArchetypeManager->Query<ECS::Transform, ECS::Velocity>(
                    [InDeltaTime, &SumOfX](ECS::Entity, ECS::Transform& t, const ECS::Velocity& v)
                    {
                        t.x += std::sin(v.dx) * InDeltaTime;
                        t.y += std::sin(v.dy) * InDeltaTime;

                        SumOfX += t.x / t.y;
                    }
                );

        std::cout << "Sum: " << SumOfX << '\r';
    }

private:
};