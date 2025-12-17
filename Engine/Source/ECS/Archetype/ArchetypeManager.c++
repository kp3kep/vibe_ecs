#include "ArchetypeManager.h++"

namespace ECS
{
    FArchetypeManager::FArchetypeManager()
    {
        ComponentSet Empty{};
        auto [Iterator, _] = Archetypes.emplace(Empty, std::make_unique<FArchetype>(Empty));
        RootArchetype = Iterator->second.get();

        ArchetypeList.emplace_back(RootArchetype);
    }

    void FArchetypeManager::RegisterEntity(Entity InEntity)
    {
        const uint32_t EntityIndex = FEntityManager::GetEntityIndex(InEntity);
        EnsureRecordSize(EntityIndex + 1);
        const uint32_t Row = RootArchetype->AddEntity(InEntity);
        EntityRecords[EntityIndex] = { RootArchetype, Row };
    }

    void FArchetypeManager::UnregisterEntity(Entity InEntity)
    {
        const uint32_t EntityIndex = FEntityManager::GetEntityIndex(InEntity);

        if (EntityIndex < EntityRecords.size() && EntityRecords[EntityIndex].Archetype != nullptr)
        {
            FArchetype* Archetype = EntityRecords[EntityIndex].Archetype;
            const uint32_t Row = EntityRecords[EntityIndex].Row;
            const Entity MovedEntity = Archetype->RemoveEntity(Row);

            if (MovedEntity != 0)
            {
                // обновить запись перемещённой сущности (она теперь на позиции row того же архетипа)
                EntityRecords[FEntityManager::GetEntityIndex(MovedEntity)] = { Archetype, Row };
            }

            EntityRecords[EntityIndex] = { nullptr, 0 };
        }
    }

    FArchetype* FArchetypeManager::GetOrCreateArchetype(const ComponentSet& InKey)
    {
        // 1. Пытаемся найти
        const auto ArchetypeIterator = Archetypes.find(InKey);
        if (ArchetypeIterator != Archetypes.end())
        {
            return ArchetypeIterator->second.get();
        }

        // 2. Не нашли - создаем новый
        auto [InsertedIt, _] = Archetypes.emplace(InKey, std::make_unique<FArchetype>(InKey));
        FArchetype* RawPtr = InsertedIt->second.get();
        ArchetypeList.push_back(RawPtr);

        return RawPtr;
    }

    void FArchetypeManager::EnsureRecordSize(uint32_t NewSize)
    {
        if (EntityRecords.size() < NewSize)
        {
            EntityRecords.resize(NewSize, {nullptr, 0});
        }
    }
}