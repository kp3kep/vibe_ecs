// Copyright Vibe Coding. All Rights Reserved.

#include "SystemManager.h++"

#include "SystemBase.h++"

void FSystemManager::Update(float InDeltaTime) const
{
    for (auto& System : UpdateSystems)
    {
        System->Update(InDeltaTime);
    }
}

void FSystemManager::Render(FGraphics& Graphics, ECS::FArchetypeManager& ArchetypeManager) const
{
    for (auto& System : RenderSystems)
    {
        System->OnRender(Graphics, ArchetypeManager);
    }
}