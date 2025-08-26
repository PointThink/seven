#include "Bullet.hpp"

#include <raylib.h>
#include <math.h>
#include <algorithm>

#include "GameState/InGame.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

EntityBullet::EntityBullet() : Entity(EntityType::BULLET)
{
    size = {2, 2};
    collider.size = size;
}

void EntityBullet::Update()
{
    position.x += motion.x * GetFrameTime();
    position.y += motion.y * GetFrameTime();
}

void EntityBullet::Draw()
{
    DrawCircle(position.x + 1, position.y + 1, 2, ORANGE);
}

void EntityBullet::OnWorldCollide(CollisionInfo info, RectCollider collider)
{
    if (!active)
        return;
    
    InGameState* state = (InGameState*) GameStateManager::GetState();

    // hacky vector element removal, i love C++
    state->world.entities.erase(std::find(state->world.entities.begin(), state->world.entities.end(), this));
    active = false;
}

void EntityBullet::OnEntityCollide(CollisionInfo info, Entity* otherEntity)
{
    if (!active)
        return;
    
    else if (otherEntity == creator)
        return;

    else if (otherEntity->GetType() == EntityType::BULLET)
        return;

    else if (otherEntity->GetType() == EntityType::ENEMY)
        ((EntityEnemy*) otherEntity)->health -= damage; 

    else if (otherEntity->GetType() == EntityType::PLAYER)
        ((EntityPlayer*) otherEntity)->health -= damage; 


    InGameState* state = (InGameState*) GameStateManager::GetState();


    // hacky vector element removal, i love C++
    state->world.entities.erase(std::find(state->world.entities.begin(), state->world.entities.end(), this));
}

void CreateBullet(Entity* creator, Vector position, int damage, float angle, float velocity)
{
    EntityBullet* bullet = new EntityBullet;
    bullet->creator = creator;
    bullet->damage = damage;
    bullet->position.x = position.x - bullet->size.x / 2;
    bullet->position.y = position.y - bullet->size.y / 2;

    float tangents = angle * 0.0174532925;

    bullet->motion = {velocity * cos(tangents), velocity * sin(tangents)};

    InGameState* state = (InGameState*) GameStateManager::GetState();
    state->world.entities.push_back(bullet);
}