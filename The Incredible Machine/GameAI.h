#pragma once

#include "State.h"
#include "Level.h"
#include "Menu.h"
#include "AgentRL.h"
#include "QTable.h"

/**
* @class GameAI
* @brief Represents game state controlled by reinforcement learning agent
* 
* Simulates same levels as human gameplay
* Actions selected by AI agent, not by the human
* Updates simulation state based on AI decision making and feedback from the environment
*/
class GameAI : public State
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
	//Pointer to the RL agent performing actions
	AgentRL* agent;

	//Flag which indicates if simulation is running
	bool isPlaying = false;

	//Current stateId used in QLearning
	int stateId;
	//Next state ID after action execution
	int nextStateId;
	//ID of executed action
	int actionId;
	//QTable for updating and retreiving QValues
	QTable table;

	//Loads font from file
	void initFont();
	//Initializes text elements
	void initText();
	//Initializes UI panel and its elements
	void initPanel();
	//Initializes game screen
	void initGameScreen();

	/// <summary>
	/// Selects action from QTable according to epsilon-greedy policy
	/// Updates simulation based on selected action
	/// </summary>
	void selectAction();
	/// <summary>
	/// Updates nextStateId if environment state changes
	/// </summary>
	void updateState();
public:
	/// <summary>
	/// Initializes everything needed: GameAI state, QTable, level and RLAgent
	/// </summary>
	GameAI();
	/// <summary>
	/// Frees everything from memory
	/// </summary>
	~GameAI();
	/// <summary>
	/// Handles keyboard and mouse events
	/// </summary>
	/// <param name="window">Window on which events are polled</param>
	void handleInput(sf::RenderWindow& window)override;
	/// <summary>
	/// Updates AI decision making and game simulation (physics)
	/// </summary>
	/// <param name="deltaTime">Time since last update</param>
	void update(float deltaTime = 0)override;
	/// <summary>
	/// Renders the game screen
	/// </summary>
	/// <param name="target">Render target on which render elements are drawn</param>
	void render(sf::RenderTarget& target)override;

	/// <summary>
	/// Gets next state to transition to from current one
	/// </summary>
	/// <returns>Pointer to the next state or nullptr</returns>
	State* getNextState()override;
};

