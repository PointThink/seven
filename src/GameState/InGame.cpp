#include "InGame.hpp"


#include <stdio.h>
#include "Entity/Player.hpp"

InGameState::InGameState()
{
    world.LoadFromFile("level.txt");
    camera.zoom = 3;
    camera.offset = {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2};
}

void InGameState::Draw()
{
    Vector2 target = {
        player->position.x + player->size.x / 2,
        player->position.y + player->size.y / 2
    };

    camera.target = target;

    BeginMode2D(camera);

    world.Draw();

    EndMode2D();

    // draw hud
    int currentAmmo = player->weapons[player->currentWeaponSlot]->GetCurrentAmmo();
    int reserveAmmo = player->ammoPool.GetAmmoCount(player->weapons[player->currentWeaponSlot]->GetAmmoType());
    DrawText(TextFormat("%i/%i", currentAmmo, reserveAmmo), 10, 10, 30, BLACK);
}

void InGameState::Update()
{
    // find player
    for (Entity* entity : world.entites)
    {
        if (entity->GetType() == EntityType::PLAYER)
            player = (EntityPlayer*) entity;
    }

    world.Update();
}