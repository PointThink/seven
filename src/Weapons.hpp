#pragma once

#include <string>
#include <unordered_map>

class Entity;

enum struct WeaponType
{
    PISTOL,
    SMG,
    SHOTGUN,
    RIFLE
};

struct AmmoPool
{
    AmmoPool();

    int GetAmmoCount(std::string type);
    void TakeAmmo(std::string type, int count);
    void GiveAmmo(std::string type, int count);

private:
    std::unordered_map<std::string, int> ammoTypes;
};

struct Weapon
{
    void Fire(Entity* user, float direction, bool firstPress);
    void Reload(AmmoPool* ammoPool);
    void Update(Entity* user, AmmoPool* ammoPool);

    inline int GetCurrentAmmo() { return currentAmmo; }
    inline void SetCurrentAmmo(int count) { currentAmmo = count; }
    inline int GetMaxAmmo() { return magazineCapacity; }
    inline WeaponType GetType() { return type; }
    inline std::string GetAmmoType() { return ammoType; }

protected:
    WeaponType type;
    std::string ammoType;
    std::string fireSound;
    bool automatic;
    float fireDelay = false;
    int magazineCapacity;
    int damage;
    float reloadTime;
    int bulletsPerShot = 1;
    int currentAmmo = 0;
    float spread = 0;

private:
    float lastShot = 0;
    float reloadBeginTime = 0;
    bool isReloading = false;
};

struct WeaponPistol : public Weapon
{
    WeaponPistol();
};

struct WeaponSMG : public Weapon
{
    WeaponSMG();
};

struct WeaponShotgun : public Weapon
{
    WeaponShotgun();
};