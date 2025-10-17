// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>
#include <unordered_map>

#include "Archetype.h++"
#include "Components.h++"


class EArchetypeManager
{
public:
    template<typename... Components>
    EArchetype<Components...>& GetOrCreate();

    template<typename... Cs>
    void AddEntity(Entity e, const Cs&... comps);

    template<typename... Components>
    std::vector<EArchetypeBase*> Query();

    void RemoveEntity(Entity e);

private:
    template<typename T> constexpr
    static uint64_t GetComponentBit();

    template<typename... Components>
    static uint64_t MaskOf();

private:
    std::unordered_map<uint64_t, std::unique_ptr<EArchetypeBase>> Archetypes;
    std::unordered_map<Entity, uint64_t> EntitySignatures;
};

template<typename ... Components>
EArchetype<Components...>& EArchetypeManager::GetOrCreate()
{
    constexpr uint64_t Signature = (0 | ... | GetComponentBit<Components>());

    if (Archetypes.contains(Signature))
    {
        return *static_cast<EArchetype<Components...>*>(Archetypes[Signature].get());
    }

    auto ptr = std::make_unique<EArchetype<Components...>>();
    auto* raw = ptr.get();
    Archetypes[Signature] = std::move(ptr);
    return *raw;
}

template<typename ... Cs>
void EArchetypeManager::AddEntity(Entity e, const Cs &...comps)
{
    auto& archetype = GetOrCreate<Cs...>();
    archetype.add(e, comps...);
    EntitySignatures[e] = (0 | ... | GetComponentBit<Cs>());
}

template<typename ... Components>
std::vector<EArchetypeBase*> EArchetypeManager::Query()
{
    constexpr uint64_t ArchetypeMask = MaskOf<Components...>();
    std::vector<EArchetypeBase*> Result;

    for (auto& [Mask, ArchetypeBase] : Archetypes)
    {
        if ((Mask & ArchetypeMask) == ArchetypeMask)
        {
            Result.push_back(ArchetypeBase.get());
        }
    }

    return Result;
}

template<typename T>
constexpr uint64_t EArchetypeManager::GetComponentBit()
{
    if constexpr (std::is_same_v<T, Transform>) return TRANSFORM;
    if constexpr (std::is_same_v<T, Velocity>)  return VELOCITY;
    return 0;
}

template<typename... Components>
uint64_t EArchetypeManager::MaskOf()
{
    constexpr uint64_t Signature = (0 | ... | GetComponentBit<Components>());
    return Signature;
}