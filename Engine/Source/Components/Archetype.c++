// Copyright Vibe Coding. All Rights Reserved.

#include "Archetype.h++"

namespace ECS
{
    template <typename T>
    void EComponentVector<T>::AddEmptyElement()
    {
        Data.emplace_back();
    }

    template <typename T>
    void EComponentVector<T>::RemoveElement(uint32_t InRow)
    {
        uint32_t LastRow = static_cast<uint32_t>(Data.size() - 1);
        if (InRow != LastRow) {
            // Быстрое удаление: меняем местами с последним...
            std::swap(Data[InRow], Data[LastRow]);
        }
        // ...и удаляем последний
        Data.pop_back();
    }

    template <typename T>
    void EComponentVector<T>::MoveElement(uint32_t FromRow, IComponentVector* ToVector, uint32_t ToRow)
    {
        // Мы знаем, что 'toVector' (если он не null)
        // должен быть того же типа ComponentVector<T>
        auto* ConcreteToVector = static_cast<EComponentVector<T>*>(ToVector);

        // Перемещаем данные из старого слота в новый
        // Слот toRow уже должен быть создан вызовом addEmptyElement
        // в целевом архетипе
        ConcreteToVector->data[ToRow] = std::move(Data[FromRow]);
    }

    template <typename T>
    size_t EComponentVector<T>::Size() const
    {
        return Data.size();
    }

    template <typename T>
    std::vector<T>& EArchetype::GetComponentVector()
    {
        const ComponentTypeId TypeId = Component::TypeId<T>();
        // TODO: Мы используем .at() для проверки в debug-сборках, что компонент действительно существует
        IComponentVector* AbstractColumn = ComponentsMap.at(TypeId).get();

        auto* ConcreteColumn = static_cast<EComponentVector<T>*>(AbstractColumn);
        return ConcreteColumn->Data;
    }

    uint32_t EArchetype::AddEntity(Entity NewEntity)
    {
        const auto NewRow = static_cast<uint32_t>(Entities.size());
        Entities.push_back(NewEntity);

        // Расширяем *каждую* колонку компонентов
        for (auto& ComponentVector : ComponentsMap | std::views::values)
        {
            ComponentVector->AddEmptyElement();
        }

        // assert(Entities.size() == ComponentsMap[0]->Size()); // Проверка консистентности
        return NewRow;
    }

    Entity EArchetype::RemoveEntity(uint32_t InRowIndex)
    {
        Entity MovedEntity = 0;
        const auto LastRow = static_cast<uint32_t>(Entities.size() - 1);

        // 1. Swap-and-pop в векторе сущностей
        if (InRowIndex != LastRow)
        {
            MovedEntity = Entities[LastRow]; // Запоминаем, кто переехал
            std::swap(Entities[InRowIndex], Entities[LastRow]);
        }
        Entities.pop_back();

        // 2. Swap-and-pop в *каждой* колонке компонентов
        for (auto& ComponentVector : ComponentsMap | std::views::values)
        {
            ComponentVector->RemoveElement(InRowIndex);
        }

        return MovedEntity;
    }
}