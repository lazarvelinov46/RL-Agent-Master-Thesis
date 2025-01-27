#include "stdafx.h"
#include "StaticObject.h"

StaticObject::StaticObject(float x, float y, float width, float height,sf::Color color,StaticObjectType t)
{
	this->object.setSize(sf::Vector2f(width, height));
	this->object.setPosition(x, y);
	this->object.setFillColor(color);
	this->type = t;
}


StaticObject::StaticObject(const StaticObject& object)
{
	this->object.setSize(object.object.getSize());
	this->object.setPosition(object.object.getPosition());
	this->object.setFillColor(object.object.getFillColor());
}

StaticObject::StaticObject(const sf::Sprite& sprite,StaticObjectType t)
{
	this->object.setSize(sprite.getGlobalBounds().getSize());
	this->object.setPosition(sprite.getGlobalBounds().getPosition());
	this->object.setTexture(sprite.getTexture());
	this->type = t;
}

void StaticObject::render(sf::RenderTarget& target)
{
	target.draw(this->object);
}

sf::FloatRect StaticObject::getGlobalBounds()const
{
	return this->object.getGlobalBounds();
}
