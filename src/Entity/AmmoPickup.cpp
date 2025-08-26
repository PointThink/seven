#include "AmmoPickup.hpp"

#include <raylib.h>
#include <algorithm>

#include "Player.hpp"
#include "GameState/InGame.hpp"

static std::unordered_map<std::string, int> pickupCounts = {
    {"9mm", 30},
    {"12ga", 8},
    {"762", 30}
};

EntityAmmoPickup::EntityAmmoPickup() : Entity(EntityType::AMMO_PICKUP)
{
    size = {16, 16};
    collider.size = size;
}

void EntityAmmoPickup::Draw()
{
    DrawRectangle(position.x, position.y, size.x, size.y, ORANGE);
}

void EntityAmmoPickup::Update()
{

}

void EntityAmmoPickup::OnEntityCollide(CollisionInfo info, Entity* otherEntity)
{
    if (otherEntity->GetType() == EntityType::PLAYER)
    {
        EntityPlayer* player = (EntityPlayer*) otherEntity;
        player->ammoPool.GiveAmmo(type, pickupCounts[type]);
    }

    InGameState* state = (InGameState*) GameStateManager::GetState();
    state->world.entites.erase(std::find(state->world.entites.begin(), state->world.entites.end(), this));
}

