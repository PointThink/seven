#include "Player.hpp"

#include <raylib.h>
#include <iostream>

#include "GameState/InGame.hpp"
#include "SoundManager.hpp"
#include "TextureManager.h"

EntityPlayer::EntityPlayer() : Entity(EntityType::PLAYER)
{
    weapons[0] = new WeaponPistol();
    weapons[0]->SetCurrentAmmo(weapons[0]->GetMaxAmmo());

    for (int i = 1; i < 4; i++)
        weapons[i] = nullptr;

    size = {20, 20};
    collider.size = size;
}

EntityPlayer::~EntityPlayer()
{
    for (Weapon* weapon : weapons)
        delete weapon;
}

void EntityPlayer::Draw()
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
    {
        if (weapons[currentWeaponSlot]->GetType() == WeaponType::PISTOL)
        {
            Rectangle src = {0, 0, 30, 23};
            Rectangle dest = {position.x + 6, position.y + 11, 30, 23};
            Vector2 origin = {6, 11};

            DrawTexturePro(TextureManager::Get("player_pistol"), src, dest, origin, rotation, WHITE);
        }
        else if (weapons[currentWeaponSlot]->GetType() == WeaponType::RIFLE)
        {
            Rectangle src = {0, 0, 45, 25};
            Rectangle dest = {position.x + 8, position.y + 13, 45, 25};
            Vector2 origin = {8, 13};

            DrawTexturePro(TextureManager::Get("player_rifle"), src, dest, origin, rotation, WHITE);
        }
        else if (weapons[currentWeaponSlot]->GetType() == WeaponType::SMG)
        {
            Rectangle src = {0, 0, 37, 25};
            Rectangle dest = {position.x + 8, position.y + 13, 37, 25};
            Vector2 origin = {8, 13};

            DrawTexturePro(TextureManager::Get("player_smg"), src, dest, origin, rotation, WHITE);
        }
        else if (weapons[currentWeaponSlot]->GetType() == WeaponType::SHOTGUN)
        {
            Rectangle src = {0, 0, 45, 25};
            Rectangle dest = {position.x + 8, position.y + 13, 45, 25};
            Vector2 origin = {8, 13};

            DrawTexturePro(TextureManager::Get("player_shotgun"), src, dest, origin, rotation, WHITE);
        }
    }
    else
    {
        Rectangle src = {0, 0, 75, 40};
        Rectangle dest = {position.x + 8, position.y + 13, 75, 40};
        Vector2 origin = {8, 13};

        DrawTexturePro(TextureManager::Get("player_dead"), src, dest, origin, rotation, WHITE);
    }
}

void EntityPlayer::Update()
{
    if (health <= 0) return;

    InGameState* state = (InGameState*) GameStateManager::GetState();

    motion = {0, 0};

    if (IsKeyDown(KEY_W))
        motion.y -= 100;
    if (IsKeyDown(KEY_S))
        motion.y += 100;
    if (IsKeyDown(KEY_A))
        motion.x -= 100;
    if (IsKeyDown(KEY_D))
        motion.x += 100;

    if (IsKeyPressed(KEY_ONE))
    {
        if (weapons[0] != nullptr)
            currentWeaponSlot = 0;
        else
            SoundManager::Play("denied");
    }

    if (IsKeyPressed(KEY_TWO))
    {
        if (weapons[1] != nullptr)
            currentWeaponSlot = 1;
        else
            SoundManager::Play("denied");
    }

    if (IsKeyPressed(KEY_THREE))
    {
        if (weapons[2] != nullptr)
            currentWeaponSlot = 2;
        else
            SoundManager::Play("denied");
    }

    if (IsKeyPressed(KEY_FOUR))
    {
        if (weapons[3] != nullptr)
            currentWeaponSlot = 3;
        else
            SoundManager::Play("denied");
    }

    if (IsKeyPressed(KEY_R))
        weapons[currentWeaponSlot]->Reload(&ammoPool);

    position.x += motion.x * GetFrameTime();
    position.y += motion.y * GetFrameTime();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        weapons[currentWeaponSlot]->Fire(this, rotation, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
    }

    Vector center = {
        position.x + size.x / 2,
        position.y + size.y / 2
    };

    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), state->camera);

    // convert to my vector type
    Vector mousePos = {
        mouseWorldPos.x,
        mouseWorldPos.y
    };

    rotation = GetDirection(center, mousePos);

    weapons[currentWeaponSlot]->Update(this, &ammoPool);

    if (weapons[currentWeaponSlot]->GetCurrentAmmo() == 0)
        weapons[currentWeaponSlot]->Reload(&ammoPool);
}