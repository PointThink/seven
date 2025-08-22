#pragma once

#include <iostream>

#include "Entity.hpp"

struct EntityWeaponPickup : public Entity
{
    std::string type;

    EntityWeaponPickup();

    void Draw() override;
    void Update() override;

    void OnEntityCollide(CollisionInfo info, Entity* otherEntity) override;
};