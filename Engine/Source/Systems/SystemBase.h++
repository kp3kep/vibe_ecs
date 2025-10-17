// Copyright Vibe Coding. All Rights Reserved.

#pragma once

class ISystemBase
{
public:
    virtual ~ISystemBase() = default;
    virtual void Update(float) = 0;
};