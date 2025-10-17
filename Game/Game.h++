// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>

#include "Application.h++"

class EWorld;

class EGame final : public IApplication
{
public:
    void OnBegin() override;
    void OnUpdate(float InDeltaTime) override;

private:
    std::unique_ptr<EWorld> World;
};