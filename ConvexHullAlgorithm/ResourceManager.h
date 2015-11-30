#pragma once
#include <SFML/Graphics/Font.hpp>

class ResourceManager 
{
public:
	enum Fonts
	{
		DEFAULT, HACK
	};

	ResourceManager();
	~ResourceManager();
	void LoadResourcesFromFileSys(bool setSmoth);

	sf::Font& GetFont(Fonts font);

private:
	sf::Font m_capture_it;
	sf::Font m_hack;
};

