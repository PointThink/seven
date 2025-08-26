#pragma once

#include "Entity.hpp"

struct EntityBullet : public Entity
{
    Vector motion;
    Entity* creator;
    int damage;
    bool active = true;

    EntityBullet();

    void Update() override;
    void Draw() override;

    void OnWorldCollide(CollisionInfo info, RectCollider collider) override;
    void OnEntityCollide(CollisionInfo info, Entity* otherEntity) override;
};

void CreateBullet(Entity* creator, Vector position, int damage, float angle, float velocity);