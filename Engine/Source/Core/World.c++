// Copyright Vibe Coding. All Rights Reserved.

#include "World.h++"

#include "ComponentsList.h++"
#include "HeathSystem.h++"
#include "MovementSystem.h++"
#include "VelocitySystem.h++"

EWorld::EWorld()
{
    RegisterSystem<EMovementSystem>();
    RegisterSystem<EHeathSystem>();
    RegisterSystem<EVelocitySystem>();
}

void EWorld::Initialize()
{
    StressTest();
}

ECS::Entity EWorld::CreateEntity()
{
    return EntityManager.CreateEntity();
}

void EWorld::DestroyEntity(ECS::Entity EntityID)
{
    ArchetypeManager.UnregisterEntity(EntityID);
}

void EWorld::Update(float InDeltaTime) const
{
    SystemManager.Update(InDeltaTime);
}

void EWorld::StressTest()
{
    constexpr int ENTITY_COUNT = 1'000'000;

    for (int i = 0; i < ENTITY_COUNT; ++i)
    {
        const ECS::Entity NewEntity = CreateEntity();
        ArchetypeManager.RegisterEntity(NewEntity);

        // Даем позицию
        ArchetypeManager.AddComponent(NewEntity, ECS::Transform{
                                          static_cast<float>(i % 1000), // Просто какие-то значения
                                          static_cast<float>(i) / 1000
                                      });

        // Даем скорость (чтобы система их двигала)
        ArchetypeManager.AddComponent(NewEntity, ECS::Velocity{ 1.0f, 0.5f });

        if (i % 2)
        {
            ArchetypeManager.AddComponent(NewEntity, ECS::Health{ 100 });
        }
        if (i % 3)
        {
            ArchetypeManager.AddComponent(NewEntity, ECS::Sprite{77});
        }
    }
}