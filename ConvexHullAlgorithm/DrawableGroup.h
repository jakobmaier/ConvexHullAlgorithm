#pragma once

#include <SFML/Graphics.hpp>

typedef const sf::Drawable* DrawableGroupResident;

class DrawableGroup : public sf::Drawable {
	std::vector<const sf::Drawable*> drawables;
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	DrawableGroup();
	virtual ~DrawableGroup() override;

	DrawableGroupResident add(const sf::Drawable& drawable);
	const sf::Drawable* remove(DrawableGroupResident id);
	void clear();

	const sf::Drawable* get(DrawableGroupResident id) const;
};