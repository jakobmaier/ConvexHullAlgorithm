#pragma once

#include "Dots.h"
#include "Hull.h"
#include "ResourceManager.h"
#include "Button.h"

struct Input;

class Menu {
public:
  Menu(ResourceManager &resMan, Hull &hull, Dots &dots);
  ~Menu();

  void Draw(sf::RenderWindow &window) const;
  void Update(Input input);

private:
	//std::vector<Button*> m_buttons;
	std::vector<Button*> m_indicators;
	sf::RectangleShape m_background;

	ResourceManager &m_resMan;
	Hull &m_hull;
	Dots &m_dots;
};
