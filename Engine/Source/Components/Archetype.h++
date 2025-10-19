// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Components.h++"
#include "EntityManager.h++"


namespace ECS
{
    // ------------------------------------------------------------------
    // 1. Интерфейс (Абстракция) для колонки с компонентами
    // ------------------------------------------------------------------

    /**
     * @brief Абстрактный базовый класс для любой колонки компонентов.
     * Позволяет Archetype хранить и обрабатывать векторы компонентов,
     * не зная их конкретного типа (т.е. "стирание типов").
     */
    class IComponentVector
    {
    public:
        virtual ~IComponentVector() = default;

        /**
         * @brief Добавляет в конец вектора пустой (default-constructed) элемент.
         * Вызывается, когда в архетип добавляется новая сущность.
         */
        virtual void AddEmptyElement() = 0;

        /**
         * @brief Удаляет элемент из строки 'row' методом "swap-and-pop".
         * Вызывается, когда из архетипа удаляется сущность.
         */
        virtual void RemoveElement(uint32_t InRow) = 0;

        /**
         * @brief Перемещает элемент из 'fromRow' этого вектора
         * в 'toRow' другого вектора 'toVector'.
         * Вызывается при миграции сущности.
         */
        virtual void MoveElement(uint32_t FromRow, IComponentVector* ToVector, uint32_t ToRow) = 0;

        /**
         * @brief Возвращает количество элементов в колонке.
         */
        [[nodiscard]] virtual size_t Size() const = 0;
    };

    // ------------------------------------------------------------------
    // 2. Конкретная реализация колонки
    // ------------------------------------------------------------------

    /**
     * @brief Шаблонная реализация колонки компонентов для конкретного типа T.
     */
    template <typename T>
    class EComponentVector final : public IComponentVector
    {

    public:

        void AddEmptyElement() override;

        void RemoveElement(uint32_t InRow) override;

        void MoveElement(uint32_t FromRow, IComponentVector* ToVector, uint32_t ToRow) override;

        [[nodiscard]] size_t Size() const override;

    private:
        std::vector<T> Data;
    };


    // ------------------------------------------------------------------
    // 3. Сам класс Архетипа
    // ------------------------------------------------------------------

    /**
     * @brief Архетип представляет собой уникальный набор типов компонентов.
     * Он хранит все сущности, которые имеют *в точности* этот набор.
     * Данные хранятся в формате SoA (Structure of Arrays).
     */
    class EArchetype {
    public:
        /**
         * @param key Отсортированный вектор ComponentTypeId,
         * определяющий этот архетип.
         */
        explicit EArchetype(ComponentSet key) : Key(std::move(key))
        {
            // TODO: Create vector components
        }

        // Запрещаем копирование, разрешаем только перемещение
        EArchetype(const EArchetype&) = delete;
        EArchetype& operator=(const EArchetype&) = delete;

        /**
         * @brief Добавляет сущность в этот архетип,
         * создает для Нее пустые слоты во всех колонках компонентов.
         * @param NewEntity Сущность для добавления.
         * @return Новый индекс строки (row) для этой сущности в этом архетипе.
         */
        uint32_t AddEntity(Entity NewEntity);

        /**
         * @brief Удаляет сущность из строки 'row' методом "swap-and-pop".
         * @param InRowIndex Индекс строки для удаления.
         * @return Сущность, которая была перемещена на место удаленной
         * (или 0, если удалялась последняя).
         */
        Entity RemoveEntity(uint32_t InRowIndex);

        /**
         * @brief Быстрый (но небезопасный) доступ к вектору данных.
         * Вызывающий код (ArchetypeManager) *должен* быть уверен,
         * что компонент T существует в этом архетипе.
         */
        template<typename T>
        std::vector<T>& GetComponentVector();

    private:
        ComponentSet Key;
        std::vector<Entity> Entities;
        std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentVector>> ComponentsMap;

        friend class EArchetypeManager;
    };
} // namespace ECS