// Copyright Vibe Coding. All Rights Reserved.

#include <chrono>
#include <cstdlib>
#include <iostream>

#include "Application.h++"
#include "SFML/Graphics.hpp"

IApplication::IApplication() : IsFailed(false)
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Vibe ECS Engine");

    constexpr sf::Color Japan = {150, 50, 50, 255};

    sf::CircleShape shape(100.f);
    shape.setFillColor(Japan);


    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event.value().is<sf::Event::Closed>())
            {
                window.close();
            }
        }


        window.clear();

        window.draw(shape);

        window.display();
    }
}

IApplication::~IApplication()
{
    // delete Input;
    // delete Graphics;
}

int IApplication::Run()
{
    if (IsFailed)
    {
        system("Pause");
        return 1;
    }

    using Time = std::chrono::high_resolution_clock;
    using Seconds = std::chrono::duration<float>;

    OnBegin();

    auto LastTick = Time::now();

    while (true /*!Input->IsWillQuit()*/)
    {
        // Input->Update();
        static uint32_t count = 0;
        if (++count > 5) break;

        auto CurrentTick = Time::now();
        Seconds DeltaTime = CurrentTick - LastTick;
        LastTick = CurrentTick;

        OnUpdate(DeltaTime.count());

        std::cout << DeltaTime.count() << std::endl;

        // Graphics->Begin();
        // OnRender();
        // Graphics->End();
    }

    return 0;
}