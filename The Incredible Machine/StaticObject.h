#pragma once
#include "stdafx.h"
class StaticObject
{
private:

	sf::RectangleShape object;

public:
	StaticObject(float x, float y, float width, float height);
	void render(sf::RenderTarget& target);
};

