#include "InGame.hpp"


#include <stdio.h>
#include "Entity/Player.hpp"
#include "Global.hpp"
#include "MainMenu.hpp"

static void DrawBillboard(std::string bigText, std::string smallText, Color color)
{
    int bigTextWidth = MeasureTextEx(font, bigText.c_str(), 60, 0).x;
    int smallTextWidth = MeasureTextEx(font, smallText.c_str(), 30, 0).x;

    int bigTextPosX = GetScreenWidth() / 2 - bigTextWidth / 2;
    int smallTextPosX = GetScreenWidth() / 2 - smallTextWidth / 2;

    DrawRectangle(smallTextPosX - 20, 40, smallTextWidth + 40, 40 + 20 + 40, color);

    DrawTextEx(font, bigText.c_str(), {(float) bigTextPosX, 50}, 60, 0, BLACK);
    DrawTextEx(font, smallText.c_str(), {(float) smallTextPosX, 90}, 30, 0, BLACK);
}

InGameState::InGameState()
{
    world.LoadFromFile("level.txt");
    camera.zoom = 3;
    camera.offset = {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2};
}

void InGameState::Draw()
{
    Vector2 mousePos = GetMousePosition();

    Vector2 target = {
        player->position.x + player->size.x / 2 + mousePos.x / 6 - GetScreenWidth() / 12,
        player->position.y + player->size.y / 2 + mousePos.y / 6 - GetScreenHeight() / 12
    };

    camera.target = target;

    BeginMode2D(camera);

    world.Draw();

    if (player->health > 0)
        player->Draw(); // render player above everything else if he's alive

    EndMode2D();

    // draw hud
    
    int rectangleWidth = 200;
    int rectangleHeight = 100;
    int padding = 4;
    int margin = 20;

    // background
    DrawRectangle(20, GetScreenHeight() - margin - rectangleHeight, rectangleWidth, rectangleHeight, {54, 55, 77, 180});

    // healthbar
    int healthbarHeight = 10;
    int startPosX = margin + padding;
    int startPosY = GetScreenHeight() - margin - padding - healthbarHeight;

    int fullWidth = rectangleWidth - padding * 2;
    int maxHealth = 100;

    int actualWidth = fullWidth * ((float) player->health / (float) maxHealth);

    DrawRectangle(startPosX, startPosY, actualWidth, healthbarHeight, {235, 23, 52, 255});

    // weapon indicators
    int index = 0;
    int indicatorSpacing = 4;
    int indicatorHeight = 5;

    for (Weapon* weapon : player->weapons)
    {
        int indicatorWidth = (rectangleWidth - padding * 2 - indicatorSpacing * 3) / 4;
        int indicatorPosX = margin + padding + ((indicatorWidth + indicatorSpacing) * index);
        int indicatorPosY = GetScreenHeight() - margin - padding * 2 - healthbarHeight - indicatorHeight;

        if (weapon != nullptr)
            DrawRectangle(indicatorPosX, indicatorPosY, indicatorWidth, indicatorHeight, {23, 118, 235, 255});
        else
            DrawRectangle(indicatorPosX, indicatorPosY, indicatorWidth, indicatorHeight, {130, 148, 171, 255});

        index++;
    }

    int ammoSpaceX =  20;
    int ammoSpaceY = GetScreenHeight() - margin - rectangleHeight;
    int ammoSpaceWidth = rectangleWidth;
    int ammoSpaceHeight = rectangleHeight - padding * 3 - healthbarHeight - indicatorHeight;

    // DrawRectangle(ammoSpaceX, ammoSpaceY, ammoSpaceWidth, ammoSpaceHeight, BLACK);

    // Draw ammo count
    int currentAmmo = player->weapons[player->currentWeaponSlot]->GetCurrentAmmo();
    int reserveAmmo = player->ammoPool.GetAmmoCount(player->weapons[player->currentWeaponSlot]->GetAmmoType());

    Vector textSize = {
        MeasureTextEx(font, TextFormat("%i/%i", currentAmmo, reserveAmmo), 60, 0).x,
        50
    };

    int ammoPosX = ammoSpaceX + ammoSpaceWidth / 2 - textSize.x / 2;
    int ammoPosY = ammoSpaceY + ammoSpaceHeight / 2 - textSize.y / 2;

    DrawTextEx(font, TextFormat("%i/%i", currentAmmo, reserveAmmo), {ammoPosX, ammoPosY}, 60, 0, WHITE);

    if (player->health <= 0)
    {
        std::string bigText = "You died!";
        std::string smallText = "Press enter to restart the level or escape to exit to main menu";
        
        DrawBillboard(bigText, smallText, {235, 23, 52, 255});
    }
    else if (world.GetNumberOfAliveEnemies() == 0)
    {
        std::string bigText = "Area clear";
        std::string smallText = "Press enter to to return to menu";
        
        DrawBillboard(bigText, smallText, {122, 173, 255, 255});
    }
}

void InGameState::Update()
{
    // find player
    for (Entity* entity : world.entities)
    {
        if (entity->GetType() == EntityType::PLAYER)
            player = (EntityPlayer*) entity;
    }

    world.Update();

    if (player->health <= 0)
    {
        if (IsKeyPressed(KEY_ESCAPE))
            GameStateManager::SetState(new MainMenu);
        if (IsKeyPressed(KEY_ENTER))
            Restart();
    }
    else if (world.GetNumberOfAliveEnemies() == 0)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            MainMenu* menu = new MainMenu();
            menu->state = MENU_LEVEL_SELECT;
            GameStateManager::SetState(menu);
        }
    }
}

void InGameState::Restart()
{
    world.LoadFromFile(world.file);
}