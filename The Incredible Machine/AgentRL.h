#pragma once

#include "stdafx.h"
#include "StaticObject.h"
#include "StaticWheel.h"
#include "StateRL.h"
#include "ActionRL.h"

/// <summary>
/// Enumerates descrete actions which agent could take
/// </summary>
enum AgentAction  { PLACE_GEAR,PLACE_BELT,START,number };

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

	void generateAction();
	void generateGearPosition();
	void generateBeltStart(std::vector<StaticObject*> objects, std::vector<StaticWheel*> wheels);
	void generateBeltEnd(std::vector<StaticWheel*> wheels);

	AgentAction getAction();
	sf::Vector2f getGearPosition();
	sf::Vector2f getBeltStartPosition();
	sf::Vector2f getBeltEndPosition();

	//q-learning
	std::vector<StateRL*> getStates()const;
	std::vector<ActionRL*> getActions()const;

	StateRL* getState(int index)const;
	ActionRL* getAction(int index)const;
	ActionRL* getBestAction(int stateIndex)const;

	int getStateIndex(const StateRL* state);
	int getActionIndex(const ActionRL* action);
};

