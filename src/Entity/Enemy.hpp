#pragma once

#include "Entity.hpp"
#include "Weapons.hpp"

#define MAX_ROTATION_SPEED 180

struct EntityEnemy : public Entity
{
    Vector motion;
    int health = 40;
    int i = 0;

    float targetRotation = 0;
    Vector lastSeenPosition;
    bool lastSeenActive = false;

    float playerSeenStart = 0;
    float reactionTime = 0.4;
    bool playerSeenLastFrame = false;

    Weapon weapon;

    EntityEnemy();

    void Update() override;
    void Draw() override;
};