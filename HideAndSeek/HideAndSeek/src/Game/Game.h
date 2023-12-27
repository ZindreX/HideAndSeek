#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../VecMath/VecMath.h"

class Game
{
	const float OFFSET_ANGLE_1 = 0.00001f;
	const float OFFSET_ANGLE_2 = -0.00001f;
	const float MOUSE_RADIUS = 5.0f;

	const sf::Color VISION_LINE_COLOR = sf::Color(255, 0, 0);
	const sf::Color SUPPORT_LINE_COLOR = sf::Color(0, 255, 0);
	const sf::Color VERTEX_POINT_COLOR = sf::Color(0, 0, 255);
	const sf::Color VISION_AREA_COLOR = sf::Color::Yellow;
	const sf::Color OBSTACLE_COLOR = sf::Color(255, 255, 255);
	const sf::Color OBSTACLE_OUTLINE_COLOR = sf::Color(0, 0, 0);

	struct Vector2fCompare {
		bool operator()(const sf::Vector2f& lhs, const sf::Vector2f& rhs) const {
			return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
		}
	};

	sf::RenderWindow m_window;
	std::vector<sf::ConvexShape> m_obstacles;
	std::vector<LineSegment> m_lineSegments;
	std::vector<sf::Vector2f> m_corners;
	sf::Font m_font;
	sf::CircleShape m_player;

	sf::Vector2f m_position = { 0.0f, 0.0f };
	sf::Vector2f m_prevPosition = { 0.0f, 0.0f };

	bool m_drawVision = false;
	bool m_drawDebug = false;

	void print(const std::string& text);

	// Assets
	void addFont();
	void addObstacles();
	void addObstacle(const std::vector<sf::Vector2f>& vertices);
	void addAllLineSegments();
	
	// Game running
	void run();
	void movement();
	void update();
	void collision();
	void draw();

	// Support functions
	const std::vector<LineSegment> getVisionLines(const sf::Vector2f& mousePosition);

	void checkVertexPoint(const sf::Vector2f& mousePosition, const sf::Vector2f& point, std::vector<LineSegment>& result, const bool makeSupportLines = true);
	void addSupportLines(const float offsetAngle, const sf::Vector2f& point, const sf::Vector2f& mousePosition, std::vector<LineSegment>& result);

	void drawVision(const std::vector<LineSegment> visionLines, const sf::Vector2f& mousePosition);
	const std::vector<sf::Vector2f> getUniqueAndSortedVisionPoints(const std::vector<LineSegment>& visionLines, const sf::Vector2f& mousePosition);
	bool comparePoints(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& origin);
	void drawObstacles();

	const std::string& getPointToString(const sf::Vector2f& point);

public:
	void init();

	const sf::RenderWindow& window();
};