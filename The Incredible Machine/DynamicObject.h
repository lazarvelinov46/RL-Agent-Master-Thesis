#pragma once
#include "stdafx.h"

/**
* @class DynamicObject
* @brief Represents dynamic object (that changes position) during the game
* In this instance it is a ball
*/
class DynamicObject
{
private:
	//TODO: Sprite which will represent the ball
	sf::Sprite sprite;
	//Visual shape that represents dynamic object
	sf::CircleShape shape;
	//Object velocity vector
	sf::Vector2f velocity;
public:
	/// <summary>
	/// TODO: Constructor using the texture
	/// </summary>
	/// <param name="x">Object position on X axis</param>
	/// <param name="y">Object position on Y axis</param>
	/// <param name="texture">Texture for object's sprite</param>
	DynamicObject(float x, float y, const sf::Texture& texture);
	/// <summary>
	/// Constructor using circle shape
	/// </summary>
	/// <param name="x">Object position on X axis</param>
	/// <param name="y">Object position on Y axis</param>
	/// <param name="radius">Radius of the circle</param>
	/// <param name="color">Fill color of the circle</param>
	DynamicObject(float x, float y, float radius, sf::Color color);
	/// <summary>
	/// Updates object position based on its velocity
	/// </summary>
	void update();
	/// <summary>
	/// Draws the object on given render target
	/// </summary>
	/// <param name="target">RenderTarget on which to draw on</param>
	void render(sf::RenderTarget& target);
	/// <summary>
	/// Adds force to object to change its velocity
	/// </summary>
	/// <param name="force">Vector to add to velocity</param>
	void move(const sf::Vector2f& force);

	sf::Vector2f getVelocity()const;
	sf::Vector2f getPosition()const;
	sf::CircleShape getShape()const;

	void setVelocity(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);

	/// <summary>
	/// Returns object's bounding box
	/// </summary>
	/// <returns>FloacRect of object's boundaries</returns>
	sf::FloatRect getGlobalBounds()const;
};

