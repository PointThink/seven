#pragma once

#include "Entity.hpp"
#include "Weapons.hpp"

struct EntityPlayer : public Entity
{
    Vector motion;
    int health = 100;

    Weapon* weapons[4];
    int currentWeaponSlot = 0;

    AmmoPool ammoPool;

    EntityPlayer();
    ~EntityPlayer();

    void Update() override;
    void Draw() override;
};