// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <memory>

// Вперед объявляем наши новые классы
class FInput;
class FGraphics;

class IApplication
{
public:
    IApplication();
    virtual ~IApplication();

    int Run();

    virtual void OnBegin() = 0;
    virtual void OnUpdate(float InDeltaTime) = 0;
    virtual void OnRender() = 0;

protected:
    // Защищенные, чтобы EGame мог их видеть
    std::unique_ptr<FInput> Input;
    std::unique_ptr<FGraphics> Graphics;

private:
    bool IsFailed;
};