#include "ResourceManager.h"
#include "Config.h"
#include <iostream>


ResourceManager::ResourceManager()
{
	LoadResourcesFromFileSys(true);
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadResourcesFromFileSys(bool setSmoth)
{
	if (!m_capture_it.loadFromFile(WORKING_DIR "Capture_it.ttf"))
	{
		std::cerr << "Couldn't load the Font: Capture_it.ttf" << std::endl << " Exiting..." << std::endl;
		system("pause");
		exit(-1);
	}
}

sf::Font& ResourceManager::GetFont(Fonts font)
{
	//switch (font)
	//{
	//case DEFAULT: 
	//case POSTER: 
	//default:
		return m_capture_it; // only one case !
	//}
}