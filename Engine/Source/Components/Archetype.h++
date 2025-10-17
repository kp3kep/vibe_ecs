// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>
#include <unordered_map>

using Entity = uint32_t;

class EArchetypeBase {
public:
    virtual ~EArchetypeBase() = default;
    virtual void Remove(Entity) = 0;
};

template<typename... ComponentTypes>
class EArchetype final : public EArchetypeBase {
public:
    using ComponentTuple = std::tuple<std::vector<ComponentTypes>...>;

    // Добавление сущности и её компонентов
    void Add(Entity InEntity, const ComponentTypes&... InComponents);

    // Удаление по entity (swap-remove)
    void Remove(Entity InEntity) override;

    // Доступ к компонентам
    template<typename T>
    std::vector<T>& Get() { return std::get<std::vector<T>>(Data); }

    const std::vector<Entity>& GetEntities() const { return Entities; }

private:
    std::vector<Entity> Entities;
    ComponentTuple Data;
    std::unordered_map<Entity, size_t> EntityToIndex;
};

// Add
template<typename ... Components>
void EArchetype<Components...>::Add(Entity InEntity, const Components&... InComponents) {
    EntityToIndex[InEntity] = Entities.size();
    Entities.push_back(InEntity);
    (std::get<std::vector<Components>>(Data).push_back(InComponents), ...);
}

// Remove
template<typename ... Components>
void EArchetype<Components...>::Remove(Entity InEntity)  {
    size_t Index = EntityToIndex[InEntity];
    size_t LastIndex = Entities.size() - 1;

    if (Index != LastIndex)
    {
        const Entity LastEntity = Entities[LastIndex];
        std::swap(Entities[Index], Entities[LastIndex]);
        EntityToIndex[LastEntity] = Index;
        ([&]
        {
            auto& ComponentVector = std::get<std::vector<Components>>(Data);
            std::swap(ComponentVector[Index], ComponentVector[LastIndex]);
        }(), ...);
    }

    EntityToIndex.erase(InEntity);
    Entities.pop_back();
    (std::get<std::vector<Components>>(Data).pop_back(), ...);
}