#include <raylib.h>
#include <iostream>

#include "GameState/GameState.hpp"
#include "GameState/InGame.hpp"
#include "GameState/LevelEditor.hpp"
#include "TextureManager.h"

int main(int argc, char** argv)
{
    InitWindow(1280, 720, "Seven");
    
    srand(time(0));

    TextureManager::Add("pistol", "assets/textures/pistol.png");
    TextureManager::Add("shotgun", "assets/textures/shotgun.png");
    TextureManager::Add("smg", "assets/textures/smg.png");
    TextureManager::Add("rifle", "assets/textures/rifle.png");

    TextureManager::Add("grunt", "assets/textures/grunt.png");
    TextureManager::Add("grunt_dead", "assets/textures/grunt_dead.png");

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
        InGameState* inGameState = new InGameState;
        inGameState->world.LoadFromFile(std::string("level.txt"));
        GameStateManager::SetState(inGameState);
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
        ClearBackground(WHITE);

        state->Draw();

        EndDrawing();
    }

    CloseWindow();
}