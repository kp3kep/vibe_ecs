// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include <utility>

#include "Archetype.h++"
#include "ComponentHelper.h++"
#include "EntityManager.h++"
#include "ThreadPool.h++"

namespace ECS
{
    class EArchetypeManager
    {
    public:
        EArchetypeManager();

        void RegisterEntity(Entity InEntity);
        void UnregisterEntity(Entity InEntity);

        /**
         * @brief Добавляет компонент сущности и перемещает ее в новый архетип.
         */
        template<typename T>
        T& AddComponent(Entity InEntity, T InComponent);

        /**
         * @brief Удаляет компонент у сущности и перемещает ее в новый архетип.
         */
        template<typename T>
        void RemoveComponent(Entity InEntity);

        /**
         * @brief Получает ссылку на компонент. O(1) операция.
         */
        template<typename T>
        T& GetComponent(Entity InEntity);

        /**
         * @brief Проверяет, есть ли у сущности компонент. O(1) операция.
         */
        template<typename T>
        bool HasComponent(Entity InEntity) const;

        /**
         * @brief Выполняет запрос (Query) по всем подходящим архетипам.
         */
        template<typename Write, typename... Reads, typename Func>
        void Query(Func&& QueryFunction);

        void SetPool(EThreadPool* ThreadPool) { Pool = ThreadPool;}

    private:
        EArchetype* GetOrCreateArchetype(const ComponentSet& key);
        void EnsureRecordSize(uint32_t NewSize);

    private:
        struct EntityRecord
        {
            EArchetype* Archetype = nullptr;
            uint32_t Row = 0;
        };

        std::vector<EntityRecord> EntityRecords;
        std::unordered_map<ComponentSet, std::unique_ptr<EArchetype>> Archetypes;
        std::vector<EArchetype*> ArchetypeList;
        EArchetype* RootArchetype;

        EThreadPool* Pool = nullptr;
    };

    template <typename T>
    T& EArchetypeManager::AddComponent(Entity InEntity, T InComponent)
    {
        const uint32_t EntityIndex = EEntityManager::GetEntityIndex(InEntity);
        EntityRecord& Record = EntityRecords[EntityIndex];
        EArchetype* FromArchetype = Record.Archetype;
        const uint32_t FromRow = Record.Row;

        // 1. Вычисляем ключ нового архетипа
        const ComponentSet ToKey = Component::SetUnion(FromArchetype->Key, Component::TypeId<T>());

        // 2. Находим или создаем целевой архетип
        EArchetype* ToArchetype = GetOrCreateArchetype(ToKey);

        // 3. Добавляем сущность в новый архетип (создает пустые слоты)
        const uint32_t ToRow = ToArchetype->AddEntity(InEntity);

        // 4. Перемещаем старые данные
        for (ComponentTypeId TypeId = 0; TypeId < MAX_COMPONENTS; ++TypeId)
        {
            if (ToArchetype->Key.test(TypeId) && FromArchetype->Key.test(TypeId))
            {
                IComponentVector* FromVector = FromArchetype->ComponentsMap.at(TypeId).get();
                IComponentVector* ToVector = ToArchetype->ComponentsMap.at(TypeId).get();
                FromVector->MoveElement(FromRow, ToVector, ToRow);
            }
        }

        // 5. Устанавливаем значение нового компонента
        ToArchetype->GetComponentVector<T>()[ToRow] = std::move(InComponent);

        // 6. Удаляем сущность из старого архетипа (swap-and-pop)
        const Entity MovedEntity = FromArchetype->RemoveEntity(FromRow);

        // 7. Обновляем запись для перемещенной сущности
        if (MovedEntity != 0)
        {
            EntityRecords[EEntityManager::GetEntityIndex(MovedEntity)] = { FromArchetype, FromRow };
        }

        // 8. Обновляем запись для *нашей* сущности
        Record = { ToArchetype, ToRow };

        return ToArchetype->GetComponentVector<T>()[ToRow];
    }

    template <typename T>
    void EArchetypeManager::RemoveComponent(Entity InEntity)
    {
        const uint32_t EntityIndex = EEntityManager::GetEntityIndex(InEntity);
        EntityRecord& Record = EntityRecords[EntityIndex];
        EArchetype* FromArchetype = Record.Archetype;
        const uint32_t FromRow = Record.Row;

        // 1. Вычисляем ключ нового архетипа
        const ComponentSet ToKey = SetSubtract(FromArchetype->Key, Component::TypeId<T>());

        // 2. Находим или создаем целевой архетип
        EArchetype* ToArchetype = GetOrCreateArchetype(ToKey);

        // 3. Добавляем сущность в новый архетип
        const uint32_t ToRow = ToArchetype->AddEntity(InEntity);

        // 4. Перемещаем старые данные (те, что есть в новом архетипе)
        //    Итерируем по ключу *назначения* - это проще.
        for (ComponentTypeId TypeId = 0; TypeId < MAX_COMPONENTS; ++TypeId)
        {
            if (ToArchetype->Key.test(TypeId) && FromArchetype->Key.test(TypeId))
            {
                IComponentVector* FromVector = FromArchetype->ComponentsMap.at(TypeId).get();
                IComponentVector* ToVector = ToArchetype->ComponentsMap.at(TypeId).get();
                FromVector->MoveElement(FromRow, ToVector, ToRow);
            }
        }

        // 5. Удаляем сущность из старого архетипа
        const Entity MovedEntity = FromArchetype->RemoveEntity(FromRow);

        // 6. Обновляем запись для перемещенной сущности
        if (MovedEntity != 0)
        {
            EntityRecords[EEntityManager::GetEntityIndex(MovedEntity)] = { FromArchetype, FromRow };
        }

        // 7. Обновляем запись для *нашей* сущности
        Record = { ToArchetype, ToRow };
    }

    template <typename T>
    T& EArchetypeManager::GetComponent(Entity InEntity)
    {
        const uint32_t EntityIndex = EEntityManager::GetEntityIndex(InEntity);
        // Тут хорошо бы добавить ассерты для отладки
        assert(EntityIndex < EntityRecords.size() && "Entity index out of bounds");
        auto& [Archetype, Row] = EntityRecords[EntityIndex];
        assert(Archetype != nullptr && "Entity is not registered or was destroyed");

        return Archetype->GetComponentVector<T>()[Row];
    }

    template <typename T>
    bool EArchetypeManager::HasComponent(Entity InEntity) const
    {
        const uint32_t EntityIndex = EEntityManager::GetEntityIndex(InEntity);
        if (EntityIndex >= EntityRecords.size())
        {
            return false;
        }

        const auto& [Archetype, Row] = EntityRecords[EntityIndex];
        if (Archetype == nullptr)
        {
            return false;
        }

        return SetContains(Archetype->Key, Component::TypeId<T>());
    }

    template <typename Write, typename... Reads, typename Func>
    void EArchetypeManager::Query(Func&& QueryFunction)
    {
        // 1. Создаем ключ запроса (один раз)
        const ComponentSet QueryKey = Component::MakeSet<Write, Reads...>();
        std::vector<std::future<void>> Futures;

        // 2. Итерируемся по плоскому списку архетипов
        for (EArchetype* Archetype : ArchetypeList)
        {
            // 3. Проверяем, содержит ли архетип все нужные компоненты
            if (!Component::ContainsSubset(Archetype->Key, QueryKey))
                continue;

            auto ArchetypeJob = [Archetype, QueryFunction]()
            {
                auto& Entities = Archetype->Entities;
                auto& WriteVec = Archetype->GetComponentVector<Write>();

                auto ReadVecsTuple = std::make_tuple(
                    std::ref(Archetype->GetComponentVector<Reads>())...
                );

                const size_t EntityCount = Entities.size();
                for (size_t i = 0; i < EntityCount; ++i)
                {
                    std::apply(
                        [&](auto&... ReadVecs) { // ReadVecs - это ссылки на векторы
                            QueryFunction(
                                Entities[i],
                                WriteVec[i],
                                // .get() нужен, т.к. мы использовали std::ref
                                (static_cast<const Reads&>(ReadVecs[i]))...
                            );
                        },
                        ReadVecsTuple
                    );
                }
            };
            Futures.push_back(Pool->enqueue(ArchetypeJob));
        }
        for (auto& Future : Futures)
        {
            Future.get();
        }
    }
} // namespace ECS