#pragma once

#include "Game.h"
#include "GameAI.h"

/// <summary>
/// Enum which represents menu options
/// </summary>
enum MenuOption{
	//Manualy playing the game
	PLAYER,
	//AI playing the game
	AI,
	//Level selector
	LEVEL,
	//Exit
	QUIT
};

/// <summary>
/// Enum which represents selected level difficulty
/// </summary>
enum LevelDifficulty {
	EASY,
	MEDIUM,
	HARD,
	NUM
};

/**
* @class Menu
* @brief Implements menu state of the game
* 
* Screen where player chooses between options given on menu
* Inherits abstract state class
*/
class Menu : public State
{
private:
	//Font used for rendering text
	sf::Font font;
	//Text element which represents title
	sf::Text title;
	//Text UI elements which represent options
	std::vector<sf::Text> options;
	//Currently highlighted menu entry
	MenuOption selectedOption;
	//Selected level (difficulty)
	LevelDifficulty selectedDifficulty;
	//Next state for transition
	State* nextState;
	/// <summary>
	/// Loads font from assets
	/// </summary>
	void initFont();
	/// <summary>
	/// Sets up title and menu entries text
	/// </summary>
	void initText();
public:
	/// <summary>
	/// Initializes components (font and text)
	/// </summary>
	Menu();
	/// <summary>
	/// Deletes nextState pointer
	/// </summary>
	~Menu();
	//getters
	/// Returns currently selected menu option
	const MenuOption getSelectedOption()const { return this->selectedOption; }
	/// <summary>
	/// Handles user input to navigate between menu entries
	/// </summary>
	/// <param name="window">
	/// Reference to render window for polling events
	/// </param>
	void handleInput(sf::RenderWindow& window)override;
	/// <summary>
	/// Updates visual state of menu
	/// </summary>
	/// <param name="deltaTime">
	/// Time since last frame
	/// </param>
	void update(float deltaTime=0)override;
	/// <summary>
	/// Draws menu title and options
	/// </summary>
	/// <param name="target">
	/// Render target on which to draw on
	/// </param>
	void render(sf::RenderTarget& target)override;
	/// <summary>
	/// Gets next state to switch to
	/// </summary>
	/// <returns>
	/// Pointer to next state instance or nullptr
	/// </returns>
	State* getNextState()override;
};

