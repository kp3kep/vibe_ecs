// Copyright Vibe Coding. All Rights Reserved.

#include "EntityManager.h++"

namespace ECS
{
    Entity FEntityManager::CreateEntity()
    {
        uint32_t Index;
        if (FreeIndices.empty())
        {
            Index = static_cast<uint32_t>(GenerationByIndex.size());
            GenerationByIndex.push_back(1); // поколения начинаются с 1
        }
        else
        {
            Index = FreeIndices.back();
            FreeIndices.pop_back();
            // generation already set
        }
        return MakeEntity(Index, GenerationByIndex[Index]);
    }

    bool FEntityManager::IsAlive(Entity InEntity) const
    {
        const uint32_t EntityIndex = GetEntityIndex(InEntity);

        if (EntityIndex < GenerationByIndex.size())
        {
            const bool ValidGeneration = GenerationByIndex[EntityIndex] != 0;
            const bool SameGeneration = GetEntityGeneration(InEntity) == GenerationByIndex[EntityIndex];

            return ValidGeneration && SameGeneration;
        }
        return false;
    }

    void FEntityManager::Destroy(Entity InEntity)
    {
        const uint32_t EntityIndex = GetEntityIndex(InEntity);
        if (EntityIndex >= GenerationByIndex.size())
        {
            return;
        }

        ++GenerationByIndex[EntityIndex];
        FreeIndices.push_back(EntityIndex);
    }
}