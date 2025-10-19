// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "ComponentHelper.h++"

namespace ECS
{
    class EComponentFactory
    {
    public:
        template<typename T>
        static void RegisterComponent()
        {
            const ComponentTypeId TypeId = Component::TypeId<T>();
            GetComponentFactory()[TypeId] = []->std::unique_ptr<IComponentVector>
            {
                return std::make_unique<EComponentVector<T>>();
            };
        }

        static std::unique_ptr<IComponentVector> CreateVector(ComponentTypeId InId)
        {
            const auto FactoryIterator = GetComponentFactory().find(InId);
            if (FactoryIterator == GetComponentFactory().end())
            {
                // TODO: Добавить нормальную обработку ошибок (assert/log)
                // Это значит, что мы пытаемся создать архетип с компонентом,
                // который "забыли" зарегистрировать.
                return nullptr;
            }
            return FactoryIterator->second();
        }

    private:
        static std::unordered_map<ComponentTypeId, std::function<std::unique_ptr<IComponentVector>()>>& GetComponentFactory()
        {
            static std::unordered_map<ComponentTypeId, std::function<std::unique_ptr<IComponentVector>()>> Factories;
            return Factories;
        }
    };
}