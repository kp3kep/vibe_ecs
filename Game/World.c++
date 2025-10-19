// Copyright Vibe Coding. All Rights Reserved.

#include "World.h++"

#include "Systems/MovementSystem.h++"

EWorld::EWorld()
{
    RegisterSystem<EMovementSystem>();
}

void EWorld::Initialize()
{
    const ECS::Entity Player = CreateEntity();
    ArchetypeManager.RegisterEntity(Player);
    ArchetypeManager.AddComponent(Player, ECS::Transform{0,0});
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