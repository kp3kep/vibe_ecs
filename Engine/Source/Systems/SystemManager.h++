// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>
#include <vector>

#include "SystemBase.h++"

class ESystemManager
{
public:
    template<typename SystemType, typename std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool> = true>
    void RegisterSystem();

    void Update(float InDeltaTime) const;

private:
    std::vector<std::unique_ptr<ISystemBase>> Systems;
};

template <typename SystemType, typename std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool>>
void ESystemManager::RegisterSystem()
{
    auto System = std::make_unique<SystemType>();
    Systems.push_back(std::move(System));
}