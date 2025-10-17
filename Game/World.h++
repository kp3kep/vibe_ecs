// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "Components/ArchetypeManager.h++"
#include "SystemManager.h++"
#include "Entity/EntityManager.h++"

class EWorld
{
public:
    EWorld();
    ~EWorld() = default;

    void Initialize();

    Entity CreateEntity();
    void DestroyEntity(Entity EntityID);

    template<typename SystemType, std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool> = true>
    void RegisterSystem();

    void Update(float InDeltaTime) const;

private:
    EEntityManager EntityManager;
    EArchetypeManager ArchetypeManager;
    ESystemManager SystemManager;
};

template <typename SystemType, std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool>>
void EWorld::RegisterSystem()
{
    SystemManager.RegisterSystem<SystemType>();
}