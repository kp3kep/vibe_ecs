// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <algorithm>
#include <vector>

namespace ECS
{
    using ComponentTypeId = uintptr_t;
    using ComponentSet = std::vector<ComponentTypeId>;

    struct ComponentSetHash
    {
        size_t operator()(const ComponentSet& s) const noexcept
        {
            // 64-bit FNV-1a
            uint64_t h = 1469598103934665603ull;
            for (const auto v : s) {
                h ^= v;
                h *= 1099511628211ull;
            }
            return h;
        }
    };

    namespace Component
    {
        template <typename T>
        ComponentTypeId TypeId()
        {
            static char dummy;
            return reinterpret_cast<ComponentTypeId>(&dummy);
        }

        template <typename... Ts>
        ComponentSet MakeSet()
        {
            ComponentSet Result;
            Result.reserve(sizeof...(Ts));
            (Result.push_back(TypeId<Ts>()), ...);
            std::ranges::sort(Result);
            Result.erase(std::ranges::unique(Result).begin(), Result.end());
            return Result;
        }

        inline bool SetContains(const ComponentSet& InSet, ComponentTypeId InId)
        {
            return std::ranges::binary_search(InSet, InId);
        }

        inline ComponentSet SetUnion(const ComponentSet& InSet, ComponentTypeId ComponentId)
        {
            ComponentSet Result;
            Result.reserve(InSet.size() + 1);
            const auto Iterator = std::ranges::lower_bound(InSet, ComponentId);
            Result.insert(Result.end(), InSet.begin(), Iterator);

            if (Iterator == InSet.end() || *Iterator != ComponentId)
            {
                Result.push_back(ComponentId);
            }

            Result.insert(Result.end(), Iterator, InSet.end());
            return Result;
        }

        inline ComponentSet SetUnion(const ComponentSet& FirstSet, const ComponentSet& SecondSet)
        {
            ComponentSet Result;
            Result.reserve(FirstSet.size() + SecondSet.size());
            std::ranges::set_union(FirstSet, SecondSet, std::back_inserter(Result));
            return Result;
        }

        inline ComponentSet SetSubtract(const ComponentSet& SourceSet, ComponentTypeId ComponentToRemove)
        {
            ComponentSet Result;
            Result.reserve(SourceSet.size());
            for (auto Component : SourceSet)
            {
                if (Component != ComponentToRemove)
                {
                    Result.push_back(Component);
                }
            }
            return Result;
        }

        inline bool ContainsSubset(const ComponentSet& BaseSet, const ComponentSet& Subset)
        {
            // sub ⊆ sup
            return std::ranges::includes(BaseSet, Subset);
        }
    }

    struct Transform
    {
        float x;
        float y;
    };

    struct Velocity
    {
        float dx;
        float dy;
    };

    struct Health
    {
        float hp;
    };
}