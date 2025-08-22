#pragma once

struct GameState
{
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

struct GameStateManager
{
    static void SetState(GameState* state);
    static GameState* GetState();

private:
    static GameState* state;
};