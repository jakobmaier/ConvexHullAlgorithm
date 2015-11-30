#pragma once
#include <SFML/Graphics/Font.hpp>

class ResourceManager 
{
public:
	enum Fonts
	{
		DEFAULT
	};

	ResourceManager();
	~ResourceManager();
	void LoadResourcesFromFileSys(bool setSmoth);

	sf::Font& GetFont(Fonts font);

private:
	sf::Font m_capture_it;
};

