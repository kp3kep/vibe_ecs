// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Graphics.hpp"

// Наш класс-обертка для инпута
class FInput
{
public:
    FInput() : bShouldQuit(false) {}

    // IApplication будет вызывать это каждый кадр
    void PollEvents(sf::RenderWindow& InWindow)
    {
        bShouldQuit = false;

        // --- SFML 3.0.2 ИЗМЕНЕНИЕ ---
        // pollEvent теперь возвращает std::optional<sf::Event>
        while (const auto Event = InWindow.pollEvent())
        {
            // Проверка типа события теперь делается через .is<Тип>()
            if (Event->is<sf::Event::Closed>())
            {
                bShouldQuit = true;
            }

           if (const auto* keyPress = Event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPress->code == sf::Keyboard::Key::Escape)
                    bShouldQuit = true;
            }
        }
    }

    [[nodiscard]] bool IsWillQuit() const { return bShouldQuit; }

private:
    bool bShouldQuit;
};