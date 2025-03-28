#include "stdafx.h"
#include "QTable.h"

QTable::QTable()
{
	this->alpha = 0.1;
	this->gamma = 0.9;
}

QTable::QTable(int numberOfStates, int numberOfActions)
{
	for (int i = 0;i < numberOfStates;i++) {
		for (int j = 0;j < numberOfActions;j++) {
			this->values[i][j] = 0.0;
		}
	}
	std::cout << this->values[0].size() << std::endl;
	this->alpha = 0.1;
	this->gamma = 0.9;
}

double QTable::getQValue(int stateId, int actionId) const
{
	if (this->values.count(stateId) && this->values.at(stateId).count(actionId)) {
		return this->values.at(stateId).at(actionId);
	}
	return 0.0;
}

void QTable::updateQValue(int stateId, int actionId, double reward, int nextStateId)
{
	double qValue = this->getQValue(stateId, actionId);
	double maxNextQ = -DBL_MAX;
	if (this->values.count(nextStateId)) {
		for (const std::pair<int, double>& action : this->values.at(nextStateId)) {
			maxNextQ = std::max(maxNextQ,action.second);
		}
	}
	double newValue = qValue + this->alpha*(reward + gamma * maxNextQ - qValue);
	this->values[stateId][actionId] = newValue;
}

//maybe not all actions but best possible
int QTable::getAction(int stateId, double epsilon)
{
	std::srand(std::time(0));
	if ((rand()/RAND_MAX) < epsilon) {
		int action = rand() % 252;
		return action;
	}
	else {
		int actionId = 0;
		double maxValue = this->values[stateId][actionId];
		for (int i = 1;i < this->values[stateId].size();i++) {
			double value= this->values[stateId][i];
			if (value > maxValue) {
				maxValue = value;
				actionId = i;
			}
		}
		return actionId;
	}
}
