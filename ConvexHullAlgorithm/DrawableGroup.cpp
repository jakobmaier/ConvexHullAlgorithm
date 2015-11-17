#include "DrawableGroup.h"
#include <assert.h>

void DrawableGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for(std::vector<const sf::Drawable*>::const_iterator i = drawables.begin(); i != drawables.end(); ++i) {
		target.draw(**i, states);
	}
}

DrawableGroup::DrawableGroup()
: sf::Drawable() {
}

DrawableGroupResident DrawableGroup::add(const sf::Drawable& drawable) {
	drawables.push_back(&drawable);
	return &drawable;
}

const sf::Drawable* DrawableGroup::remove(DrawableGroupResident id) {
	for(std::vector<const sf::Drawable*>::const_iterator i = drawables.begin(); i != drawables.end(); ++i) {
		if(*i == id) {
			drawables.erase(i);
			return *i;
		}
	}
	return nullptr;
}

void DrawableGroup::clear() {
	drawables.clear();
}

const sf::Drawable* DrawableGroup::get(DrawableGroupResident id) const {
	for(std::vector<const sf::Drawable*>::const_iterator i = drawables.begin(); i != drawables.end(); ++i) {
		if(*i == id) {
			return *i;
		}
	}
	return nullptr;
}

DrawableGroup::~DrawableGroup() {
}
