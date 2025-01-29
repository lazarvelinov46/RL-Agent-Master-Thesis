#pragma once

#include "stdafx.h"

enum AgentAction  { PLACE_GEAR,PLACE_BELT,START,number };

class AgentRL
{
private:
	AgentAction action;
	sf::Vector2f gearPosition;
	sf::Vector2f startBeltPosition;
	sf::Vector2f endBeltPosition;

public:

	void generateAction();

	void getAction();
	void getGearPosition();
	void getBeltStart();
	void getBeltEnd();
};

