#pragma once

#include "stdafx.h"
#include "TextureManager.h"

/**
* @class StaticWheel
* @brief Represents static wheel on platforms in game
* 
* Used for transfering movement to platform
*/
class StaticWheel
{
private:
	//Sprite which will represent the wheel
	sf::Sprite sprite;
	/// <summary>
	/// Circle shape which is used to render the "wheel"
	/// TODO: Implement it with some sprite and texture
	/// </summary>
	sf::CircleShape object;
	/// <summary>
	/// Indicator if the belt is attached to wheel
	/// </summary>
	bool attached;
	//sets texture of an object depending on object type
	void setTexture();

	//wheel size in game
	static const float IN_GAME_WHEEL_SIZE;

public:
	/// <summary>
	/// Creates static wheel with position, size and color
	/// </summary>
	/// <param name="x">Position on X axis</param>
	/// <param name="y">Position on Y axis</param>
	/// <param name="radius">Radius of the wheel</param>
	/// <param name="color">Fill color of the wheel</param>
	StaticWheel(float x, float y, float radius);
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="object">object from which to copy from</param>
	StaticWheel(const StaticWheel& object);
	/// <summary>
	/// Constructor that creates wheel with sprite
	/// </summary>
	/// <param name="sprite">Sprite that defines the wheel</param>
	StaticWheel(float x, float y);
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
	/// <summary>
	/// For gear objects only, sets if belt is attached or not
	/// </summary>
	/// <param name="attached">true if belt attached, otherwise false</param>
	void setAttached(bool attached);
	/// <summary>
	/// For gear objects retreives info if belt is attached to them
	/// </summary>
	/// <returns>true if attached false otherwise</returns>
	bool getAttached()const;
};

