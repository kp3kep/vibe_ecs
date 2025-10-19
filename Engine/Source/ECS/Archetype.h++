// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentHelper.h++"
#include "EntityManager.h++"


namespace ECS
{
    /**
     * @brief Архетип представляет собой уникальный набор типов компонентов.
     * Он хранит все сущности, которые имеют *в точности* этот набор.
     * Данные хранятся в формате SoA (Structure of Arrays).
     */
    class EArchetype {
    public:
        /**
         * @param InKey Отсортированный вектор ComponentTypeId,
         * определяющий этот архетип.
         */
        explicit EArchetype(ComponentSet InKey);

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

    template <typename T>
    std::vector<T>& EArchetype::GetComponentVector()
    {
        const ComponentTypeId TypeId = Component::TypeId<T>();
        // TODO: Мы используем .at() для проверки в debug-сборках, что компонент действительно существует
        IComponentVector* AbstractVector = ComponentsMap.at(TypeId).get();

        auto* ComponentVector = static_cast<EComponentVector<T>*>(AbstractVector);
        return ComponentVector->Data;
    }
} // namespace ECS