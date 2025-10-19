// Copyright Vibe Coding. All Rights Reserved.

#pragma once
#include <cstdint>

namespace ECS
{
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

    struct Sprite
    {
        uint32_t TextureIndex;
    };
}