// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

/**
 * @brief Менеджер Ресурсов (Паттерн Singleton).
 * Отвечает за загрузку и хранение "тяжелых" ресурсов (Текстур, Шейдеров, Звуков).
 * Гарантирует, что каждая текстура загружена в память ровно один раз.
 */
class EResourceManager
{
public:
    // 1. Точка доступа (Singleton)
    static EResourceManager& Get()
    {
        static EResourceManager Instance;
        return Instance;
    }

    // Удаляем копирование (чтобы случайно не создать клона)
    EResourceManager(const EResourceManager&) = delete;
    void operator=(const EResourceManager&) = delete;

    /**
     * @brief Загружает текстуру или возвращает уже загруженную.
     */
    std::shared_ptr<sf::Texture> LoadTexture(const std::string& Path)
    {
        // Проверяем кэш
        auto It = Textures.find(Path);
        if (It != Textures.end())
        {
            return It->second;
        }

        // Загружаем новую
        auto NewTexture = std::make_shared<sf::Texture>();
        if (NewTexture->loadFromFile(Path))
        {
            std::cout << "[Resource] Loaded texture: " << Path << std::endl;
            Textures[Path] = NewTexture;
            return NewTexture;
        }
        else
        {
            std::cerr << "[Resource] FAILED to load texture: " << Path << std::endl;
            // Возвращаем пустую (или можно дефолтную "error texture")
            return nullptr;
        }
    }

private:
    EResourceManager() = default; // Приватный конструктор

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> Textures;
};