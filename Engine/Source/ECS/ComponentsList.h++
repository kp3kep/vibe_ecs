// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Graphics/Texture.hpp"

namespace ECS
{
    struct Transform
    {
        float x;
        float y;
    };

    struct Velocity
    {
        float dx;
        float dy;
    };

    struct Health
    {
        float hp;
    };

    // 1. Хранит "намерения" игрока
    struct C_PlayerInput {
        float MoveX = 0.0f;
        float MoveY = 0.0f;
        bool IsAttackPressed = false;
    };

    struct C_Sprite {
        std::shared_ptr<sf::Texture> Texture;
        // Можно добавить: sf::IntRect TextureRect; (для анимации)
        // Можно добавить: float Scale;
    };
}