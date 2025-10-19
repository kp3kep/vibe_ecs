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

        friend class EArchetype;
    };

    template <typename T>
 void EComponentVector<T>::AddEmptyElement()
    {
        Data.emplace_back();
    }

    template <typename T>
    void EComponentVector<T>::RemoveElement(uint32_t InRow)
    {
        auto LastRow = static_cast<uint32_t>(Data.size() - 1);
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
        ConcreteToVector->Data[ToRow] = std::move(Data[FromRow]);
    }

    template <typename T>
    size_t EComponentVector<T>::Size() const
    {
        return Data.size();
    }
}