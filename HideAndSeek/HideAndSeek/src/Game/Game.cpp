#include <set>
#include "Game.h"

void Game::print(const std::string& text)
{
	std::cout << text << std::endl;
}

void Game::init()
{
	m_window.create(sf::VideoMode(1200, 720), "My window");
	m_window.setMouseCursorVisible(false);
	m_window.setMouseCursorGrabbed(true);

	m_player = sf::CircleShape(MOUSE_RADIUS);
	m_player.setFillColor(VISION_LINE_COLOR);
	m_player.setOrigin(sf::Vector2f(MOUSE_RADIUS, MOUSE_RADIUS));

	const sf::Vector2u windowSize = m_window.getSize();
	m_corners = {
		sf::Vector2f(0, 0),
		sf::Vector2f(windowSize.x, 0),
		sf::Vector2f(0, windowSize.y),
		sf::Vector2f(windowSize.x, windowSize.y)
	};

	addFont();
	addObstacles();
	addAllLineSegments();
	
	run();
}

void Game::addFont()
{
	sf::Font font;
	if (!font.loadFromFile("./data/SummerPixel22Regular-jE0W7.ttf"))
	{
		print("Error loading font!");
	}

	m_font = font;
}

void Game::addObstacles()
{
	addObstacle({
		sf::Vector2f(500, 500),
		sf::Vector2f(550, 500),
		sf::Vector2f(550, 550),
		sf::Vector2f(500, 550)
	});

	addObstacle({
		sf::Vector2f(500, 100),
		sf::Vector2f(750, 100),
		sf::Vector2f(750, 350),
		sf::Vector2f(500, 350)
	});

	addObstacle({
		sf::Vector2f(900, 100),
		sf::Vector2f(1100, 100),
		sf::Vector2f(1100, 200),
		sf::Vector2f(900, 200)
	});

	addObstacle({
		sf::Vector2f(70, 50),
		sf::Vector2f(370, 50),
		sf::Vector2f(185, 350)
	});

	addObstacle({
		sf::Vector2f(870, 650),
		sf::Vector2f(1170, 650),
		sf::Vector2f(1055, 350)
	});

	addObstacle({
		sf::Vector2f(200, 400),
		sf::Vector2f(330, 550),
		sf::Vector2f(280, 700),
		sf::Vector2f(120, 700),
		sf::Vector2f(70, 550)
	});
}

void Game::addObstacle(const std::vector<sf::Vector2f>& vertices)
{
	const size_t vertexCount = vertices.size();

	sf::ConvexShape shape(vertexCount);
	for (size_t i = 0; i < vertexCount; i++)
	{
		shape.setPoint(i, vertices[i]);
		shape.setFillColor(OBSTACLE_COLOR);
		//shape.setOutlineThickness(3.0f);
		//shape.setOutlineColor(OBSTACLE_OUTLINE_COLOR);
	}

	m_obstacles.push_back(shape);
}

void Game::addAllLineSegments()
{
	for (size_t i = 0; i < m_obstacles.size(); i++)
	{
		const sf::ConvexShape& obstacle = m_obstacles[i];

		const size_t pointCount = obstacle.getPointCount() - 1;
		// Go through each vertex of the shape, and add its edges (line segments)
		for (size_t j = 0; j < pointCount; j++)
		{
			const LineSegment lineSegment(obstacle.getPoint(j), obstacle.getPoint(j + 1));
			m_lineSegments.push_back(lineSegment);
		}

		// Add final edge back to start vertex
		const LineSegment finalLineSegment(obstacle.getPoint(pointCount), obstacle.getPoint(0));
		m_lineSegments.push_back(finalLineSegment);
	}
}

void Game::run()
{
	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				m_window.close();
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::V:
					m_drawVision = !m_drawVision;
					break;
					
				case sf::Keyboard::D:
					m_drawDebug = !m_drawDebug;
					break;
				}
			}
		}

		update();
	}
}

void Game::update()
{
	movement();
	//collision();
	draw();
}

void Game::movement()
{
	m_prevPosition = m_position;
	m_position = sf::Vector2f(sf::Mouse::getPosition(m_window));
	print(getPointToString(m_position));
	m_player.setPosition(m_position);
}

void Game::collision()
{
	for (const auto& obstacle : m_obstacles)
	{
		sf::FloatRect obstacleBounds = obstacle.getLocalBounds();

		// Check if mouse is inside the obstacle's bounding box
		if (m_player.getGlobalBounds().intersects(obstacleBounds))
		{
			//const sf::Vector2f mousePosition = m_player.getPosition();

			// Adjust the mouse position to be at the edge of the obstacle
			//float newX = std::max(obstacleBounds.left, std::min(obstacleBounds.left + obstacleBounds.width, static_cast<float>(mousePosition.x)));
			//float newY = std::max(obstacleBounds.top, std::min(obstacleBounds.top + obstacleBounds.height, static_cast<float>(mousePosition.y)));

			sf::Mouse::setPosition(sf::Vector2i(m_prevPosition.x, m_prevPosition.y)); // sf::Vector2i(newX, newY));
		}
	}
}

void Game::draw()
{
	const std::vector<LineSegment> visionLines = getVisionLines(m_position);

	// Rendering
	m_window.clear();

	drawObstacles();
	drawVision(visionLines, m_position);
	m_window.draw(m_player);

	m_window.display();
}

const std::vector<LineSegment> Game::getVisionLines(const sf::Vector2f& mousePosition)
{
	std::vector<LineSegment> result;

	#pragma region Alternative 1 (Stepping approach)
	//for (size_t angle = 0; angle < 360; angle += 5)
	//{
	//	// Convert angle to radians
	//	const float radians = VecMath::ConvertAngleToRadians(angle);

	//	// Calculate ray direction
	//	sf::Vector2f rayDirection(std::cos(radians), std::sin(radians));

	//	// Normalize ray direction
	//	VecMath::NormalizeVector(rayDirection);

	//	// Extend the ray to the edge of the window
	//	const sf::Vector2f intersectionAtEdgeOfWindow = VecMath::CalculateWindowEdgeIntersection(mousePosition, rayDirection, windowSize);
	//	LineSegment angleLine = { sf::Vector2f(mousePosition), intersectionAtEdgeOfWindow };

	//	// Go through obstacles and cut the beam if it hits one
	//	for (const auto& lineSegment : m_lineSegments)
	//	{
	//		const Intersect intersection = VecMath::LineIntersect(angleLine, lineSegment);

	//		if (intersection.result)
	//		{
	//			angleLine.end = intersection.pos;
	//		}
	//	}

	//	result.push_back(angleLine);
	//}
	#pragma endregion

	#pragma region Alternative 2 (Drawing to each vertex + 2x scewed support lines)
	for (const auto& obstacleLine : m_lineSegments)
	{
		checkVertexPoint(mousePosition, obstacleLine.start, result);
		checkVertexPoint(mousePosition, obstacleLine.end, result);
	}
	// Add corners of the window
	for (const sf::Vector2f& corner : m_corners)
	{
		checkVertexPoint(mousePosition, corner, result, false);
	}
	#pragma endregion

	return result;
}

void Game::checkVertexPoint(const sf::Vector2f& mousePosition, const sf::Vector2f& point, std::vector<LineSegment>& result, const bool makeSupportLines)
{
	LineSegment visionLine = { mousePosition, point };

	bool skipSupportLines = false;
	for (const auto& obstacleLine : m_lineSegments)
	{
		const Intersect intersection = VecMath::LineIntersect(visionLine, obstacleLine);

		if (intersection.result)
		{
			visionLine.end = intersection.pos;
			//skipSupportLines = true;
		}
	}

	if (makeSupportLines && !skipSupportLines)
	{
		addSupportLines(OFFSET_ANGLE_1, point, mousePosition, result);
		addSupportLines(OFFSET_ANGLE_2, point, mousePosition, result);
	}
	else if (!makeSupportLines)
	{
		visionLine.isSupport = true;
	}
	
	result.push_back(visionLine);
}

void Game::addSupportLines(const float offsetAngle, const sf::Vector2f& point, const sf::Vector2f& mousePosition, std::vector<LineSegment>& result)
{
	sf::Vector2f direction = -(point - mousePosition);

	const sf::Vector2f scewedDirection = sf::Vector2f(std::cos(offsetAngle) * direction.x - std::sin(offsetAngle) * direction.y,
		std::sin(offsetAngle) * direction.x + std::cos(offsetAngle) * direction.y);
	const sf::Vector2f intersectionAtEdgeOfWindow = VecMath::CalculateWindowEdgeIntersection(mousePosition, scewedDirection, m_window.getSize());

	checkVertexPoint(mousePosition, intersectionAtEdgeOfWindow, result, false);
}

void Game::drawVision(const std::vector<LineSegment> visionLines, const sf::Vector2f& mousePosition)
{
	const std::vector<sf::Vector2f> visionPoints = getUniqueAndSortedVisionPoints(visionLines, mousePosition);

	if (!m_drawVision)
	{
		for (const auto& visionLine : visionLines)
		{
			sf::VertexArray line(sf::Lines, 2);
			line[0].position = visionLine.start;
			line[1].position = visionLine.end;
			line[0].color = visionLine.isSupport ? SUPPORT_LINE_COLOR : VISION_LINE_COLOR;
			line[1].color = visionLine.isSupport ? SUPPORT_LINE_COLOR : VISION_LINE_COLOR;

			m_window.draw(line);
		}
	}
	else
	{
		// NOTE: ConvexShape won't draw correctly, hence manually drawing triangles
		for (size_t i = 0; i < visionPoints.size() - 1; i++)
		{
			sf::ConvexShape triangleShape(3);
			triangleShape.setPoint(0, mousePosition);
			triangleShape.setPoint(1, visionPoints[i]);
			triangleShape.setPoint(2, visionPoints[i + 1]);
			triangleShape.setFillColor(VISION_AREA_COLOR);
			m_window.draw(triangleShape);
		}

		// Draw last triangle
		sf::ConvexShape triangleShape(3);
		triangleShape.setPoint(0, mousePosition);
		triangleShape.setPoint(1, visionPoints[visionPoints.size() - 1]);
		triangleShape.setPoint(2, visionPoints[0]);
		triangleShape.setFillColor(VISION_AREA_COLOR);
		m_window.draw(triangleShape);
	}

	#pragma region Debug draw
	if (m_drawDebug)
	{
		sf::Text pointNr;
		pointNr.setCharacterSize(25);
		pointNr.setFillColor(sf::Color::White);
		pointNr.setFont(m_font);
		size_t index = 0;
		std::string debugString = "";
		for (const auto& point : visionPoints)
		{
			const std::string indexWithPosition = std::to_string(index) + ": " + getPointToString(point);
			debugString += indexWithPosition + "\n";
			
			pointNr.setString(indexWithPosition);
			pointNr.setPosition(point);

			sf::CircleShape vertexPointShape(MOUSE_RADIUS);
			vertexPointShape.setFillColor(VERTEX_POINT_COLOR);
			vertexPointShape.setPosition(point);
			vertexPointShape.setOrigin(MOUSE_RADIUS, MOUSE_RADIUS);
			m_window.draw(vertexPointShape);
			m_window.draw(pointNr);
			index++;
		}

		sf::Text debugText;
		debugText.setCharacterSize(25);
		debugText.setFillColor(sf::Color::White);
		debugText.setFont(m_font);
		debugText.setPosition(sf::Vector2f(10.0f, 50.0f));
		debugText.setString(debugString);
		m_window.draw(debugText);
		//print(debugString);
	}
	#pragma endregion
}

const std::vector<sf::Vector2f> Game::getUniqueAndSortedVisionPoints(const std::vector<LineSegment>& visionLines, const sf::Vector2f& mousePosition)
{
	std::set<sf::Vector2f, Vector2fCompare> uniquePoints;
	for (const auto& visionLine : visionLines)
	{
		uniquePoints.insert(visionLine.end);
	}

	// Back to vector for sorting
	std::vector<sf::Vector2f> visionPoints(uniquePoints.begin(), uniquePoints.end());

	// Sort the vision points based on their angle relative to the mouse position
	std::sort(visionPoints.begin(), visionPoints.end(), [=](const auto& a, const auto& b) {
		return comparePoints(a, b, mousePosition);
		});

	return visionPoints;
}

const std::string& Game::getPointToString(const sf::Vector2f& point)
{
	return "(" + std::to_string(static_cast<size_t>(point.x)) + ", " + std::to_string(static_cast<size_t>(point.y)) + ")";
}

bool Game::comparePoints(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& origin)
{
	// Compare points based on their angle relative to the origin (mouse position)
	return std::atan2(a.y - origin.y, a.x - origin.x) < std::atan2(b.y - origin.y, b.x - origin.x);
}

void Game::drawObstacles()
{
	for (const sf::ConvexShape& shape : m_obstacles)
	{
		m_window.draw(shape);
	}
}

const sf::RenderWindow& Game::window()
{
	return m_window;
}
