#include "Config.h"
#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager() { LoadResourcesFromFileSys(true); }

ResourceManager::~ResourceManager() {}

void ResourceManager::LoadResourcesFromFileSys(bool setSmoth) {
  if (!m_capture_it.loadFromFile(WORKING_DIR "Capture_it.ttf")) {
    std::cerr << "Couldn't load the Font: Capture_it.ttf" << std::endl
              << " Exiting..." << std::endl;
    system("pause");
    exit(-1);
  }

  if (!m_hack.loadFromFile(WORKING_DIR "HackRegular.ttf")) {
    std::cerr << "Couldn't load the Font: HackRegular.ttf" << std::endl
              << " Exiting..." << std::endl;
    system("pause");
    exit(-1);
  }
}

sf::Font &ResourceManager::GetFont(Fonts font) {
  switch (font) {
  case HACK:
    return m_hack;

  // case DEFAULT:
  default:
    return m_capture_it;
  }
}