#include <raylib.h>
#include <iostream>

#include "GameState/GameState.hpp"
#include "GameState/InGame.hpp"
#include "GameState/LevelEditor.hpp"
#include "TextureManager.h"
#include "Global.hpp"
#include "GameState/MainMenu.hpp"
#include "SoundManager.hpp"

int main(int argc, char** argv)
{
    InitWindow(1600, 900, "Seven");
    InitAudioDevice();
    
    font = LoadFontEx("assets/Saira-Regular.ttf", 60, NULL, 0);
    fontBold = LoadFontEx("assets/Saira-Bold.ttf", 60, NULL, 0);

    srand(time(0));

    TextureManager::Add("pistol", "assets/textures/pistol.png");
    TextureManager::Add("shotgun", "assets/textures/shotgun.png");
    TextureManager::Add("smg", "assets/textures/smg.png");
    TextureManager::Add("rifle", "assets/textures/rifle.png");

    TextureManager::Add("grunt", "assets/textures/grunt.png");
    TextureManager::Add("grunt_dead", "assets/textures/grunt_dead.png");

    TextureManager::Add("floor_grass", "assets/textures/floors/grass.png");
    TextureManager::Add("floor_wood_dark", "assets/textures/floors/wood_dark.png");
    TextureManager::Add("floor_wood_light", "assets/textures/floors/wood_light.png");
    TextureManager::Add("floor_kitchen", "assets/textures/floors/kitchen.png");
    TextureManager::Add("floor_pavement1", "assets/textures/floors/pavement1.png");
    TextureManager::Add("floor_pavement2", "assets/textures/floors/pavement2.png");

    TextureManager::Add("player_pistol", "assets/textures/player/player_pistol.png");
    TextureManager::Add("player_smg", "assets/textures/player/player_smg.png");
    TextureManager::Add("player_rifle", "assets/textures/player/player_rifle.png");
    TextureManager::Add("player_shotgun", "assets/textures/player/player_shotgun.png");
    TextureManager::Add("player_dead", "assets/textures/player/player_dead.png");

    SoundManager::Add("click", "assets/sounds/click.wav");
    SoundManager::Add("hover", "assets/sounds/hover.wav");
    SoundManager::Add("denied", "assets/sounds/denied.wav");

    SoundManager::Add("weapon_pistol_fire", "assets/sounds/weapons/pistol_fire.ogg");
    SoundManager::Add("weapon_smg_fire", "assets/sounds/weapons/smg_fire.ogg");
    SoundManager::Add("weapon_shotgun_fire", "assets/sounds/weapons/shotgun_fire.ogg");
    SoundManager::Add("weapon_rifle_fire", "assets/sounds/weapons/rifle_fire.ogg");

    if (argc > 2 && std::string(argv[1]) == "run")
    {
        InGameState* inGameState = new InGameState;
        inGameState->world.LoadFromFile(std::string(argv[2]));
        GameStateManager::SetState(inGameState);
    }
    else if (argc > 2 && std::string(argv[1]) == "edit")
    {
        LevelEditorState* inGameState = new LevelEditorState;
        inGameState->world.LoadFromFile(std::string(argv[2]));
        GameStateManager::SetState(inGameState);
    }
    else
    {
        GameStateManager::SetState(new MainMenu);
    }

/*
    InGameState* inGameState = new InGameState;
    inGameState->world.LoadFromFile(std::string("level.txt"));
    GameStateManager::SetState(inGameState);
*/
    while (!WindowShouldClose())
    {
        GameState* state = GameStateManager::GetState();

        state->Update();

        BeginDrawing();
        ClearBackground(GRAY);
        state->Draw();
        EndDrawing();
    }

    CloseWindow();
}