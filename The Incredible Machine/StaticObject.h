#pragma once
#include "stdafx.h"

/// <summary>
/// Types of static objects that could appear on level
/// </summary>
enum StaticObjectType {
	PLATFORM,
	GEAR,
	FLOOR,
	WALL,
	GOAL
};

/// <summary>
/// @class StaticObject
/// @brief Reprents all static objects which could be found/put on game screen
/// 
/// Managing rendering, moving (rotating), collision
/// </summary>
class StaticObject
{
private:
	//Rectangle shape for rendering of object
	sf::RectangleShape object;
	//Object type
	StaticObjectType type;
	//Indication if object is currently moving (rotating)
	bool moving = false;
	//Speed of moving (rotating)
	float speed = 0.f;


public:
	/// <summary>
	/// Creates static object with given position, size, color and type
	/// </summary>
	/// <param name="x">Position on X axis</param>
	/// <param name="y">Position on Y axis</param>
	/// <param name="width">Width of an object</param>
	/// <param name="height">Height of an object</param>
	/// <param name="color">Color of an object</param>
	/// <param name="t">Object type</param>
	StaticObject(float x, float y, float width, float height,sf::Color color,StaticObjectType t);
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="object">object from which to copy</param>
	StaticObject(const StaticObject& object);
	/// <summary>
	/// Creates object using sprite
	/// </summary>
	/// <param name="sprite">Sprite source</param>
	/// <param name="t">Object type</param>
	StaticObject(const sf::Sprite& sprite,StaticObjectType t);
	/// <summary>
	/// Draws the object on specific render target
	/// </summary>
	/// <param name="target">Target on which to draw on</param>
	void render(sf::RenderTarget& target);
	
	/// <summary>
	/// Marks the object as moving (rotating) and sets its speed
	/// </summary>
	/// <param name="speed">Rotating speed of an object</param>
	void move(float speed);
	/// <summary>
	/// Stops the moving (rotating) of an object
	/// </summary>
	void stop();
	/// <summary>
	/// Retrieves global bounds of rectangle around object for collision checks
	/// </summary>
	/// <returns>FloatRect object of objects bounds</returns>
	sf::FloatRect getGlobalBounds()const;
	/// <summary>
	/// Retrieves info if object is rotating
	/// </summary>
	/// <returns>true if rotating, otherwise false</returns>
	bool getMoving()const { return this->moving; }
	/// <summary>
	/// Returns rotating speed of an object
	/// </summary>
	/// <returns>Speed value as floar</returns>
	float getSpeed()const { return this->speed; }
	/// <summary>
	/// Retrieves object type
	/// </summary>
	/// <returns>StaticObjectType enum value</returns>
	StaticObjectType getObjectType()const { return this->type; }
};

