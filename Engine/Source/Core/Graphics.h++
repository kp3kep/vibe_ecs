// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Graphics.hpp"

class FGraphics
{
public:
    FGraphics()
    {
        Window.create(sf::VideoMode({1024, 768}), "Vibe ECS Engine");

        Window.setFramerateLimit(100);
    }

    void Begin()
    {
        Window.clear(sf::Color::Black); // Классический черный фон
    }

    void End()
    {
        Window.display();
    }

    sf::RenderWindow& GetWindow()
    {
        return Window;
    }

    bool IsOpen() const
    {
        return Window.isOpen();
    }

private:
    sf::RenderWindow Window;
};