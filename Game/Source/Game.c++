// Copyright Vibe Coding. All Rights Reserved.

#include <memory>

#include "ComponentFactory.h++"
#include "ComponentsList.h++"
#include "Game.h++"

namespace Game
{
    EGame::EGame()
    {
        ECS::EComponentFactory::RegisterComponent<ECS::Transform>();
        ECS::EComponentFactory::RegisterComponent<ECS::Velocity>();
        ECS::EComponentFactory::RegisterComponent<ECS::Health>();
        ECS::EComponentFactory::RegisterComponent<ECS::C_Sprite>();
        ECS::EComponentFactory::RegisterComponent<ECS::C_PlayerInput>();
    }

    void EGame::OnBegin()
    {
        World = std::make_unique<EWorld>();
        World->Initialize();
    }

    void EGame::OnUpdate(float InDeltaTime)
    {
        World->Update(InDeltaTime);
    }

    void EGame::OnRender()
    {
        World->Render(*Graphics);
    }
}