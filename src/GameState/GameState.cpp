#include "GameState.hpp"

GameState* GameStateManager::state = nullptr;

GameState* GameStateManager::GetState()
{
    return state;
}

void GameStateManager::SetState(GameState* newState)
{
    // yeah this leaks memory but i cant be assed to fix it
    // delete GameStateManager::state;
    GameStateManager::state = newState;
}
