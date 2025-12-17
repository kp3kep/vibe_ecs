// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <algorithm>
#include <bitset>
#include <vector>

namespace ECS
{
    constexpr size_t MAX_COMPONENTS = 256;
    using ComponentSet = std::bitset<MAX_COMPONENTS>;

    using ComponentTypeId = uint32_t;

    namespace Internal
    {
        class FComponentRegistry
        {
        public:
            template <typename>
            static ComponentTypeId TypeId()
            {
                static const ComponentTypeId TypeId = Counter()++;
                return TypeId;
            }

        private:
            static ComponentTypeId& Counter()
            {
                static ComponentTypeId Count = 0;
                return Count;
            }
        };
    } // namespace Internal

    namespace Component
    {
        template <typename T>
        ComponentTypeId TypeId()
        {
            return Internal::FComponentRegistry::TypeId<T>();
        }

        template <typename... Ts>
        ComponentSet MakeSet()
        {
            ComponentSet set;
            (set.set(TypeId<Ts>()), ...);
            return set;
        }

        inline bool ContainsSubset(const ComponentSet& BaseSet, const ComponentSet& Subset)
        {
            // sub ⊆ base
            return (BaseSet & Subset) == Subset;
        }

        inline ComponentSet SetUnion(const ComponentSet& InSet, ComponentTypeId ComponentId)
        {
            auto Result = InSet;
            Result.set(ComponentId);
            return Result;
        }

        inline ComponentSet SetUnion(const ComponentSet& FirstSet, const ComponentSet& SecondSet)
        {
            return FirstSet | SecondSet;
        }

        inline ComponentSet SetSubtract(const ComponentSet& SourceSet, ComponentTypeId ComponentToRemove)
        {
            auto Result = SourceSet;
            Result.reset(ComponentToRemove);
            return Result;
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
    class FComponentVector final : public IComponentVector
    {
    public:
        void AddEmptyElement() override;

        void RemoveElement(uint32_t InRow) override;

        void MoveElement(uint32_t FromRow, IComponentVector* ToVector, uint32_t ToRow) override;

        [[nodiscard]] size_t Size() const override;

    private:
        std::vector<T> Data;

        friend class FArchetype;
    };

    template <typename T>
    void FComponentVector<T>::AddEmptyElement()
    {
        Data.emplace_back();
    }

    template <typename T>
    void FComponentVector<T>::RemoveElement(uint32_t InRow)
    {
        auto LastRow = static_cast<uint32_t>(Data.size() - 1);
        if (InRow != LastRow)
        {
            // Быстрое удаление: меняем местами с последним...
            std::swap(Data[InRow], Data[LastRow]);
        }
        // ...и удаляем последний
        Data.pop_back();
    }

    template <typename T>
    void FComponentVector<T>::MoveElement(uint32_t FromRow, IComponentVector* ToVector, uint32_t ToRow)
    {
        // Мы знаем, что 'toVector' (если он не null)
        // должен быть того же типа ComponentVector<T>
        auto* ConcreteToVector = static_cast<FComponentVector<T>*>(ToVector);

        // Перемещаем данные из старого слота в новый
        // Слот toRow уже должен быть создан вызовом addEmptyElement
        // в целевом архетипе
        ConcreteToVector->Data[ToRow] = std::move(Data[FromRow]);
    }

    template <typename T>
    size_t FComponentVector<T>::Size() const
    {
        return Data.size();
    }
}
