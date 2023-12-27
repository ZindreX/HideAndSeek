#include <utility>
#include <cmath>

#include "VecMath.h"

const float VecMath::CrossProduct2D(const sf::Vector2f& r, const sf::Vector2f& s)
{
	return r.x * s.y - r.y * s.x;
}

const Intersect& VecMath::LineIntersect(const LineSegment& line1, const LineSegment& line2)
{
	// Find the vectors of the lines
	const sf::Vector2f r = line1.end - line1.start;
	const sf::Vector2f s = line2.end - line2.start;

	// Calculate the cross product
	const float rxs = CrossProduct2D(r, s);

	// 
	const sf::Vector2f cma = line2.start - line1.start;
	const float t = CrossProduct2D(cma, s) / rxs;
	const float u = CrossProduct2D(cma, r) / rxs;

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return { true, sf::Vector2f(line1.start.x + t * r.x, line1.start.y + t * r.y) };
	}
	return { false, sf::Vector2f(0.0f, 0.0f) };
}

const sf::Vector2f VecMath::CalculateWindowEdgeIntersection(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Vector2u& windowSize) {
	// Calculate intersection with vertical window boundaries
	float tX1 = (0 - origin.x) / direction.x;
	float tX2 = (windowSize.x - origin.x) / direction.x;

	// Calculate intersection with horizontal window boundaries
	float tY1 = (0 - origin.y) / direction.y;
	float tY2 = (windowSize.y - origin.y) / direction.y;

	// Find the smallest positive t value for valid intersection
	float tMin = std::max(std::min(tX1, tX2), std::min(tY1, tY2));

	// Calculate intersection point
	return origin + tMin * direction;
}

const float VecMath::ConvertAngleToRadians(const float angle)
{
	return static_cast<float>(angle) * (3.14159265359f / 180.0f);
}

void VecMath::NormalizeVector(sf::Vector2f& rayDirection)
{
	float length = std::sqrt(rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y);
	rayDirection /= length;
}
