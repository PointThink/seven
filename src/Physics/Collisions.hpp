#pragma once

#include "Vector.hpp"

struct CollisionInfo
{
    bool collides = false;
    Vector normal;
    float depth;
};

struct RectCollider
{
    Vector position;
    Vector size;
};

CollisionInfo TestCollisionRects(RectCollider a, RectCollider b);
bool LineIntersectsRect(const Vector& p1, const Vector& p2, const RectCollider& rect);