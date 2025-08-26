#pragma once

#include "Vector.hpp"
#include "Physics/Collisions.hpp"

enum struct EntityType
{
    PLAYER,
    BULLET,
    ENEMY,
    AMMO_PICKUP,
    WEAPON_PICKUP
};

struct Entity
{
    Vector position;
    Vector size;
    float rotation = 0;
    RectCollider collider = {0};

    inline Entity(EntityType type)
    {
        this->type = type; 
    }

    virtual void Update() {}
    virtual void Draw() {}

    virtual void OnWorldCollide(CollisionInfo info, RectCollider collider) {}
    virtual void OnEntityCollide(CollisionInfo info, Entity* otherEntity) { }

    inline EntityType GetType() { return type; }

    inline Vector GetCenter()
    {
        return {
            position.x + size.x / 2,
            position.y + size.y / 2
        };
    }

private:
    EntityType type;
};