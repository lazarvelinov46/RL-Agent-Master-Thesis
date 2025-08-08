#include "stdafx.h"
#include "StaticWheel.h"

const float StaticWheel::IN_GAME_WHEEL_SIZE = 30.0;

void StaticWheel::setTexture()
{
	sf::Texture& tex = TextureManager::getTexture("assets/Textures/wheel.png");
	//tex.setRepeated(true);
	this->object.setTexture(&tex);
	this->object.setTextureRect(sf::IntRect(0, 0, this->object.getRadius() * 2.2, this->object.getRadius() * 2.2));
}

StaticWheel::StaticWheel(float x, float y, float radius)
{
	this->object.setPosition(x, y);
	this->object.setRadius(radius);
	this->object.setOrigin(radius, radius);
	this->setTexture();
}

StaticWheel::StaticWheel(float x,float y)
{
	sf::Texture& tex = TextureManager::getTexture(TextureManager::getTextureFolderPath() + "wheel_full.png");
	this->sprite.setTexture(tex);
	this->sprite.setPosition(x, y-StaticWheel::IN_GAME_WHEEL_SIZE / 2);
	float scaleX = StaticWheel::IN_GAME_WHEEL_SIZE / tex.getSize().x;
	float scaleY = StaticWheel::IN_GAME_WHEEL_SIZE / tex.getSize().y;
	this->sprite.setScale(scaleX, scaleY);
}

void StaticWheel::render(sf::RenderTarget& target)
{
	this->sprite.getTexture()?target.draw(this->sprite):target.draw(this->object);
}

sf::FloatRect StaticWheel::getGlobalBounds() const
{
	return this->sprite.getTexture()?this->sprite.getGlobalBounds() : this->object.getGlobalBounds();
}
void StaticWheel::setAttached(bool attached)
{
	this->attached = attached;
}

bool StaticWheel::getAttached()const
{
	return this->attached;
}