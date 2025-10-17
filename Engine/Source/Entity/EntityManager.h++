// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <cstdint>

using Entity = uint32_t;

class EEntityManager
{
public:
    Entity CreateEntity();

private:
    Entity LastEntityID = 0;
};