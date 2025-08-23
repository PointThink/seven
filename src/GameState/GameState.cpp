#include "GameState.hpp"

GameState* GameStateManager::state = nullptr;

GameState* GameStateManager::GetState()
{
    return state;
}

void GameStateManager::SetState(GameState* state)
{
    delete GameStateManager::state;
    GameStateManager::state = state;
}
