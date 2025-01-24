#include "stdafx.h"
#include "StaticObject.h"

StaticObject::StaticObject(float x, float y, float width, float height,sf::Color color)
{
	this->object.setSize(sf::Vector2f(width, height));
	this->object.setPosition(x, y);
	this->object.setFillColor(color);
}

StaticObject::StaticObject(const StaticObject& object)
{
	this->object.setSize(object.object.getSize());
	this->object.setPosition(object.object.getPosition());
	this->object.setFillColor(object.object.getFillColor());
}

StaticObject::StaticObject(const sf::Sprite& sprite)
{
	this->object.setSize(sprite.getGlobalBounds().getSize());
	this->object.setPosition(sprite.getGlobalBounds().getPosition());
	this->object.setTexture(sprite.getTexture());
}

void StaticObject::render(sf::RenderTarget& target)
{
	target.draw(this->object);
}
