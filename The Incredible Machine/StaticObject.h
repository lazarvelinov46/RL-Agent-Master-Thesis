#pragma once
#include "stdafx.h"
class StaticObject
{
private:

	sf::RectangleShape object;

public:
	StaticObject(float x, float y, float width, float height,sf::Color color);
	StaticObject(const StaticObject& object);
	StaticObject(const sf::Sprite& sprite);
	void render(sf::RenderTarget& target);
	
	sf::FloatRect getGlobalBounds()const;
};

