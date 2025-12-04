// Copyright Vibe Coding. All Rights Reserved.

#include "World.h++"

#include "ComponentsList.h++"
#include "HeathSystem.h++"
#include "InputSystem.h++"
#include "MovementSystem.h++"
#include "PlayerControlSystem.h++"
#include "RenderSystem.h++"
#include "ResourceManager.h++"
#include "VelocitySystem.h++"

EWorld::EWorld()
{
    SystemManager.SetPool(&ThreadPool);
    ArchetypeManager.SetPool(&ThreadPool);

    RegisterSystem<EHeathSystem>();
    RegisterSystem<EInputSystem>();
    RegisterSystem<EMovementSystem>();
    RegisterSystem<EPlayerControlSystem>();
    RegisterSystem<EVelocitySystem>();

    SystemManager.RegisterRenderSystem<ERenderSystem>();
}

void EWorld::Initialize()
{
    // StressTest();
    std::shared_ptr<sf::Texture> HeroTexture = EResourceManager::Get().LoadTexture("hero.png");

    if (!HeroTexture)
    {
        HeroTexture = std::make_shared<sf::Texture>();
        auto size = HeroTexture->resize({32, 32});
        std::vector<uint8_t> pixels(32 * 32 * 4, 255);
        for (size_t i = 0; i < pixels.size(); i += 4)
        {
            pixels[i + 1] = 0;
        }
        HeroTexture->update(pixels.data());
    }


    // --- СОЗДАНИЕ ИГРОКА ---
    const ECS::Entity Player = CreateEntity();
    ArchetypeManager.RegisterEntity(Player);

    ArchetypeManager.AddComponent(Player, ECS::Transform{100.f, 100.f});
    ArchetypeManager.AddComponent(Player, ECS::Velocity{0.f, 0.f});
    ArchetypeManager.AddComponent(Player, ECS::C_PlayerInput{0,0,false});
    ArchetypeManager.AddComponent(Player, ECS::C_Sprite{ HeroTexture });
}

ECS::Entity EWorld::CreateEntity()
{
    return EntityManager.CreateEntity();
}

void EWorld::DestroyEntity(ECS::Entity EntityID)
{
    ArchetypeManager.UnregisterEntity(EntityID);
}

void EWorld::Update(float InDeltaTime) const
{
    SystemManager.Update(InDeltaTime);
}

void EWorld::Render(FGraphics& Graphics) const
{
    SystemManager.Render(Graphics, const_cast<ECS::EArchetypeManager&>(ArchetypeManager));
}

void EWorld::StressTest()
{
    constexpr int ENTITY_COUNT = 1'000'000;

    for (int i = 0; i < ENTITY_COUNT; ++i)
    {
        const ECS::Entity NewEntity = CreateEntity();
        ArchetypeManager.RegisterEntity(NewEntity);

        // Даем позицию
        ArchetypeManager.AddComponent(NewEntity, ECS::Transform{
                                          static_cast<float>(i % 1000), // Просто какие-то значения
                                          static_cast<float>(i) / 1000
                                      });

        // Даем скорость (чтобы система их двигала)
        ArchetypeManager.AddComponent(NewEntity, ECS::Velocity{ 1.0f, 0.5f });

        if (i % 2)
        {
            ArchetypeManager.AddComponent(NewEntity, ECS::Health{ 100 });
        }
        if (i % 3)
        {
            ArchetypeManager.AddComponent(NewEntity, ECS::C_Sprite{});
        }
    }
}