// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>

#include "Application.h++"
#include "World.h++"

namespace Game
{
    class EGame final : public IApplication
    {
    public:
        EGame();

        void OnBegin() override;
        void OnUpdate(float InDeltaTime) override;
        void OnRender() override;

    private:
        std::unique_ptr<EWorld> World;
    };
}