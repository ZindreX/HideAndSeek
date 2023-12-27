#pragma once
#include <SFML/System/Vector2.hpp>

struct LineSegment
{
	sf::Vector2f start = { 0.0f, 0.0f };
	sf::Vector2f end = { 0.0f, 0.0f };
	bool isSupport = false;

	LineSegment(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, const bool isSupport = false)
		: start(startPoint)
		, end(endPoint)
		, isSupport(isSupport) { }
};

struct Intersect
{
	bool result;
	sf::Vector2f pos;
};

class VecMath
{
public:
	static const float CrossProduct2D(const sf::Vector2f& r, const sf::Vector2f& s);
	static const Intersect& LineIntersect(const LineSegment& line1, const LineSegment& line2);
	static const sf::Vector2f CalculateWindowEdgeIntersection(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Vector2u& windowSize);
	static const float ConvertAngleToRadians(const float angle);
	static void NormalizeVector(sf::Vector2f& rayDirection);
};