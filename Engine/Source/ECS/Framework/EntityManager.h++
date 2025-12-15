// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cstdint>
#include <vector>

namespace ECS
{
    using Entity = uint32_t;


    class EEntityManager
    {
    public:
        // Константы для упаковки/распаковки сущности внутри менеджера
        static constexpr uint32_t INDEX_BITS = 22;     // ~4 млн сущностей
        static constexpr uint32_t GENERATION_BITS = 10;
        static constexpr uint32_t INDEX_MASK = (1u << INDEX_BITS) - 1u;

        // Вспомогательные методы работы с Entity
        static inline uint32_t GetEntityIndex(Entity EntityId)
        {
            return EntityId & INDEX_MASK;
        }

        static inline uint32_t GetEntityGeneration(Entity EntityId)
        {
            return EntityId >> INDEX_BITS;
        }

        static inline Entity MakeEntity(uint32_t Index, uint32_t Generation)
        {
            return (Generation << INDEX_BITS) | Index;
        }

        // Создаёт новую сущность:
        // - если есть свободные индексы, переиспользует последний из них;
        //   поколение уже инкрементировано в destroy(), поэтому остаётся валидным
        //   и отличным от всех ранее выданных Entity для этого индекса;
        // - иначе выделяет новый индекс и инициализирует его поколение значением 1.
        // Возвращает упакованный Entity (generation << INDEX_BITS | index).
        Entity CreateEntity();

        // Проверка, что Entity ещё жив:
        // - его индекс в пределах таблицы;
        // - поколение в таблице совпадает с поколением внутри Entity;
        // - поколение не равно 0 (0 — невалидное состояние).
        [[nodiscard]] bool IsAlive(Entity InEntity) const;

        // Уничтожает сущность:
        // - инкрементирует поколение слота, тем самым инвалидируя все старые Entity
        //   с прежним поколением (ловим висячие ссылки);
        // - избегает значения 0 как валидного поколения (переполнение);
        // - добавляет индекс слота в free list для будущего переиспользования.
        void Destroy(Entity InEntity);

    private:
        // Таблица поколений по индексу сущности:
        // generationByIndex[index] хранит текущее поколение слота.
        // Значение 0 зарезервировано как невалидное (мёртвый/неинициализированный слот).
        std::vector<uint32_t> GenerationByIndex;
        // Стек свободных индексов (free list), позволяющий переиспользовать слоты
        // без постоянного роста таблицы.
        std::vector<uint32_t> FreeIndices;
    };
}