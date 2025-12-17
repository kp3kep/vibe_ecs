// Copyright Vibe Coding. All Rights Reserved.

#include <memory>

#include "ComponentFactory.h++"
#include "EngineComponents.h++"
#include "Game.h++"

#include "ResourceManager.h++"

namespace Game
{
    FGame::FGame()
    {
        ECS::FComponentFactory::RegisterComponent<ECS::CTransform>();
        ECS::FComponentFactory::RegisterComponent<ECS::CVelocity>();
        ECS::FComponentFactory::RegisterComponent<ECS::CHealth>();
        ECS::FComponentFactory::RegisterComponent<ECS::CSprite>();
        ECS::FComponentFactory::RegisterComponent<ECS::CPlayerInput>();
        ECS::FComponentFactory::RegisterComponent<ECS::CAIInput>();
    }

    void FGame::OnBegin()
    {
        World = std::make_unique<EWorld>();
        World->Initialize();
        CreatePlayer();
    }

    void FGame::OnUpdate(float InDeltaTime)
    {
        World->Update(InDeltaTime);
    }

    void FGame::OnRender()
    {
        World->Render(*Graphics);
    }

    void FGame::CreatePlayer()
    {
        std::shared_ptr<sf::Texture> HeroTexture = EResourceManager::Get().LoadTexture("hero.png");
        World->CreatePlayer(HeroTexture);
    }
}
