#include "Player.hpp"

#include <raylib.h>
#include <iostream>

#include "GameState/InGame.hpp"

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
        DrawRectanglePro(rect, origin, rotation, BLUE);
    else
        DrawRectanglePro(rect, origin, rotation, GRAY);
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
        currentWeaponSlot = 0;
    if (IsKeyPressed(KEY_TWO))
        currentWeaponSlot = 1;
    if (IsKeyPressed(KEY_THREE))
        currentWeaponSlot = 2;
    if (IsKeyPressed(KEY_FOUR))
        currentWeaponSlot = 3;

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
}