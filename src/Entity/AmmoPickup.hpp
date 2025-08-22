#pragma once

#include "Entity.hpp"

#include <unordered_map>
#include <string>

struct EntityAmmoPickup : public Entity
{
    std::string type;

    EntityAmmoPickup();

    void Draw() override;
    void Update() override;
    void OnEntityCollide(CollisionInfo info, Entity* otherEntity);
};