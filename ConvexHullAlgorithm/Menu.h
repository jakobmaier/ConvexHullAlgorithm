#pragma once

class Dots;
class Hull;
class ResourceManager;
class Button;

struct Input;

class Menu {
public:
  Menu(ResourceManager &resMan, Hull &hull, Dots &dots);
  ~Menu();

  void Draw(sf::RenderWindow &window) const;
  void Update(Input input);

private:
	Button* td;
	std::vector<Button*> m_buttons;
	std::vector<Button*> m_indicators;
	sf::RectangleShape m_background;

	ResourceManager &m_resMan;
	Hull &m_hull;
	Dots &m_dots;
};
