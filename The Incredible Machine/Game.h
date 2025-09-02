#pragma once
#include "State.h"
#include "MediumLevel.h"
#include "Menu.h"

/**
* @class Game
* @brief Represents game state when user plays
* Handles rendering, logic, interactions etc.
* Initializes right game panel and handles transitions between states
*/
class Game : public State
{
private:
	//UI panel on right side on the screen (consists of user action and resource elements
	sf::RectangleShape panel;
	//Play button that starts the simulation
	sf::RectangleShape playButton;
	//Font used in UI text
	sf::Font font;
	//Text element displayed on play button
	sf::Text playButtonText;
	//Main game screen
	sf::RectangleShape gameScreen;

	//Level object managing game and logic of specific level
	Level level;
	//Next state object to transition to
	State* nextState;
	
	//Flag which indicates if simulation is running
	bool isPlaying = false;

	//Loads font from file
	void initFont();
	//Initializes text elements
	void initText();
	//Initializes UI panel and its elements
	void initPanel();
	//Initializes game screen
	void initGameScreen();
	//Initializes textures
	void initTextures();
public:
	/// <summary>
	/// Initializes all elements of the game
	/// </summary>
	Game();
	/// <summary>
	/// Handles user input events such as pressing keyboard buttons and mouse clicks
	/// </summary>
	/// <param name="window">Game window where events happen</param>
	void handleInput(sf::RenderWindow& window)override;
	/// <summary>
	/// Updates game logic (delegates it to level)
	/// </summary>
	/// <param name="deltaTime">Time elapsed since last update</param>
	void update(float deltaTime=0)override;
	/// <summary>
	/// Renders all game elements
	/// </summary>
	/// <param name="target">RenderTarget on which to draw on</param>
	void render(sf::RenderTarget& target)override;

	/// <summary>
	/// Gets next state to transition to from current one
	/// </summary>
	/// <returns>Pointer to the next state or nullptr</returns>
	State* getNextState()override;
};

