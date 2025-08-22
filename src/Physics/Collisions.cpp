#include "Collisions.hpp"

#include <math.h>
#include <vector>

CollisionInfo TestCollisionRects(RectCollider box1, RectCollider box2)
{
    CollisionInfo info;
	info.collides = false;

	float aCenterX, aCenterY, bCenterX, bCenterY;

	aCenterX = box1.position.x + box1.size.x / 2;
	aCenterY = box1.position.y + box1.size.y / 2;
	bCenterX = box2.position.x + box2.size.x / 2;
	bCenterY = box2.position.y + box2.size.y / 2;

	float nx, ny;
	nx = aCenterX - bCenterX;
	ny = aCenterY - bCenterY;

	float aExtent = ((box1.position.x + box1.size.x) - box1.position.x) / 2;
	float bExtent = ((box2.position.x + box2.size.x) - box2.position.x) / 2;

	float xOverlap = aExtent + bExtent - abs(nx);

	if (xOverlap > 0)
	{
		aExtent = ((box1.position.y + box1.size.y) - box1.position.y) / 2;
		bExtent = ((box2.position.y + box2.size.y) - box2.position.y) / 2;

		float yOverlap = aExtent + bExtent - abs(ny);

		if (yOverlap > 0)
		{
			if (xOverlap > yOverlap)
			{
				if (ny < 0)
				{
					info.normal.x = 0;
					info.normal.y = 1;
				}
				else
				{
					info.normal.x = 0;
					info.normal.y = -1;
				}

				info.depth = yOverlap;
				info.collides = true;
			}
			else
			{
				if (nx < 0)
				{
					info.normal.x = 1;
					info.normal.y = 0;
				}
				else
				{
					info.normal.x = -1;
					info.normal.y = 0;
				}

				info.depth = xOverlap;
				info.collides = true;
			}
		}
	}

	return info;
}

// No clue how this works
// ChatGPT spat it out

// Helper: orientation test
float Cross(const Vector& a, const Vector& b) {
    return a.x * b.y - a.y * b.x;
}

// Check if two line segments intersect
bool SegmentsIntersect(const Vector& p1, const Vector& p2,
                       const Vector& q1, const Vector& q2) {
    Vector r = p2 - p1;
    Vector s = q2 - q1;
    float denom = Cross(r, s);

    if (denom == 0.0f) return false; // parallel or collinear

    float t = Cross(q1 - p1, s) / denom;
    float u = Cross(q1 - p1, r) / denom;

    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

// Check if line segment intersects an axis-aligned rectangle
bool LineIntersectsRect(const Vector& p1, const Vector& p2, const RectCollider& rect) {
    float xMin = rect.position.x;
    float yMin = rect.position.y;
    float xMax = rect.position.x + rect.size.x;
    float yMax = rect.position.y + rect.size.y;

    // Rectangle corners
    Vector topLeft     = {xMin, yMin};
    Vector topRight    = {xMax, yMin};
    Vector bottomLeft  = {xMin, yMax};
    Vector bottomRight = {xMax, yMax};

    // Check if segment intersects any of the four edges
    if (SegmentsIntersect(p1, p2, topLeft, topRight)) return true;
    if (SegmentsIntersect(p1, p2, topRight, bottomRight)) return true;
    if (SegmentsIntersect(p1, p2, bottomRight, bottomLeft)) return true;
    if (SegmentsIntersect(p1, p2, bottomLeft, topLeft)) return true;

    // Also check if both points are inside the rectangle
    if (p1.x >= xMin && p1.x <= xMax && p1.y >= yMin && p1.y <= yMax) return true;
    if (p2.x >= xMin && p2.x <= xMax && p2.y >= yMin && p2.y <= yMax) return true;

    return false;
}