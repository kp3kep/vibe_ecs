// Copyright Vibe Coding. All Rights Reserved.

#include <memory>

#include "ComponentFactory.h++"
#include "ComponentsList.h++"
#include "Game.h++"

#include "ResourceManager.h++"

namespace Game
{
    EGame::EGame()
    {
        ECS::EComponentFactory::RegisterComponent<ECS::Transform>();
        ECS::EComponentFactory::RegisterComponent<ECS::Velocity>();
        ECS::EComponentFactory::RegisterComponent<ECS::Health>();
        ECS::EComponentFactory::RegisterComponent<ECS::C_Sprite>();
        ECS::EComponentFactory::RegisterComponent<ECS::PlayerInput>();
        ECS::EComponentFactory::RegisterComponent<ECS::AIInput>();
    }

    void EGame::OnBegin()
    {
        World = std::make_unique<EWorld>();
        World->Initialize();
        CreatePlayer();
    }

    void EGame::OnUpdate(float InDeltaTime)
    {
        World->Update(InDeltaTime);
    }

    void EGame::OnRender()
    {
        World->Render(*Graphics);
    }

    void EGame::CreatePlayer()
    {
        std::shared_ptr<sf::Texture> HeroTexture = EResourceManager::Get().LoadTexture("hero.png");
        World->CreatePlayer(HeroTexture);
    }
}
