// Copyright Vibe Coding. All Rights Reserved.

#pragma once

namespace ECS
{
    class EArchetypeManager;
}

class ISystemBase
{
public:
    virtual ~ISystemBase() = default;
    virtual void Update(float) = 0;

    void SetArchetypeManager(ECS::EArchetypeManager* InArchManager)
    {
        ArchetypeManager = InArchManager;
    }

protected:
    ECS::EArchetypeManager* ArchetypeManager = nullptr;
};