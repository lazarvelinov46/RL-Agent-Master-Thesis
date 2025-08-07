#include "stdafx.h"
#include "StaticWheel.h"

void StaticWheel::setTexture()
{
}

StaticWheel::StaticWheel(float x, float y, float radius)
{
	this->object.setPosition(x, y);
	this->object.setRadius(radius);
	this->object.setOrigin(radius, radius);
}

StaticWheel::StaticWheel(const sf::Sprite& sprite)
{
	sf::Texture& tex = TextureManager::getTexture("assets/Textures/wheel.png");
	tex.setRepeated(true);
	this->object.setTexture(&tex);
}

void StaticWheel::render(sf::RenderTarget& target)
{
	target.draw(this->object);
}

sf::FloatRect StaticWheel::getGlobalBounds() const
{
	return this->object.getGlobalBounds();
}
