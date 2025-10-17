#include "ArchetypeManager.h++"

void EArchetypeManager::RemoveEntity(Entity e)
{
    if (!EntitySignatures.contains(e))
    {
        return;
    }

    const uint64_t sig = EntitySignatures[e];
    auto it = Archetypes.find(sig);

    if (it != Archetypes.end())
    {
        it->second->Remove(e);
    }
    EntitySignatures.erase(e);
}