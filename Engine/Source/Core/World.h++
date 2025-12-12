// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "ArchetypeManager.h++"
#include "SystemManager.h++"
#include "EntityManager.h++"
#include "ThreadPool.h++"
#include "SFML/Graphics/Texture.hpp"

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
    void Render(FGraphics& Graphics) const;
    void CreatePlayer(const std::shared_ptr<sf::Texture>& InHeroTexture);

private:
    ECS::EEntityManager EntityManager;

public:
    ECS::EArchetypeManager ArchetypeManager;

private:
    ESystemManager SystemManager;

    EThreadPool ThreadPool;
};

template <typename SystemType, std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool>>
void EWorld::RegisterSystem()
{
    SystemType* NewSystem = SystemManager.RegisterUpdateSystem<SystemType>();
    NewSystem->SetArchetypeManager(&ArchetypeManager);
}