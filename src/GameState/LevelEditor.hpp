#pragma once

#include <string>
#include <array>

#include "GameState.hpp"
#include "World.hpp"
#include "Vector.hpp"

enum struct EditorMode
{
    WALL,
    ENEMY,
    AMMO_PICKUP,
    WEAPON_PICKUP,
    PLAYER,
    FLOOR,
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
    Vector position;
    Vector mousePos;

    std::vector<std::string> ammoPickupTypes = {"9mm", "12ga", "762"};
    int currentAmmoPickupTypeIndex = 0;

    std::vector<std::string> weaponPickupTypes = {"smg", "shotgun", "rifle"};
    int currentWeaponPickupTypeIndex = 0;

    std::vector<std::string> floorTextures = {
        "floor_wood_dark",
        "floor_wood_light",
        "floor_pavement1",
        "floor_pavement2",
        "floor_kitchen"
    };

    int currentFloorTextureIndex = 0;

    float enemyRotation = 0;
};