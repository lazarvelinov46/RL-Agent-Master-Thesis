#include "stdafx.h"
#include "QTable.h"

QTable::QTable()
{
	this->alpha = 0.1;
	this->gamma = 0.9;
}

QTable::QTable(int numberOfStates, int numberOfActions,std::string filename)
{
	for (int i = 0;i < numberOfStates;i++) {
		for (int j = 0;j < numberOfActions;j++) {
			this->values[i][j] = 0.0;
		}
	}
	this->alpha = 0.1;
	this->gamma = 0.9;
	this->numStates = numberOfStates;
	this->numActions = numberOfActions;
	this->filename = filename;
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

void QTable::printTable(const std::string& filename,int iteration)
{
	std::ofstream outFile(filename, std::ios::app);
	if (!outFile.is_open()) return;

	outFile << "Iteration: " << iteration << "\n";
	for (int s = 0; s < this->numStates; ++s) {
		for (int a = 0; a < this->numActions; ++a) {
			outFile << std::fixed << std::setprecision(2) << this->values[s][a] << " ";
		}
		outFile << "\n";
	}
	outFile << "-------------------------------\n";
	outFile.close();
}

void QTable::saveQTableCSV(const std::string& filename) {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) return;

	for (int s = 0; s < this->numStates; ++s) {
		for (int a = 0; a < this->numActions; ++a) {
			outFile << this->values[s][a];
			if (a != this->numActions - 1)
				outFile << ",";
		}
		outFile << "\n";
	}

	outFile.close();
}
