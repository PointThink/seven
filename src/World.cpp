#include "World.hpp"

#include <iostream>
#include <fstream>
#include <raylib.h>
#include "Entity/Player.hpp"
#include "Entity/Enemy.hpp"
#include "Entity/AmmoPickup.hpp"
#include "Entity/WeaponPickup.hpp"
#include "StringUtils.hpp"
#include "Physics/Collisions.hpp"

WallGrid::WallGrid(int width, int height)
{
    this->width = width;
    this->height = height;
    grid = new bool[width * height];

    for (int i = 0; i < width * height; i++)
        grid[i] = false;
}

WallGrid::~WallGrid()
{
    delete[] grid;
}

std::vector<RectCollider> WallGrid::MakeCollidersFromCells()
{
    std::vector<RectCollider> colliders;

    // no merging colliders, just 1 per cell
    for (int y = 0; y < GetHeight(); y++)
    {
        for (int x = 0; x < GetWidth(); x++)
        {
            if (GetCell(x, y))
            {
                RectCollider collider;
                collider.position = {(float) x * CELL_SIZE, (float) y * CELL_SIZE};
                collider.size = {CELL_SIZE, CELL_SIZE};
                colliders.push_back(collider);
            }
        }
    }

    return colliders;
}
inline int ToGridIndex(float pixelCoord)
{
    return pixelCoord / CELL_SIZE;
}

World::World()
{
    wallGrid = new WallGrid(100, 100);
    entites.push_back(new EntityPlayer);
    worldColliders = wallGrid->MakeCollidersFromCells();
}

World::~World()
{
    delete wallGrid;
}

void World::Update()
{
    // other entities might be added or removed which could break everything
    std::vector<Entity*> entitesCopy = entites;

    for (Entity* entity : entitesCopy)
    {
        for (RectCollider collider : worldColliders)
        {
            entity->collider.position = entity->position;

            CollisionInfo info = TestCollisionRects(entity->collider, collider);
            
            if (info.collides)
            {
                entity->OnWorldCollide(info, collider);

                entity->position.x -= info.normal.x * info.depth;
                entity->position.y -= info.normal.y * info.depth;
            }
        }

        for (Entity* otherEntity : entitesCopy)
        {
            if (otherEntity == entity) continue;

            CollisionInfo info = TestCollisionRects(entity->collider, otherEntity->collider);

            if (!info.collides)
                continue;

            entity->OnEntityCollide(info, otherEntity);
        }

        entity->Update();
    }
}

void World::Draw()
{
    for (Entity* entity : entites)
    {
        entity->Draw();
    }

    for (int y = 0; y < wallGrid->GetHeight(); y++)
    {
        for (int x = 0; x < wallGrid->GetWidth(); x++)
        {
            if (wallGrid->GetCell(x, y))
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
        }
    }
}

void World::ExportToFile(std::string path)
{
    std::ofstream file(path);

    for (int y = 0; y < wallGrid->GetHeight(); y++)
    {
        for (int x = 0; x < wallGrid->GetWidth(); x++)
        {
            if (wallGrid->GetCell(x, y))
                file << "wall=" << x << "," << y << std::endl;
        }
    }

    for (Entity* entity : entites)
    {
        switch (entity->GetType())
        {
            case EntityType::ENEMY:
                file << "enemy=" << entity->position.x << "," << entity->position.y << "," << entity->rotation << std::endl;
                break;
            case EntityType::AMMO_PICKUP:
            {
                EntityAmmoPickup* pickup = (EntityAmmoPickup*) entity;
                file << "ammo=" << pickup->position.x << "," << pickup->position.y << "," << pickup->type << std::endl;
                break;
            }
            case EntityType::PLAYER:
                file << "player=" << entity->position.x << "," << entity->position.y << std::endl;
                break;
            case EntityType::WEAPON_PICKUP:
            {
                EntityWeaponPickup* pickup = (EntityWeaponPickup*) entity;
                file << "weapon=" << entity->position.x << "," << entity->position.y << "," << pickup->type << std::endl;
                break;
            }
        }
    }

    file.close();
}

void World::LoadFromFile(std::string path)
{
    if (wallGrid != nullptr)
        delete wallGrid;

    for (Entity* entity : entites)
        delete entity;

    entites.clear();
    worldColliders.clear();

    wallGrid = new WallGrid(100, 100);

    std::ifstream file(path);
    std::string buffer;

    while(getline(file, buffer))
    {
        std::cout << buffer << std::endl;

        std::vector<std::string> parts = SplitString(buffer, '=');

        std::cout << parts[0] << " = " <<  parts[1] << std::endl;
        
        if (parts[0] == "wall")
        {
            std::vector<std::string> posParts = SplitString(parts[1], ',');
            int x = std::stoi(posParts[0]);
            int y = std::stoi(posParts[1]);
            wallGrid->SetCell(x, y, true);
        }
        if (parts[0] == "enemy")
        {
            std::vector<std::string> posParts = SplitString(parts[1], ',');
            float x = std::stoi(posParts[0]);
            float y = std::stoi(posParts[1]);
            float rotation = std::stoi(posParts[2]);

            EntityEnemy* enemy = new EntityEnemy;
            enemy->position.x = x;
            enemy->position.y = y;
            enemy->rotation = rotation;
            entites.push_back(enemy);
        }
        if (parts[0] == "ammo")
        {
            std::vector<std::string> entityParts = SplitString(parts[1], ',');
            float x = std::stoi(entityParts[0]);
            float y = std::stoi(entityParts[1]);

            EntityAmmoPickup* pickup = new EntityAmmoPickup;
            pickup->position.x = x;
            pickup->position.y = y;
            pickup->type = entityParts[2];
            entites.push_back(pickup);
        }
        if (parts[0] == "weapon")
        {
            std::vector<std::string> entityParts = SplitString(parts[1], ',');
            float x = std::stoi(entityParts[0]);
            float y = std::stoi(entityParts[1]);

            EntityWeaponPickup* pickup = new EntityWeaponPickup;
            pickup->position.x = x;
            pickup->position.y = y;
            pickup->type = entityParts[2];
            entites.push_back(pickup);
        }
        if (parts[0] == "player")
        {
            std::vector<std::string> entityParts = SplitString(parts[1], ',');
            float x = std::stoi(entityParts[0]);
            float y = std::stoi(entityParts[1]);

            EntityPlayer* player = new EntityPlayer;
            player->position.x = x;
            player->position.y = y;
            entites.push_back(player);
        }
    }

    worldColliders = wallGrid->MakeCollidersFromCells();

    file.close();
}

bool World::HasLineOfSight(const Vector& from, const Vector& to)
{
    for (const RectCollider& rect : worldColliders)
    {
        if (LineIntersectsRect(from, to, rect)) 
        {
            return false; // blocked
        }
    }
    return true; // no blockers
}