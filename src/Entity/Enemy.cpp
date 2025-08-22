#include "Enemy.hpp"

#include <raylib.h>
#include <iostream>
#include "GameState/InGame.hpp"
#include "Entity/Player.hpp"

EntityEnemy::EntityEnemy() : Entity(EntityType::ENEMY)
{
    size = {20, 20};
    collider.size = {20, 20};
    weapon = WeaponPistol();
}

void EntityEnemy::Draw()
{
    Rectangle rect;
    // origin parameter shifts the position so we need to adjust
    rect.x = position.x + size.x / 2;
    rect.y = position.y + size.y / 2;
    rect.width = size.x;
    rect.height = size.y;

    Vector2 origin = {
        size.x / 2,
        size.y / 2
    };

    if (health > 0)
        DrawRectanglePro(rect, origin, rotation, RED);
    else
        DrawRectanglePro(rect, origin, rotation, GRAY);
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