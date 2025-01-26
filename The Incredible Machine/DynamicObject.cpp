#include "stdafx.h"
#include "DynamicObject.h"

DynamicObject::DynamicObject(float x, float y, const sf::Texture& texture)
{
}

DynamicObject::DynamicObject(float x, float y, float radius, sf::Color color)
{
    this->shape.setPosition(x, y);
    this->shape.setRadius(radius);
    this->shape.setFillColor(color);
    this->velocity = sf::Vector2f(0, 0);
}

void DynamicObject::render(sf::RenderTarget& target)
{
    target.draw(this->shape);
}

sf::Vector2f DynamicObject::getVelocity() const
{
    return this->velocity;
}

sf::Vector2f DynamicObject::getPosition() const
{
    return this->shape.getPosition();
}

sf::CircleShape DynamicObject::getShape() const
{
    return this->shape;
}

void DynamicObject::setVelocity(sf::Vector2f velocity)
{
    this->velocity = velocity;
}

const sf::FloatRect& DynamicObject::getGlobalBounds()
{
    return this->sprite.getGlobalBounds();
}
