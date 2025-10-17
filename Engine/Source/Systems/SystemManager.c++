// Copyright Vibe Coding. All Rights Reserved.

#include "SystemManager.h++"

#include "SystemBase.h++"

void ESystemManager::Update(float InDeltaTime) const
{
    for (auto& System : Systems)
    {
        System->Update(InDeltaTime);
    }
}