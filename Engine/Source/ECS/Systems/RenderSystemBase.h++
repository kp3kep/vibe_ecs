// Copyright Vibe Coding. All Rights Reserved.

#pragma once

class FGraphics;
namespace ECS { class EArchetypeManager; }

/**
 * @brief Базовый класс для всех СИСТЕМ ОТРИСОВКИ.
 */
class IRenderSystemBase
{
public:
    virtual ~IRenderSystemBase() = default;

    /**
     * @brief Главный метод отрисовки.
     * @param Graphics - Наша обертка для SFML (чтобы знать, *где* рисовать)
     * @param ArchetypeManager - Наш ECS (чтобы знать, *что* рисовать)
     */
    virtual void OnRender(FGraphics& Graphics, ECS::EArchetypeManager& ArchetypeManager) = 0;
};