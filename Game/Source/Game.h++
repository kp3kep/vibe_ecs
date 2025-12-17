// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>

#include "Application.h++"
#include "World.h++"

namespace Game
{
    class FGame final : public IApplication
    {
    public:
        FGame();

        void OnBegin() override;
        void OnUpdate(float InDeltaTime) override;
        void OnRender() override;
        void CreatePlayer();

    private:
        std::unique_ptr<EWorld> World;
    };
}