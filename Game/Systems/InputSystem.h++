// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "Systems/SystemBase.h++"
#include "ECS/ArchetypeManager.h++"
#include "ECS/ComponentsList.h++"
#include "SFML/Window/Keyboard.hpp"

class EInputSystem final : public ISystemBase
{
public:
    void Update(float InDeltaTime) override
    {
        if (!ArchetypeManager) return;

        // Ищем всех, у кого есть компонент ввода (обычно это только игрок)
        ArchetypeManager->Query<ECS::PlayerInput>(
            [](ECS::Entity e, ECS::PlayerInput& Input)
            {
                Input.MoveX = 0.0f;
                Input.MoveY = 0.0f;

                // SFML 2.x/3.x style real-time input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    Input.MoveY = -1.0f;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    Input.MoveY = 1.0f;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    Input.MoveX = -1.0f;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    Input.MoveX = 1.0f;
            }
        );
    }
};