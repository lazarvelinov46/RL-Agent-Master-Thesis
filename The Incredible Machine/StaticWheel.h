#pragma once

#include "stdafx.h"

/**
* @class StaticWheel
* @brief Represents static wheel on platforms in game
* 
* Used for transfering movement to platform
*/
class StaticWheel
{
private:
	/// <summary>
	/// Circle shape which is used to render the "wheel"
	/// TODO: Implement it with some sprite and texture
	/// </summary>
	sf::CircleShape object;

public:
	/// <summary>
	/// Creates static wheel with position, size and color
	/// </summary>
	/// <param name="x">Position on X axis</param>
	/// <param name="y">Position on Y axis</param>
	/// <param name="radius">Radius of the wheel</param>
	/// <param name="color">Fill color of the wheel</param>
	StaticWheel(float x, float y, float radius, sf::Color color);
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="object">object from which to copy from</param>
	StaticWheel(const StaticWheel& object);
	/// <summary>
	/// Constructor that creates wheel with sprite
	/// </summary>
	/// <param name="sprite">Sprite that defines the wheel</param>
	StaticWheel(const sf::Sprite& sprite);
	/// <summary>
	/// Draws the wheel on specific render target
	/// </summary>
	/// <param name="target">Target on which to draw on</param>
	void render(sf::RenderTarget& target);

	/// <summary>
	/// Retrieves global bounds of rectangle that surrounds the wheel
	/// </summary>
	/// <returns>Float rectangle around wheel</returns>
	sf::FloatRect getGlobalBounds()const;
};

