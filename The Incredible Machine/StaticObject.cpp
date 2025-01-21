#include "stdafx.h"
#include "StaticObject.h"

StaticObject::StaticObject(float x, float y, float width, float height,sf::Color color)
{
	this->object.setSize(sf::Vector2f(width, height));
	this->object.setPosition(x, y);
	this->object.setFillColor(color);
}

void StaticObject::render(sf::RenderTarget& target)
{
	target.draw(this->object);
}
