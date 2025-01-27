#pragma once

#include "stdafx.h"

class StaticWheel
{
private:

	sf::CircleShape object;

public:
	StaticWheel(float x, float y, float radius, sf::Color color);
	StaticWheel(const StaticWheel& object);
	StaticWheel(const sf::Sprite& sprite);
	void render(sf::RenderTarget& target);

	sf::FloatRect getGlobalBounds()const;
};

