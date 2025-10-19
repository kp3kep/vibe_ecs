#include "ArchetypeManager.h++"

namespace ECS
{
    EArchetypeManager::EArchetypeManager()
    {
        ComponentSet Empty{};
        auto [Iterator, _] = Archetypes.emplace(Empty, std::make_unique<EArchetype>(Empty));
        RootArchetype = Iterator->second.get();

        ArchetypeList.emplace_back(RootArchetype);
    }

    void EArchetypeManager::RegisterEntity(Entity InEntity)
    {
        const uint32_t EntityIndex = EEntityManager::GetEntityIndex(InEntity);
        EnsureRecordSize(EntityIndex + 1);
        const uint32_t Row = RootArchetype->AddEntity(InEntity);
        EntityRecords[EntityIndex] = { RootArchetype, Row };
    }

    void EArchetypeManager::UnregisterEntity(Entity InEntity)
    {
        const uint32_t EntityIndex = EEntityManager::GetEntityIndex(InEntity);

        if (EntityIndex < EntityRecords.size() && EntityRecords[EntityIndex].Archetype != nullptr)
        {
            EArchetype* Archetype = EntityRecords[EntityIndex].Archetype;
            const uint32_t Row = EntityRecords[EntityIndex].Row;
            const Entity MovedEntity = Archetype->RemoveEntity(Row);

            if (MovedEntity != 0)
            {
                // обновить запись перемещённой сущности (она теперь на позиции row того же архетипа)
                EntityRecords[EEntityManager::GetEntityIndex(MovedEntity)] = { Archetype, Row };
            }

            EntityRecords[EntityIndex] = { nullptr, 0 };
        }
    }

    EArchetype* EArchetypeManager::GetOrCreateArchetype(const ComponentSet& InKey)
    {
        // 1. Пытаемся найти
        const auto ArchetypeIterator = Archetypes.find(InKey);
        if (ArchetypeIterator != Archetypes.end())
        {
            return ArchetypeIterator->second.get();
        }

        // 2. Не нашли - создаем новый
        auto [InsertedIt, _] = Archetypes.emplace(InKey, std::make_unique<EArchetype>(InKey));
        EArchetype* RawPtr = InsertedIt->second.get();
        ArchetypeList.push_back(RawPtr);

        return RawPtr;
    }

    void EArchetypeManager::EnsureRecordSize(uint32_t NewSize)
    {
        if (EntityRecords.size() < NewSize)
        {
            EntityRecords.resize(NewSize, {nullptr, 0});
        }
    }
}