#include "WeaponPickup.hpp"

#include <raylib.h>
#include <algorithm>

#include "GameState/InGame.hpp"
#include "Player.hpp"

Weapon* CreateWeaponFromString(std::string name)
{
    if (name == "pistol")
        return new WeaponPistol();
    else if (name == "smg")
        return new WeaponSMG();
    else if (name == "shotgun")
        return new WeaponShotgun();
    else if (name == "rifle")
        return new WeaponPistol(); // todo: implement
}

EntityWeaponPickup::EntityWeaponPickup() : Entity(EntityType::WEAPON_PICKUP)
{
    size = {16, 16};
    collider.size = size;
}

void EntityWeaponPickup::Draw()
{
    DrawRectangle(position.x, position.y, size.x, size.y, GREEN);
}

void EntityWeaponPickup::Update()
{

}

void EntityWeaponPickup::OnEntityCollide(CollisionInfo info, Entity* otherEntity)
{
    if (otherEntity->GetType() == EntityType::PLAYER)
    {
        Weapon* weapon = CreateWeaponFromString(type);
        weapon->SetCurrentAmmo(weapon->GetMaxAmmo());

        EntityPlayer* player = (EntityPlayer*) otherEntity;
        player->weapons[(int) weapon->GetType()] = weapon;
    }

    InGameState* state = (InGameState*) GameStateManager::GetState();
    state->world.entites.erase(std::find(state->world.entites.begin(), state->world.entites.end(), this));
}

