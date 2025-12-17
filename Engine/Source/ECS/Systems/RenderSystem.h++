// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "RenderSystemBase.h++"
#include "Graphics.h++"
#include "ArchetypeManager.h++"
#include "EngineComponents.h++"

#include "SFML/Graphics/Sprite.hpp"

class SRenderSystem final : public IRenderSystemBase
{
public:
    void OnRender(FGraphics& Graphics, ECS::FArchetypeManager& ArchetypeManager) override
    {
        sf::RenderWindow& Window = Graphics.GetWindow();

        // Запрос: Рисуем все, у чего есть Трансформ и Спрайт
        ArchetypeManager.QuerySync<ECS::CTransform, ECS::CSprite>(
            [&](ECS::Entity InEntity, const ECS::CTransform& InTransform, const ECS::CSprite& InSprite)
            {
                // Если текстура загружена - рисуем
                if (InSprite.Texture)
                {
                    // Создаем легковесный спрайт на стеке
                    sf::Sprite Sprite(*InSprite.Texture);

                    // Центрируем спрайт (опционально)
                    // auto size = s.Texture->getSize();
                    // Sprite.setOrigin(size.x / 2.f, size.y / 2.f);

                    Sprite.setPosition({InTransform.x, InTransform.y});
                    Window.draw(Sprite);
                }
            }
        );
    }
};