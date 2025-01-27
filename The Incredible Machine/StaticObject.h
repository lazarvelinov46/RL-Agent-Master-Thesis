#pragma once
#include "stdafx.h"

enum StaticObjectType {PLATFORM,GEAR};

class StaticObject
{
private:

	sf::RectangleShape object;
	StaticObjectType type;

public:
	StaticObject(float x, float y, float width, float height,sf::Color color,StaticObjectType t);
	StaticObject(const StaticObject& object);
	StaticObject(const sf::Sprite& sprite,StaticObjectType t);
	void render(sf::RenderTarget& target);
	
	sf::FloatRect getGlobalBounds()const;
	StaticObjectType getObjectType()const { return this->type; }
};

