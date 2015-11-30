#pragma once
#include "Point.h"
#include "basics.h"
#include "Config.h"

#include <memory>
#include <stack>
class LineShape;

namespace sf {
	class RenderWindow;
}

class Hull {

public:
	Hull();
	~Hull();
	void Draw(sf::RenderWindow &window);

	void CreateHull(std::stack<const Point *> &points,
		const Point *point_that_caused_pop = nullptr);

	void CreateHull(std::vector<const Point *> &points);

	void PushBackNewLine(Vec2f pos1, Vec2f pos2, sf::Color color = LINE_COLOR);
	
	
	std::shared_ptr<LineShape> CreateLine(Vec2f pos1, Vec2f pos2,
		sf::Color color = LINE_COLOR) const;

	void Clear();
private:
	std::vector<std::shared_ptr<LineShape>> m_hull_lines;
};
