// Copyright Vibe Coding. All Rights Reserved.

#include "World.h++"

#include "Systems/MovementSystem.h++"

EWorld::EWorld()
{
    RegisterSystem<EMovementSystem>();
}

void EWorld::Initialize()
{
    Entity Player = CreateEntity();
}

Entity EWorld::CreateEntity()
{
    return EntityManager.CreateEntity();
}

void EWorld::DestroyEntity(Entity EntityID)
{
    ArchetypeManager.RemoveEntity(EntityID);
}

void EWorld::Update(float InDeltaTime) const
{
    SystemManager.Update(InDeltaTime);
}