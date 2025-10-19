// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "ArchetypeManager.h++"
#include "SystemManager.h++"
#include "EntityManager.h++"

class EWorld
{
public:
    EWorld();
    ~EWorld() = default;

    void Initialize();

    ECS::Entity CreateEntity();
    void DestroyEntity(ECS::Entity EntityID);

    template<typename SystemType, std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool> = true>
    void RegisterSystem();

    void Update(float InDeltaTime) const;

private:
    ECS::EEntityManager EntityManager;
    ECS::EArchetypeManager ArchetypeManager;
    ESystemManager SystemManager;
};

template <typename SystemType, std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool>>
void EWorld::RegisterSystem()
{
    SystemManager.RegisterSystem<SystemType>();
}