// Copyright Vibe Coding. All Rights Reserved.

#include "SystemManager.h++"

#include "SystemBase.h++"

void ESystemManager::Update(float InDeltaTime) const
{
    for (auto& System : UpdateSystems)
    {
        System->Update(InDeltaTime);
    }
}

void ESystemManager::Render(FGraphics& Graphics, ECS::EArchetypeManager& ArchetypeManager) const
{
    for (auto& System : RenderSystems)
    {
        System->OnRender(Graphics, ArchetypeManager);
    }
}