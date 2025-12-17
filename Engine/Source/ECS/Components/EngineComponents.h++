// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "SFML/Graphics/Texture.hpp"

namespace ECS
{
    struct CTransform
    {
        float x;
        float y;
    };

    struct CVelocity
    {
        float dx;
        float dy;
    };

    struct CHealth
    {
        float hp;
    };

    // 1. Хранит "намерения" игрока
    struct CPlayerInput
    {
        float MoveX = 0.0f;
        float MoveY = 0.0f;
        bool IsAttackPressed = false;
    };

    struct CAIInput
    {
        float MoveX = 0.0f;
        float MoveY = 0.0f;
        float TempTimer = 0.0f;
        bool IsAttackPressed = false;
    };

    struct CSprite
    {
        std::shared_ptr<sf::Texture> Texture;
        // Можно добавить: sf::IntRect TextureRect; (для анимации)
        // Можно добавить: float Scale;
    };
}