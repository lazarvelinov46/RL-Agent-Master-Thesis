#include "stdafx.h"
#include "StaticObject.h"

void StaticObject::setTexture()
{
	switch (this->type)
	{
	case StaticObjectType::FLOOR: {
		sf::Texture &tex = TextureManager::getTexture("assets/Textures/ground.jpg");
		tex.setRepeated(true);
		this->object.setTexture(&tex);
		this->object.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->object.getGlobalBounds().width),
			static_cast<int>(this->object.getGlobalBounds().height)));
		break;
	}
	case StaticObjectType::PLATFORM: {
		sf::Texture& tex = TextureManager::getTexture("assets/Textures/platform.jpg");
		tex.setRepeated(true);
		this->object.setTexture(&tex);
		this->object.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->object.getGlobalBounds().width),
			static_cast<int>(this->object.getGlobalBounds().height)));
		break;
		break;
	}
	case StaticObjectType::WALL: {
		sf::Texture& tex = TextureManager::getTexture("assets/Textures/basket.jpg");
		tex.setRepeated(true);
		this->object.setTexture(&tex);
		this->object.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->object.getGlobalBounds().width),
			static_cast<int>(this->object.getGlobalBounds().height)));
		break;
	}
	case StaticObjectType::GOAL: {
		sf::Texture& tex = TextureManager::getTexture("assets/Textures/basket.jpg");
		tex.setRepeated(true);
		this->object.setTexture(&tex);
		this->object.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->object.getGlobalBounds().width),
			static_cast<int>(this->object.getGlobalBounds().height)));
		break;
	}
	default:
		break;
	}
}

StaticObject::StaticObject(float x, float y, float width, float height,sf::Color color,StaticObjectType t)
{
	this->object.setSize(sf::Vector2f(width, height));
	this->object.setPosition(x, y);
	this->object.setFillColor(color);
	this->type = t;
	this->setTexture();
}


StaticObject::StaticObject(float x, float y, float width, float height, StaticObjectType t)
{
	this->object.setSize(sf::Vector2f(width, height));
	this->object.setPosition(x, y);
	this->type = t;
	this->setTexture();
}

StaticObject::StaticObject(const StaticObject& object)
{
	this->object.setSize(object.object.getSize());
	this->object.setPosition(object.object.getPosition());
	this->object.setFillColor(object.object.getFillColor());
	this->type = object.getObjectType();
	this->setTexture();
}

StaticObject::StaticObject(const sf::Sprite& sprite,StaticObjectType t)
{
	this->object.setSize(sprite.getGlobalBounds().getSize());
	this->object.setPosition(sprite.getGlobalBounds().getPosition());
	this->object.setTexture(sprite.getTexture());
	this->type = t;
	this->setTexture();
}

void StaticObject::render(sf::RenderTarget& target)
{
	target.draw(this->object);
}

void StaticObject::move(float speed)
{
	this->moving = true;
	this->speed = speed;
}

void StaticObject::stop()
{
	this->moving = false;
	this->speed = 0.0f;
}

sf::FloatRect StaticObject::getGlobalBounds()const
{
	return this->object.getGlobalBounds();
}
