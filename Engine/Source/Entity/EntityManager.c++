// Copyright Vibe Coding. All Rights Reserved.

#include "EntityManager.h++"

Entity EEntityManager::CreateEntity()
{
    return ++LastEntityID;
}