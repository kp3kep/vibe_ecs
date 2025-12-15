// Copyright Vibe Coding. All Rights Reserved.

#include "World.h++"

#include "AIControlSystem.h++"
#include "AIInputSystem.h++"
#include "HeathSystem.h++"
#include "InputSystem.h++"
#include "MovementSystem.h++"
#include "PlayerControlSystem.h++"
#include "RenderSystem.h++"
#include "VelocitySystem.h++"

EWorld::EWorld()
{
    SystemManager.SetPool(&ThreadPool);
    ArchetypeManager.SetPool(&ThreadPool);

    RegisterSystem<EHeathSystem>();
    RegisterSystem<EInputSystem>();
    RegisterSystem<EAIInputSystem>();
    RegisterSystem<EMovementSystem>();
    RegisterSystem<EPlayerControlSystem>();
    RegisterSystem<EAIControlSystem>();
    RegisterSystem<EVelocitySystem>();

    SystemManager.RegisterRenderSystem<ERenderSystem>();
}

void EWorld::Initialize()
{
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

void EWorld::Render(FGraphics& Graphics) const
{
    SystemManager.Render(Graphics, const_cast<ECS::EArchetypeManager&>(ArchetypeManager));
}

void EWorld::CreatePlayer(const std::shared_ptr<sf::Texture>& InHeroTexture)
{
    const ECS::Entity Player = CreateEntity();
    ArchetypeManager.RegisterEntity(Player);

    ArchetypeManager.AddComponent(Player, ECS::Transform {100.f, 100.f});
    ArchetypeManager.AddComponent(Player, ECS::Velocity {0.f, 0.f});
    ArchetypeManager.AddComponent(Player, ECS::PlayerInput {0,0,false});
    ArchetypeManager.AddComponent(Player, ECS::C_Sprite { InHeroTexture });
}
