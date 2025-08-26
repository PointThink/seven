#include "Enemy.hpp"

#include <raylib.h>
#include <iostream>
#include "GameState/InGame.hpp"
#include "Entity/Player.hpp"
#include "TextureManager.h"

EntityEnemy::EntityEnemy() : Entity(EntityType::ENEMY)
{
    size = {20, 20};
    collider.size = {20, 20};
    weapon = WeaponPistol();
}

void EntityEnemy::Draw()
{
    if (health > 0)
    {
        Rectangle src = {0, 0, 45, 25};
        Rectangle dest = {position.x + 33 / 2, position.y + 19 / 2, 45, 25};
        Vector2 origin = {8, 13};

        DrawTexturePro(TextureManager::Get("grunt"), src, dest, origin, rotation, WHITE);
    }
    else
    {
        Rectangle src = {0, 0, 75, 40};
        Rectangle dest = {position.x - 33, position.y - 19, 75, 40};
        Vector2 origin = {33, 19};

        DrawTexturePro(TextureManager::Get("grunt_dead"), src, dest, origin, rotation, WHITE);
    }
}

void EntityEnemy::Update()
{
    if (health <= 0)
        return;

    // check line of sight to player
    InGameState* state = (InGameState*) GameStateManager::GetState();
    Vector playerCenter = state->GetPlayer()->GetCenter();

    if (weapon.GetCurrentAmmo() <= 0)
        weapon.Reload(nullptr);
    

    
    if (state->world.HasLineOfSight(GetCenter(), playerCenter))
    {
        if (!playerSeenLastFrame)
        {
            playerSeenStart = GetTime();
            playerSeenLastFrame = true;
        }
        else
        {
            if (playerSeenStart + REACTION_TIME < GetTime())
            {
                rotation = GetDirection(GetCenter(), playerCenter);
                weapon.Fire(this, rotation, true);
            }
        }
    }
    else
    {
        playerSeenLastFrame = false;
    }

    weapon.Update(this, nullptr);
}