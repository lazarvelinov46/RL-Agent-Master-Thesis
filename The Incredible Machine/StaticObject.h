#pragma once
#include "stdafx.h"

enum StaticObjectType {PLATFORM,GEAR,FLOOR,WALL,GOAL};

class StaticObject
{
private:

	sf::RectangleShape object;
	StaticObjectType type;
	bool moving = false;
	float speed = 0.f;


public:
	StaticObject(float x, float y, float width, float height,sf::Color color,StaticObjectType t);
	StaticObject(const StaticObject& object);
	StaticObject(const sf::Sprite& sprite,StaticObjectType t);
	void render(sf::RenderTarget& target);
	
	void move(float speed);
	sf::FloatRect getGlobalBounds()const;
	bool getMoving()const { return this->moving; }
	float getSpeed()const { return this->speed; }
	StaticObjectType getObjectType()const { return this->type; }
};

