#include "Config.h"
#include "Input.h"
#include "Menu.h"
#include "Button.h"

Menu::Menu(ResourceManager &resMan, Hull &hull, Dots &dots)
    : m_resMan(resMan), m_hull(hull), m_dots(dots) {


  m_background.setSize(sf::Vector2f(WINDOW_X, WINDOW_Y));
  m_background.setFillColor(MENU_BACKGROUND_COLOR);

  // ############# Indicators ##########################
  Button *fieldSizeIndicator = new Button(resMan, "XXX", sf::Vector2f(70, 140),
                                          sf::Vector2f(130.f, 50.f));
  fieldSizeIndicator->SetDefaultColor(BTN_DEFAULT_INDICATOR_COLOR);
  fieldSizeIndicator->Update(Input());
  m_indicators.push_back(fieldSizeIndicator);

  // // ############# Buttons ############################

   Button *clear =
       new Button(resMan, "Clear", sf::Vector2f(180, 380));
   //clear->setTriggerFunction([this]() {
   //  m_hull.Clear();
   //  m_dots.Clear();
   //});
   m_buttons.push_back(clear);
}

Menu::~Menu() {}

void Menu::Draw(sf::RenderWindow &window) const {
  window.draw(m_background);
  for (auto &button : m_buttons) {
    button->Draw(window);
  }
  for (auto &indicator : m_indicators) {
    indicator->Draw(window);
  }
}

void Menu::Update(Input input) {
  for (auto &button : m_buttons) {
    button->Update(input);
  }
}