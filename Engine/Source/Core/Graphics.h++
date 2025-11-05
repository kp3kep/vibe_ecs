// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Graphics.hpp"

// Наш класс-обертка для графики
class FGraphics
{
public:
    // Конструктор СОЗДАЕТ окно
    FGraphics()
    {
        // Используем create, а не конструктор,
        // т.к. окно - член класса
        Window.create(sf::VideoMode(1024, 768), "Vibe ECS Engine (SFML 2.x)");
        Window.setFramerateLimit(600);
    }

    // Твой "Begin()"
    void Begin()
    {
        Window.clear(sf::Color(15, 15, 30)); // Темно-синий фон
    }

    // Твой "End()"
    void End()
    {
        Window.display();
    }

    // Функция, чтобы S_RenderSystem мог рисовать
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