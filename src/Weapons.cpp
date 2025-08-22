#include "Weapons.hpp"

#include <iostream>
#include <raylib.h>

#include "Entity/Entity.hpp"
#include "Entity/Bullet.hpp"

void Weapon::Fire(Entity* user, float direction, bool firstPress)
{
    if (user->GetType() != EntityType::PLAYER && !automatic)
    {
        if (lastShot + fireDelay * 2 > GetTime())
            return;
    }
    else
    {
        if (lastShot + fireDelay > GetTime())
            return;
    }

    if (currentAmmo > 0 && !isReloading)
    {
        if (firstPress || automatic)
        {
            currentAmmo--;
            std::cout << currentAmmo << "/" << magazineCapacity << std::endl;

            Vector userCenter = {
                user->position.x + user->size.x / 2,
                user->position.y + user->size.y / 2
            };

            for (int i = 0; i < bulletsPerShot; i++)
            {
                float x = (float)rand()/(float)(RAND_MAX/spread) - (spread / 2);
                std::cout << x << std::endl;
                CreateBullet(user, userCenter, damage, direction + x, 800);
            }

            lastShot = GetTime();
        }
    }
}

void Weapon::Reload(AmmoPool* ammoPool)
{
    if (isReloading)
        return;

    if (ammoPool != nullptr && ammoPool->GetAmmoCount(ammoType) <= 0)
        return;

    isReloading = true;
    reloadBeginTime = GetTime();
}

void Weapon::Update(Entity* user, AmmoPool* ammoPool)
{
    if (isReloading && reloadBeginTime + reloadTime < GetTime())
    {
        isReloading = false;
        if (ammoPool == nullptr)
        {
            currentAmmo = magazineCapacity;
            return;
        }
        
        int roundsToTake = 0;

        if (currentAmmo == 0)
            roundsToTake = magazineCapacity;
        else
            roundsToTake = (magazineCapacity + 1) - currentAmmo;

        int availableAmmo = ammoPool->GetAmmoCount(ammoType);

        if (roundsToTake <= availableAmmo)
        {
            currentAmmo += roundsToTake;
            ammoPool->TakeAmmo("9mm", roundsToTake);
        }
        else
        {
            currentAmmo += availableAmmo;
            ammoPool->TakeAmmo("9mm", availableAmmo);
        }
    }
}


AmmoPool::AmmoPool()
{
    ammoTypes.insert({"9mm", 0});
    ammoTypes.insert({"12ga", 0});
    ammoTypes.insert({"556", 0});
}

int AmmoPool::GetAmmoCount(std::string ammoType)
{
    return ammoTypes[ammoType];
}

void AmmoPool::TakeAmmo(std::string ammoType, int count)
{
    ammoTypes[ammoType] -= count;
}

void AmmoPool::GiveAmmo(std::string ammoType, int count)
{
    ammoTypes[ammoType] += count;
}

WeaponPistol::WeaponPistol()
{
    type = WeaponType::PISTOL;
    ammoType = "9mm";
    magazineCapacity = 17;
    automatic = false;
    damage = 15;
    reloadTime = 1;
    fireDelay = 0.1;
    spread = 2;
}

WeaponSMG::WeaponSMG()
{
    type = WeaponType::SMG;
    ammoType = "9mm";
    magazineCapacity = 30;
    automatic = true;
    damage = 15;
    reloadTime = 1.5;
    fireDelay = 0.07;
    spread = 4;
}

WeaponShotgun::WeaponShotgun()
{
    type = WeaponType::SHOTGUN;
    ammoType = "12ga";
    magazineCapacity = 8;
    automatic = true;
    damage = 10;
    reloadTime = 1.5;
    bulletsPerShot = 6;
    fireDelay = 0.5;
    spread = 12;
}