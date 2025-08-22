#pragma once

#include <vector>
#include <string>

#include "Entity/Entity.hpp"
#include "Physics/Collisions.hpp"

#define CELL_SIZE 16

struct WallGrid
{
    WallGrid(int width, int height);
    ~WallGrid();

    inline bool GetCell(int x, int y)
    {
        return grid[y * height + x];
    }

    inline void SetCell(int x, int y, bool wall)
    {
        grid[y * height + x] = wall;
    }

    inline int GetWidth() { return width; }
    inline int GetHeight() { return height; }

    std::vector<RectCollider> MakeCollidersFromCells();

private:
    int width, height;
    bool* grid;
};

struct World
{
    WallGrid* wallGrid = nullptr;
    std::vector<Entity*> entites;
    std::vector<RectCollider> worldColliders;

    World();
    ~World();

    void Draw();
    void Update();

    void ExportToFile(std::string path);
    void LoadFromFile(std::string path);

    bool HasLineOfSight(const Vector& from, const Vector& to);
};