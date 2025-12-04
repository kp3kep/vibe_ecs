// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Graphics.hpp"

class FGraphics
{
public:
    FGraphics()
    {
        Window.create(sf::VideoMode({800, 600}), "Vibe ECS Engine");

        Window.setFramerateLimit(60);
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