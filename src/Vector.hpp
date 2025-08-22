#pragma once

struct Vector
{
    float x = 0, y = 0;
};

inline Vector operator+(const Vector& a, const Vector& b)
{
    return {a.x + b.x, a.y + b.y};
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return {a.x - b.x, a.y - b.y};
}

float GetDirection(Vector from, Vector to);