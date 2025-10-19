// Copyright Vibe Coding. All Rights Reserved.

#include <memory>

#include "Game.h++"


void EGame::OnBegin()
{
    World = std::make_unique<EWorld>();
    World->Initialize();
}

void EGame::OnUpdate(float InDeltaTime)
{
    World->Update(InDeltaTime);
}