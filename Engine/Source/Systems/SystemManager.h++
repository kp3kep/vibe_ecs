// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>
#include <vector>

#include "SystemBase.h++"
#include "RenderSystemBase.h++"
#include "ThreadPool.h++"

class ESystemManager
{
public:
    ESystemManager() = default;

    template<typename SystemType, typename std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool> = true>
    SystemType* RegisterUpdateSystem();

    template<typename RenderSystemType, std::enable_if_t<std::is_base_of_v<IRenderSystemBase, RenderSystemType>, bool> = true>
    void RegisterRenderSystem();

    void Update(float InDeltaTime) const;
    void Render(FGraphics& Graphics, ECS::EArchetypeManager& ArchetypeManager) const;

    void SetPool(EThreadPool* ThreadPool) { Pool = ThreadPool; }

private:
    std::vector<std::unique_ptr<ISystemBase>> UpdateSystems;
    std::vector<std::unique_ptr<IRenderSystemBase>> RenderSystems;

    EThreadPool* Pool = nullptr;
};

template <typename SystemType, typename std::enable_if_t<std::is_base_of_v<ISystemBase, SystemType>, bool>>
SystemType* ESystemManager::RegisterUpdateSystem()
{
    auto System = std::make_unique<SystemType>();
    SystemType* RawPtr = System.get();
    UpdateSystems.push_back(std::move(System));
    return RawPtr;
}

template <typename RenderSystemType, std::enable_if_t<std::is_base_of_v<IRenderSystemBase, RenderSystemType>, bool>>
void ESystemManager::RegisterRenderSystem()
{
    auto System = std::make_unique<RenderSystemType>();
    RenderSystems.push_back(std::move(System));
}