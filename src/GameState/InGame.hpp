#pragma once

#include <raylib.h>

#include "GameState.hpp"
#include "World.hpp"

struct EntityPlayer;

struct InGameState : GameState
{
    World world;
    Camera2D camera = {0};

    InGameState();

    void Update() override;
    void Draw() override;

    inline EntityPlayer* GetPlayer() { return player; }

private:
    EntityPlayer* player = nullptr;
};
