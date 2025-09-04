#pragma once

#include "stdafx.h"


enum LevelDifficulty {
	EASY,
	MEDIUM,
	HARD,
	NUM
};
/// <summary>
/// Abstract class State which represents all the states (screens)
/// which can be shown in the game
/// </summary>
class State
{
private:
public:
	/// <summary>
	/// Virtual destructor
	/// </summary>
	virtual ~State()=default;
	/// <summary>
	/// Function which handles user input and makes appropriate changes based
	/// on input
	/// </summary>
	/// <param name="window">
	/// Object of RenderWindow class (game window which is currently shown)
	/// </param>
	virtual void handleInput(sf::RenderWindow& window)=0;
	/// <summary>
	/// Function which updates physics of the state (screen)
	/// </summary>
	/// <param name="deltaTime">
	/// Time passed since the last frame
	/// </param>
	virtual void update(float deltaTime=0)=0;
	/// <summary>
	/// Function which renders and updates graphics
	/// </summary>
	/// <param name="target">
	/// Render target on which to draw on
	/// </param>
	virtual void render(sf::RenderTarget& target)=0;

	/// <summary>
	/// Function which returns next state if state is changed
	/// </summary>
	/// <returns>
	/// Pointer to the next state or null if current state continues
	/// </returns>
	virtual State* getNextState() = 0;
};

