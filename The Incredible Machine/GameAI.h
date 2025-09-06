#pragma once

#include "State.h"
#include "MediumLevel.h"
#include "Menu.h"
#include "AgentRL.h"
#include "QTable.h"

typedef struct Transition {
	int stateId;
	int actionId;
	double reward;
	int nextStateId;
}Transition;

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
	double E_START = 0.6;
	double E_END = 0.05;
	int E_DECAY = 5000;
	double ALPHA_START = 0.3;
	double ALPHA_END = 0.05;
	int ALPHA_DECAY = 10000;
	double GAMMA = 0.95;
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

	//Selected level
	LevelDifficulty selectedLevel;
	//Level object managing game and logic of specific level
	Level* level;
	//Next state object to transition to
	State* nextState;
	//Pointer to the RL agent performing actions
	AgentRL* agent;

	//test
	ActionRL* actionFunctions;

	//Flag which indicates if simulation is running
	bool isPlaying = false;
	//Indicates if new action is to be selected
	bool selectAction = false;
	//Number of iterations since the beggining of game
	int iterations = 0;
	//game over indication
	bool gameOver = false;
	//sets of actions which are forbidden
	std::unordered_set<int> forbiddenActions;

	//episode start time for keeping record
	std::chrono::time_point<std::chrono::system_clock> episodeStartTime;

	//Resets game resources before new iteration of the game
	void resetResources();

	//numberOfStates
	int statesNum;

	//numberOfActions
	int actionsNum;

	//Current stateId used in QLearning
	int stateId;
	//Next state ID after action execution
	int nextStateId;
	//ID of currently executed action
	int actionId;
	//current sum of rewards before state change
	float currentReward = 0.0f;
	//total reward gained in episode
	float cumulativeReward = 0.0f;
	/// <summary>
	/// Id of last executed action
	/// waits until state is changed to update qtable
	/// because noaction is -1 (almost every iteration between regular action and state changed)
	/// we need to store last regular action in order to correctly update qtable
	/// </summary>
	int lastExecutedAction;
	//QTable for updating and retreiving QValues
	QTable table;

	//Vector with all transitions within one episode
	std::vector<Transition*> episode;

	bool loadQTableFromFile(const std::string& filename);

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
	//Initializes QTable
	void initQTable();
	/// <summary>
	/// Selects action from QTable according to epsilon-greedy policy
	/// Updates simulation based on selected action
	/// </summary>
	void updateActionState();
	/// <summary>
	/// Updates nextStateId if environment state changes
	/// </summary>
	bool updateState();
	/// <summary>
	/// Gets timestape of now
	/// </summary>
	/// <returns>timestape of now in string format</returns>
	static std::string isoTimestampNow();
	void appendEpisodeLog(int episode, double eps, double alphaCap,
		double totalReward, int win, int steps, double duration_s, 
		int unique_sa_visited, double q_max, double q_mean);
public:
	/// <summary>
	/// Initializes everything needed: GameAI state, QTable, level and RLAgent
	/// </summary>
	GameAI(LevelDifficulty difficulty);
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
	/// <summary>
	/// decays value lineary
	/// </summary>
	static double linearDecay(double start, double end, int episode, int decayEpisodes);
	/// <summary>
	/// Updates forbidden actions based on gears and belts placed
	/// </summary>
	/// <param name="gearsPlaced">number of gears placed</param>
	/// <param name="beltsPlaced">number of belts placed</param>
	void updateForbiddenActions(int gearsPlaced, int beltsPlaced);
};

