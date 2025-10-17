// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cstdint>

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

enum ComponentBits : uint64_t {
    TRANSFORM = 1 << 0,
    VELOCITY  = 1 << 1,
    HEALTH    = 1 << 2,
};