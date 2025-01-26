#pragma once
#include "stdafx.h"
class DynamicObject
{
private:
	sf::Sprite sprite;
	sf::CircleShape shape;
	sf::Vector2f velocity;
public:
	DynamicObject(float x, float y, const sf::Texture& texture);
	DynamicObject(float x, float y, float radius, sf::Color color);
	void update();
	void render(sf::RenderTarget& target);
	void move(const sf::Vector2f& force);

	sf::Vector2f getVelocity()const;
	sf::Vector2f getPosition()const;
	sf::CircleShape getShape()const;

	void setVelocity(sf::Vector2f velocity);

	const sf::FloatRect& getGlobalBounds();
};

