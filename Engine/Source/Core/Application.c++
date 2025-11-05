// Copyright Vibe Coding. All Rights Reserved.

#include "Application.h++"
#include "Input.h++"
#include "Graphics.h++"
#include "SFML/Graphics.hpp"

#include <chrono>
#include <iostream>

IApplication::IApplication() : IsFailed(false)
{
    // 1. Создаем наши классы-обертки
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

// Виртуальный деструктор нужен для unique_ptr
IApplication::~IApplication() = default;


int IApplication::Run()
{
    if (IsFailed)
    {
        return 1;
    }

    // OnBegin() - EGame создаст здесь World
    OnBegin();

    using Time = std::chrono::high_resolution_clock;
    using Seconds = std::chrono::duration<float>;
    auto LastTick = Time::now();

    // ===============================================
    //           НАШ НОВЫЙ ИГРОВОЙ ЦИКЛ
    // ===============================================
    // (Он работает, пока окно SFML открыто)
    while (Graphics->IsOpen())
    {
        // 1. СЧИТЫВАЕМ ИНПУТ
        // (Этот PollEvents также проверит sf::Event::Closed)
        Input->PollEvents(Graphics->GetWindow());
        
        // Если Input сказал "выходим"
        if (Input->IsWillQuit())
        {
            break; // Выходим из цикла
        }

        // 2. СЧИТАЕМ ВРЕМЯ
        auto CurrentTick = Time::now();
        Seconds DeltaTime = CurrentTick - LastTick;
        LastTick = CurrentTick;

        // 3. ОБНОВЛЯЕМ ЛОГИКУ
        // (Вызываем EGame::OnUpdate -> World->Update -> все Системы)
        OnUpdate(DeltaTime.count());
        
        // std::cout << DeltaTime.count() << std::endl; // Раскомментируй для FPS

        // 4. РИСУЕМ
        Graphics->Begin();  // (window.clear)
        OnRender();         // (Вызываем EGame::OnRender -> S_RenderSystem)
        Graphics->End();    // (window.display)
    }

    return 0;
}