#include "LevelEditor.hpp"

#include <raylib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "Entity/Enemy.hpp"
#include "Entity/AmmoPickup.hpp"
#include "Entity/WeaponPickup.hpp"
#include "Entity/Player.hpp"
#include "TextureManager.h"
#include "Global.hpp"

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
    case EditorMode::FLOOR:
        return "Floor";
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
    camera.offset = {position.x, position.y};

    Vector2 mouseWorldPosRl = GetScreenToWorld2D(GetMousePosition(), camera);
    mousePos = { mouseWorldPosRl.x, mouseWorldPosRl.y };

    BeginMode2D(camera);
    world.Draw();

    if (mode == EditorMode::ENEMY)
    {
        float length = 100;
        float tangents = enemyRotation * 0.0174532925;
        float offsetX = cos(tangents) * length;
        float offsetY = sin(tangents) * length;

        DrawLine(mousePos.x, mousePos.y, mousePos.x + offsetX, mousePos.y + offsetY, BLUE);
    }

    EndMode2D();

    DrawTextEx(font, GetEditorModeName(mode).c_str(), {10, 10}, 60, 0, WHITE);

    if (mode == EditorMode::AMMO_PICKUP)
    {
        DrawTextEx(font, ammoPickupTypes[currentAmmoPickupTypeIndex].c_str(), {10, 50}, 40, 0, WHITE);
    }

    else if (mode == EditorMode::WEAPON_PICKUP)
    {
       DrawTextEx(font, weaponPickupTypes[currentWeaponPickupTypeIndex].c_str(), {10, 50}, 40, 0, WHITE);
    }
    else if (mode == EditorMode::FLOOR)
    {
        DrawTextureEx(
            TextureManager::Get(floorTextures[currentFloorTextureIndex]),
            {0, 50},
            0, 3, WHITE
        );
    }
}

void LevelEditorState::Update()
{
    if (IsKeyDown(KEY_W))
        position.y += 300 * GetFrameTime();
    if (IsKeyDown(KEY_S))
        position.y -= 300 * GetFrameTime();
    if (IsKeyDown(KEY_A))
        position.x += 300 * GetFrameTime();
    if (IsKeyDown(KEY_D))
        position.x -= 300 * GetFrameTime();

    if (mode == EditorMode::WALL)
    {
        int gridX = (mousePos.x) / CELL_SIZE;
        int gridY = (mousePos.y) / CELL_SIZE;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            world.wallGrid->SetCell(gridX, gridY, true);
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            world.wallGrid->SetCell(gridX, gridY, false);
    }
    else if (mode == EditorMode::ENEMY)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityEnemy* enemy = new EntityEnemy;
            enemy->position = {mousePos.x, mousePos.y};
            enemy->rotation = enemyRotation;
            world.entities.push_back(enemy);
        }

        enemyRotation += GetMouseWheelMove() * 11.25;
    }
    else if (mode == EditorMode::PLAYER)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityPlayer* player = new EntityPlayer;
            player->position = {mousePos.x, mousePos.y};
            world.entities.push_back(player);
        }
    }
    else if (mode == EditorMode::AMMO_PICKUP)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityAmmoPickup* pickup = new EntityAmmoPickup;
            pickup->type = ammoPickupTypes[currentAmmoPickupTypeIndex];
            pickup->position = mousePos;
            world.entities.push_back(pickup);
        }

        int wheelMove = GetMouseWheelMove();
        currentAmmoPickupTypeIndex += wheelMove;

        if (currentAmmoPickupTypeIndex > ammoPickupTypes.size() - 1)
            currentAmmoPickupTypeIndex = 0;
        else if (currentAmmoPickupTypeIndex < 0)
            currentAmmoPickupTypeIndex = ammoPickupTypes.size() - 1;
    }
    else if (mode == EditorMode::WEAPON_PICKUP)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            EntityWeaponPickup* pickup = new EntityWeaponPickup;
            pickup->type = weaponPickupTypes[currentWeaponPickupTypeIndex];
            pickup->position = mousePos;
            world.entities.push_back(pickup);
        }

        int wheelMove = GetMouseWheelMove();
        currentWeaponPickupTypeIndex += wheelMove;

        if (currentWeaponPickupTypeIndex > weaponPickupTypes.size() - 1)
            currentWeaponPickupTypeIndex = 0;
        else if (currentWeaponPickupTypeIndex < 0)
            currentWeaponPickupTypeIndex = weaponPickupTypes.size() - 1;
    }
    else if (mode == EditorMode::DELETE)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (Entity* entity : world.entities)
            {
                entity->collider.position = entity->position;

                if (
                    mousePos.x > entity->collider.position.x && mousePos.y > entity->collider.position.y &&
                    mousePos.x < entity->collider.position.x + entity->collider.size.x && mousePos.y < entity->collider.position.y + entity->collider.size.y
                )
                {
                    world.entities.erase(std::find(world.entities.begin(), world.entities.end(), entity));
                }
            }
        }
    }
    else if (mode == EditorMode::FLOOR)
    {
        int gridX = (mousePos.x) / CELL_SIZE;
        int gridY = (mousePos.y) / CELL_SIZE;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            world.floorGrid->SetCell(gridX, gridY, floorTextures[currentFloorTextureIndex]);
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            world.floorGrid->SetCell(gridX, gridY, "floor_grass");

        int wheelMove = GetMouseWheelMove();
        currentFloorTextureIndex += wheelMove;

        if (currentFloorTextureIndex > floorTextures.size() - 1)
            currentFloorTextureIndex = 0;
        else if (currentFloorTextureIndex < 0)
            currentFloorTextureIndex = floorTextures.size() - 1;
    }

    if (IsKeyPressed(KEY_KP_ADD))
        zoom++;
    if (IsKeyPressed(KEY_KP_SUBTRACT))
        zoom--;

    if (IsKeyPressed(KEY_F1))
        mode = EditorMode::WALL;
    if (IsKeyPressed(KEY_F2))
        mode = EditorMode::FLOOR;
    if (IsKeyPressed(KEY_F3))
        mode = EditorMode::PLAYER;
    if (IsKeyPressed(KEY_F4))
        mode = EditorMode::ENEMY;
    if (IsKeyPressed(KEY_F5))
        mode = EditorMode::WEAPON_PICKUP;
    if (IsKeyPressed(KEY_F6))
        mode = EditorMode::AMMO_PICKUP;
    if (IsKeyPressed(KEY_F7))
        mode = EditorMode::DELETE;

    if (IsKeyPressed(KEY_F8) || IsKeyPressed(KEY_F9))
        world.ExportToFile(world.file);

    if (IsKeyPressed(KEY_F8))
        system(std::string("./build/seven run " + world.file).c_str());
}