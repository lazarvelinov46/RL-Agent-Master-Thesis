#pragma once

#include "State.h"
#include "MediumLevel.h"
#include "Menu.h"
#include "AgentRL.h"
#include "DQNAgent.h"

/**
* @class GameDQN
* @brief Represents game state controlled by Deep Q Learning agent
*
* Simulates same levels as human and Q learning gameplay
* Actions selected by DQN agent
* Updates simulation state based on DQN decision making and feedback from the environment
*/
class GameDQN : public State
{
public:
	/// <summary>
	/// Initializes everything needed: GameAI state, QTable, level and RLAgent
	/// </summary>
	explicit GameDQN(LevelDifficulty difficulty);
	/// <summary>
	/// Frees everything from memory
	/// </summary>
	~GameDQN();
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
	/// Updates forbidden actions based on gears and belts placed
	/// </summary>
	/// <param name="gearsPlaced">number of gears placed</param>
	/// <param name="beltsPlaced">number of belts placed</param>
	void updateForbiddenActions(int gearsPlaced, int beltsPlaced);
private:
	//UI panel on right side on the screen (consists of user action and resource elements
	sf::RectangleShape panel_;
	//Play button that starts the simulation
	sf::RectangleShape playButton_;
	//Main game screen
	sf::RectangleShape gameScreen_;
	//Font used in UI text
	sf::Font font_;
	//Text element displayed on play button
	sf::Text playButtonText_;

	//Selected level
	LevelDifficulty selectedLevel_;
	//Level object managing game and logic of specific level
	Level* level_=nullptr;
	//Next state object to transition to
	State* nextState_ = nullptr;
	//test
	ActionRL* actionFunctions_ = nullptr;

	//Flag which indicates if simulation is running
	bool isPlaying_ = false;

	//Current stateId used in QLearning
	int stateId_ = -1;
	//Next state ID after action execution
	int nextStateId_ = 1;
	//ID of currently executed action
	int actionId_ = -1;
	/// <summary>
	/// Id of last executed action
	/// waits until state is changed to update qtable
	/// because noaction is -1 (almost every iteration between regular action and state changed)
	/// we need to store last regular action in order to correctly update qtable
	/// </summary>
	int lastExecutedAction_ = -1;
	//current sum of rewards before state change
	float currentReward_ = 0.0f;
	//total reward gained in episode
	float cumulativeReward_ = 0.0f;
	//Indicates if new action is to be selected
	bool selectAction_ = false;
	//sets of actions which are forbidden
	std::unordered_set<int> forbiddenActions_;
	//vector of current state (for DQN input)
	std::vector<float> currentStateVector_;

	//DQN agent ---------------------------------------------------------------
	DQNAgent* agent_ = nullptr;

	//numberOfStates
	int statesNum_=0;

	//numberOfActions
	int actionsNum_=0;

	//Logging
	std::chrono::time_point<std::chrono::system_clock> episodeStart_;
	int episodeCount_ = 0;

	// Helpers ----------------------------------------------------------------
	void initUI();
	void initTextures();
	void initDQNAgent();
	//Resets game resources before new iteration of the game
	void resetResources();
	/// <summary>
	/// Selects action from QTable according to epsilon-greedy policy
	/// Updates simulation based on selected action
	/// </summary>
	void updateActionState();
	/// <summary>
	/// Updates nextStateId if environment state changes
	/// </summary>
	bool updateState();

	void appendLog(int ep, double eps, float totalReward,
		int win, int steps, double duration);

	static std::string isoNow();
};

