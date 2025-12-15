// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include "RenderSystemBase.h++"
#include "Graphics.h++"
#include "ArchetypeManager.h++"
#include "ComponentsList.h++"

#include "SFML/Graphics/Sprite.hpp"

class ERenderSystem final : public IRenderSystemBase
{
public:
    void OnRender(FGraphics& Graphics, ECS::EArchetypeManager& ArchetypeManager) override
    {
        sf::RenderWindow& Window = Graphics.GetWindow();

        // Запрос: Рисуем все, у чего есть Трансформ и Спрайт
        ArchetypeManager.QuerySync<ECS::Transform, ECS::C_Sprite>(
            [&](ECS::Entity InEntity, const ECS::Transform& InTransform, const ECS::C_Sprite& InSprite)
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