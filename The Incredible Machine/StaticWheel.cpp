#include "stdafx.h"
#include "StaticWheel.h"

StaticWheel::StaticWheel(float x, float y, float radius, sf::Color color)
{
	this->object.setPosition(x, y);
	this->object.setRadius(radius);
	this->object.setFillColor(color);
	this->object.setOrigin(radius, radius);
}

void StaticWheel::render(sf::RenderTarget& target)
{
	target.draw(this->object);
}

sf::FloatRect StaticWheel::getGlobalBounds() const
{
	return this->object.getGlobalBounds();
}
