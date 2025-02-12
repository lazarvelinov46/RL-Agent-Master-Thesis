#pragma once

#include "stdafx.h"
#include "StaticObject.h"
#include "StaticWheel.h"
#include "StateRL.h"
#include "ActionRL.h"

enum AgentAction  { PLACE_GEAR,PLACE_BELT,START,number };

class AgentRL
{
private:
	AgentAction action;
	sf::Vector2f gearPosition;
	sf::Vector2f startBeltPosition;
	sf::Vector2f endBeltPosition;
	bool hasGear(std::vector<StaticObject*> objects);

	//q-learning
	std::vector<StateRL*> states;
	std::vector<ActionRL*> actions;
	std::vector<std::vector<double>> QValues;

	double alpha = 0.1;
	double gamma = 0.9;
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

