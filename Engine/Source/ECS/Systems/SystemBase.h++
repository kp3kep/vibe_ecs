// Copyright Vibe Coding. All Rights Reserved.

#pragma once

namespace ECS
{
    class FArchetypeManager;
}

class ISystemBase
{
public:
    virtual ~ISystemBase() = default;
    virtual void Update(float) = 0;

    void SetArchetypeManager(ECS::FArchetypeManager* InArchManager)
    {
        ArchetypeManager = InArchManager;
    }

protected:
    ECS::FArchetypeManager* ArchetypeManager = nullptr;
};