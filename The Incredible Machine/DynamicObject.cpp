#include "stdafx.h"
#include "DynamicObject.h"

const float DynamicObject::IN_GAME_BALL_SIZE = 50.0;

DynamicObject::DynamicObject(float x, float y, bool goalBall)
{
    std::string ballTexture = goalBall ? "goal_ball.png" : "ball.png";
    sf::Texture& tex = TextureManager::getTexture(TextureManager::getTextureFolderPath()+ballTexture);
    this->sprite.setTexture(tex);
    this->sprite.setPosition(x, y);
    float scaleX = DynamicObject::IN_GAME_BALL_SIZE / tex.getSize().x;
    float scaleY = DynamicObject::IN_GAME_BALL_SIZE / tex.getSize().y;
    this->sprite.setScale(scaleX, scaleY);
    this->velocity = sf::Vector2f(0, 0);
    this->isSprite = true;
}

DynamicObject::DynamicObject(float x, float y, float radius, sf::Color color)
{
    this->shape.setPosition(x, y);
    this->shape.setRadius(radius);
    this->shape.setFillColor(color);
    this->velocity = sf::Vector2f(0, 0);
    this->isSprite = false;
}

void DynamicObject::render(sf::RenderTarget& target)
{
    this->isSprite?target.draw(this->sprite):target.draw(this->shape);
}

sf::Vector2f DynamicObject::getVelocity() const
{
    return this->velocity;
}

sf::Vector2f DynamicObject::getPosition() const
{
    return this->isSprite ? this->sprite.getPosition() : this->shape.getPosition();
}

sf::CircleShape DynamicObject::getShape() const
{
    return this->shape;
}

sf::Sprite DynamicObject::getSprite() const
{
    return this->sprite;
}

void DynamicObject::setVelocity(sf::Vector2f velocity)
{
    this->velocity = velocity;
}

void DynamicObject::setPosition(sf::Vector2f position)
{
    this->isSprite?this->sprite.setPosition(position):this->shape.setPosition(position);
}

sf::FloatRect DynamicObject::getGlobalBounds()const
{
    return this->isSprite?this->sprite.getGlobalBounds():this->shape.getGlobalBounds();
}
