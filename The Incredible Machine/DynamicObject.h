#pragma once
#include "stdafx.h"
class DynamicObject
{
private:
	sf::Sprite sprite;
	sf::Vector2f velocity;
public:
	DynamicObject(float x, float y, const sf::Texture& texture);
	void update();
	void render(sf::RenderTarget& target);
};

