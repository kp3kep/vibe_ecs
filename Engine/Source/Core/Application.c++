// Copyright Vibe Coding. All Rights Reserved.

#include "Application.h++"
#include "Input.h++"
#include "Graphics.h++"

#include <chrono>
#include <iostream>
#include <exception>

// Для теста красного круга
#include "SFML/Graphics/CircleShape.hpp"

IApplication::IApplication() : IsFailed(false)
{
    try
    {
        Input = std::make_unique<FInput>();
        Graphics = std::make_unique<FGraphics>();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to initialize Application: " << e.what() << std::endl;
        IsFailed = true;
    }
}

IApplication::~IApplication() = default;

int IApplication::Run()
{
    if (IsFailed) return 1;

    OnBegin();

    using FClock = std::chrono::high_resolution_clock;
    using Seconds = std::chrono::duration<float>;
    auto LastTick = FClock::now();

    // --- ТЕСТОВЫЙ ОБЪЕКТ ---
    sf::CircleShape DebugShape(100.f);
    DebugShape.setFillColor(sf::Color::Red);
    DebugShape.setPosition({300.f, 200.f});
    // ------------------------------------

    while (Graphics->IsOpen())
    {
        Input->PollEvents(Graphics->GetWindow());

        if (Input->IsWillQuit()) break;

        auto CurrentTick = FClock::now();
        Seconds DeltaTime = CurrentTick - LastTick;
        LastTick = CurrentTick;

        std::cout << 1 / DeltaTime.count() << " FPS" << std::endl;

        OnUpdate(DeltaTime.count());

        Graphics->Begin();
        OnRender();
        Graphics->End();
    }

    return 0;
}