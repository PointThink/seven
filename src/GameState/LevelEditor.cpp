#include "LevelEditor.hpp"

#include <raylib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "Entity/Enemy.hpp"
#include "Entity/AmmoPickup.hpp"
#include "Entity/WeaponPickup.hpp"
#include "Entity/Player.hpp"

std::string GetEditorModeName(EditorMode mode)
{
    switch (mode)
    {
    case EditorMode::WALL:
        return "Wall";
    case EditorMode::ENEMY:
        return "Enemy";
    case EditorMode::WEAPON_PICKUP:
        return "Weapon";
    case EditorMode::AMMO_PICKUP:
        return "Ammo pickup";
    case EditorMode::PLAYER:
        return "Player";
    case EditorMode::DELETE:
        return "Delete";
    }

    return "Unknown";
}

LevelEditorState::LevelEditorState()
{
    world.LoadFromFile("level.txt");
}

void LevelEditorState::Draw()
{
    Camera2D camera = {0};
    camera.zoom = zoom;

    BeginMode2D(camera);
    world.Draw();
    EndMode2D();

    DrawText(GetEditorModeName(mode).c_str(), 10, 10, 30, GRAY);

    if (mode == EditorMode::AMMO_PICKUP)
    {
        DrawText(ammoPickupTypes[currentAmmoPickupTypeIndex].c_str(), 10, 50, 30, GRAY);
    }

    if (mode == EditorMode::WEAPON_PICKUP)
    {
        DrawText(weaponPickupTypes[currentWeaponPickupTypeIndex].c_str(), 10, 50, 30, GRAY);
    }
}

void LevelEditorState::Update()
{
    Vector mousePos = {
        GetMousePosition().x / zoom,
        GetMousePosition().y / zoom
    };

    if (mode == EditorMode::WALL)
    {
        int gridX = (mousePos.x) / CELL_SIZE;
        int gridY = (mousePos.y) / CELL_SIZE;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            world.wallGrid->SetCell(gridX, gridY, true);
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            world.wallGrid->SetCell(gridX, gridY, false);
    }

    if (mode == EditorMode::ENEMY)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityEnemy* enemy = new EntityEnemy;
            enemy->position = {mousePos.x, mousePos.y};
            world.entites.push_back(enemy);
        }
    }

    if (mode == EditorMode::PLAYER)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityPlayer* player = new EntityPlayer;
            player->position = {mousePos.x, mousePos.y};
            world.entites.push_back(player);
        }
    }

    if (mode == EditorMode::AMMO_PICKUP)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityAmmoPickup* pickup = new EntityAmmoPickup;
            pickup->type = ammoPickupTypes[currentAmmoPickupTypeIndex];
            pickup->position = mousePos;
            world.entites.push_back(pickup);
        }

        int wheelMove = GetMouseWheelMove();
        currentAmmoPickupTypeIndex += wheelMove;

        if (currentAmmoPickupTypeIndex > ammoPickupTypes.size() - 1)
            currentAmmoPickupTypeIndex = 0;
        else if (currentAmmoPickupTypeIndex < 0)
            currentAmmoPickupTypeIndex = ammoPickupTypes.size() - 1;
    }

    if (mode == EditorMode::WEAPON_PICKUP)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityWeaponPickup* pickup = new EntityWeaponPickup;
            pickup->type = weaponPickupTypes[currentWeaponPickupTypeIndex];
            pickup->position = mousePos;
            world.entites.push_back(pickup);
        }

        int wheelMove = GetMouseWheelMove();
        currentWeaponPickupTypeIndex += wheelMove;

        if (currentWeaponPickupTypeIndex > weaponPickupTypes.size() - 1)
            currentWeaponPickupTypeIndex = 0;
        else if (currentWeaponPickupTypeIndex < 0)
            currentWeaponPickupTypeIndex = weaponPickupTypes.size() - 1;
    }

    if (mode == EditorMode::DELETE)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (Entity* entity : world.entites)
            {
                entity->collider.position = entity->position;

                if (
                    mousePos.x > entity->collider.position.x && mousePos.y > entity->collider.position.y &&
                    mousePos.x < entity->collider.position.x + entity->collider.size.x && mousePos.y < entity->collider.position.y + entity->collider.size.y
                )
                {
                    world.entites.erase(std::find(world.entites.begin(), world.entites.end(), entity));
                }
            }
        }
    }

    if (IsKeyPressed(KEY_KP_ADD))
        zoom++;
    if (IsKeyPressed(KEY_KP_SUBTRACT))
        zoom--;

    if (IsKeyPressed(KEY_F1))
        mode = EditorMode::WALL;
    if (IsKeyPressed(KEY_F2))
        mode = EditorMode::PLAYER;
    if (IsKeyPressed(KEY_F3))
        mode = EditorMode::ENEMY;
    if (IsKeyPressed(KEY_F4))
        mode = EditorMode::WEAPON_PICKUP;
    if (IsKeyPressed(KEY_F5))
        mode = EditorMode::AMMO_PICKUP;
    if (IsKeyPressed(KEY_F6))
        mode = EditorMode::DELETE;

    if (IsKeyPressed(KEY_F7) || IsKeyPressed(KEY_F8))
        world.ExportToFile("level.txt");

    if (IsKeyPressed(KEY_F8))
        system("./build/seven run level.txt");
}