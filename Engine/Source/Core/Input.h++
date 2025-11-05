// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

// Наш класс-обертка для инпута, как ты и хотел
class FInput
{
public:
    FInput() : bShouldQuit(false) {}

    // IApplication будет вызывать это каждый кадр
    void PollEvents(sf::RenderWindow& InWindow)
    {
        bShouldQuit = false;
        
        // В SFML 2.x используем этот цикл
        sf::Event event;
        while (InWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                bShouldQuit = true;
            }

            // TODO: Здесь мы будем заполнять наш C_InputComponent
            // (пока просто закрываем окно)
        }
    }

    bool IsWillQuit() const { return bShouldQuit; }

private:
    bool bShouldQuit;
};