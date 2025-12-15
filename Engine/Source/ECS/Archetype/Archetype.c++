// Copyright Vibe Coding. All Rights Reserved.

#include <ranges>

#include "Archetype.h++"
#include "ComponentFactory.h++"

namespace ECS
{
    EArchetype::EArchetype(const ComponentSet& InKey): Key(InKey)
    {
        for (ComponentTypeId TypeId = 0; TypeId < MAX_COMPONENTS; ++TypeId)
        {
            if (Key.test(TypeId))
            {
                ComponentsMap[TypeId] = EComponentFactory::CreateVector(TypeId);
            }
        }
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