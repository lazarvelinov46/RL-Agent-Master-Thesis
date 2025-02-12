#include "stdafx.h"
#include "AgentRL.h"

bool AgentRL::hasGear(std::vector<StaticObject*> objects)
{
	for (const StaticObject* object : objects) {
		if (object->getObjectType() == StaticObjectType::GEAR) {
			return true;
		}
	}
	return false;
}

void AgentRL::generateAction()
{
	this->action = static_cast<AgentAction>(rand() % AgentAction::number);
}

void AgentRL::generateGearPosition()
{
	this->gearPosition = sf::Vector2f(static_cast<float>(rand() % 1000), static_cast<float>(rand() % 800));
}

void AgentRL::generateBeltStart(std::vector<StaticObject*> objects, std::vector<StaticWheel*> wheels)
{
	bool selected = false;
	while (!selected) {
		int select = rand() % 2;
		if (select == 0) {
			int id = rand() % wheels.size();
			this->startBeltPosition = sf::Vector2f(wheels[id]->getGlobalBounds().left, wheels[id]->getGlobalBounds().top);
			selected = true;
		}
		else {
			if (!this->hasGear(objects)) {
				continue;
			}
			int id = rand() % objects.size();
			StaticObject* obj = objects[id];
			while (obj->getObjectType() != StaticObjectType::GEAR) {
				id = rand() % objects.size();
				obj = objects[id];
			}
			this->startBeltPosition = sf::Vector2f(obj->getGlobalBounds().left, obj->getGlobalBounds().top);
			selected = true;
		}
	}
	
}

void AgentRL::generateBeltEnd(std::vector<StaticWheel*> wheels)
{
	int id = rand() % wheels.size();
	this->endBeltPosition = sf::Vector2f(wheels[id]->getGlobalBounds().left, wheels[id]->getGlobalBounds().top);
}

AgentAction AgentRL::getAction()
{
	return this->action;
}

sf::Vector2f AgentRL::getGearPosition()
{
	return this->gearPosition;
}

sf::Vector2f AgentRL::getBeltStartPosition()
{
	return this->startBeltPosition;
}

sf::Vector2f AgentRL::getBeltEndPosition()
{
	return this->endBeltPosition;
}

std::vector<StateRL*> AgentRL::getStates() const
{
	// TODO: insert return statement here
	return this->states;
}

std::vector<ActionRL*> AgentRL::getActions() const
{
	// TODO: insert return statement here
	return this->actions;
}

StateRL* AgentRL::getState(int index) const
{
	return this->states[index];
}

ActionRL* AgentRL::getAction(int index) const
{
	return this->actions[index];
}

ActionRL* AgentRL::getBestAction(int stateIndex) const
{
	double maxQValue = -DBL_MAX;
	int maxActionIndex = -1;
	for (int i = 0;i < this->QValues[stateIndex].size();i++) {
		if (this->QValues[stateIndex][i] > maxQValue) {
			maxQValue = this->QValues[stateIndex][i];
			maxActionIndex = i;
		}
	}
	return this->actions[maxActionIndex];
}

int AgentRL::getStateIndex(const StateRL* state)
{
	for (int i = 0;i < this->states.size();i++) {
		if (this->states[i] == state) {
			return i;
		}
	}
	return -1;
}

int AgentRL::getActionIndex(const ActionRL* action)
{
	for (int i = 0;i < this->actions.size();i++) {
		if (this->actions[i] == action) {
			return i;
		}
	}
	return -1;
}
