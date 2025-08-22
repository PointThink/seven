#pragma once

#include <string>

#include "GameState.hpp"
#include "World.hpp"

enum struct EditorMode
{
    WALL,
    ENEMY,
    AMMO_PICKUP,
    WEAPON_PICKUP,
    PLAYER, 
    DELETE
};

std::string GetEditorModeName(EditorMode mode);

struct LevelEditorState : public GameState
{
    World world;
    
    LevelEditorState();

    void Update() override;
    void Draw() override;

private:
    EditorMode mode = EditorMode::WALL;
    int zoom = 2;

    std::vector<std::string> ammoPickupTypes = {"9mm", "12ga", "556"};
    int currentAmmoPickupTypeIndex = 0;

    std::vector<std::string> weaponPickupTypes = {"smg", "shotgun", "rifle"};
    int currentWeaponPickupTypeIndex = 0;
};