#pragma once

#include "stdafx.h"
#include "StaticObject.h"
#include "StaticWheel.h"
#include "StateRL.h"
#include "ActionRL.h"

/**
* @class AgentRL
* @brief Handles random or policy-driven action generation for RL agent
* 
* Handles action selection
* Generation of positions for gears and belts
*/
class AgentRL
{
private:
	//currently selected action
	AgentAction action;
	//Cordinates for choosen gear placement
	sf::Vector2f gearPosition;
	//Chosen start position for a belt
	sf::Vector2f startBeltPosition;
	//Chosen end position for a belt
	sf::Vector2f endBeltPosition;

	/// <summary>
	/// Checks if there is a gear on placed static objects
	/// </summary>
	/// <param name="objects">Collection of static objects on the level</param>
	/// <returns>True if gear is present, false otherwise</returns>
	bool hasGear(std::vector<StaticObject*> objects);

	//q-learning maybe bad approach
	//all encountered RL states
	std::vector<StateRL*> states;
	//all possible discrete actions
	std::vector<ActionRL*> actions;
	//QTable 
	std::vector<std::vector<double>> QValues;

	//learning rate
	double alpha = 0.1;
	//discount factor
	double gamma = 0.9;
	//exploration rate
	double epsilon = 0.2;

public:
	/// <summary>
	/// Randomly selects one of agent action values
	/// </summary>
	void generateAction();
	/// <summary>
	/// Randomly picks grid position to put gear on
	/// </summary>
	void generateGearPosition();
	/// <summary>
	/// Randomly picks start object (wheel or gear) for a belt to place
	/// </summary>
	/// <param name="objects">Static objects to choose from (gears)</param>
	/// <param name="wheels">Static wheels to choose from</param>
	void generateBeltStart(std::vector<StaticObject*> objects, std::vector<StaticWheel*> wheels);
	/// <summary>
	/// Randomly picks end wheel for belt placement
	/// </summary>
	/// <param name="wheels">Static wheels to chose from</param>
	void generateBeltEnd(std::vector<StaticWheel*> wheels);

	/// <summary>
	/// Returns currently selected agent action
	/// </summary>
	/// <returns>Enum value of currently selected action</returns>
	AgentAction getAction();
	/// <summary>
	/// Retrieves generated gear position
	/// </summary>
	/// <returns></returns>
	sf::Vector2f getGearPosition();
	/// <summary>
	/// Retrieves generated belt start position
	/// </summary>
	/// <returns></returns>
	sf::Vector2f getBeltStartPosition();
	/// <summary>
	/// Retrieves generated belt end position
	/// </summary>
	/// <returns></returns>
	sf::Vector2f getBeltEndPosition();

	//q-learning
	std::vector<StateRL*> getStates()const;
	std::vector<ActionRL*> getActions()const;

	///
	StateRL* getState(int index)const;
	ActionRL* getAction(int index)const;
	ActionRL* getBestAction(int stateIndex)const;

	/// <summary>
	/// Finds the index of given state in internal list
	/// </summary>
	/// <param name="state">State pointer to search</param>
	/// <returns>Index of the state or -1 if not found</returns>
	int getStateIndex(const StateRL* state);
	/// <summary>
	/// Finds the index of given action in internal list
	/// </summary>
	/// <param name="state">Action pointer to search</param>
	/// <returns>Index of the action or -1 if not found</returns>
	int getActionIndex(const ActionRL* action);
};

