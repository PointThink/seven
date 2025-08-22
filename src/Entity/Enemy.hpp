#pragma once

#include "Entity.hpp"
#include "Weapons.hpp"

#define REACTION_TIME 0.4

struct EntityEnemy : public Entity
{
    Vector motion;
    float rotation = 0;
    int health = 40;
    int i = 0;

    float playerSeenStart = 0;
    bool playerSeenLastFrame = false;

    Weapon weapon;

    EntityEnemy();

    void Update() override;
    void Draw() override;
};