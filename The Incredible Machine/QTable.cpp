#include "stdafx.h"
#include "QTable.h"

QTable::QTable()
{
	this->alpha = 0.1;
	this->gamma = 0.9;
}

QTable::QTable(int numberOfStates, int numberOfActions,std::string filename):
	rng(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))
{
	this->values.assign(numberOfStates, std::vector<double>(numberOfActions, 0.0));
	this->alpha = 0.1;
	this->gamma = 0.9;
	this->numStates = numberOfStates;
	this->numActions = numberOfActions;
	this->filename = filename;
}

double QTable::getQValue(int stateId, int actionId) const
{
	if (stateId < 0 || stateId >= numStates)return 0.0;
	if (actionId < 0 || actionId >= numActions)return 0.0;
	return this->values[stateId][actionId];
}

void QTable::updateQValue(int stateId, int actionId, double reward, int nextStateId, bool nextIsTerminal)
{
	if (stateId < 0 || stateId >= numStates)return;
	if (actionId < 0 || actionId >= numActions)return;
	double qValue = this->getQValue(stateId, actionId);
	double maxNextQ = 0.0;
	if (!nextIsTerminal&&nextStateId>=0&&nextStateId<this->numStates) {
		maxNextQ = *std::max_element(this->values[nextStateId].begin(), this->values[nextStateId].end());
	}
	double newValue = qValue + this->alpha*(reward + gamma * maxNextQ - qValue);
	this->values[stateId][actionId] = newValue;
}

//maybe not all actions but best possible
int QTable::getAction(int stateId, double epsilon)
{
	if (stateId < 0 || stateId >= numStates)return 0;
	std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
	double sample = uniformDist(rng);
	if (sample < epsilon) {
		std::uniform_int_distribution<int> actionDist(0, numActions - 1);
		return actionDist(rng);
	}
	else {
		std::vector<double> row = this->values[stateId];
		double maxVal = *std::max_element(this->values[stateId].begin(), this->values[stateId].end());

		std::vector<int> bestActions;
		bestActions.reserve(numActions);

		for (int i = 0; i < numActions; i++) {
			if (this->values[stateId][i] == maxVal)bestActions.push_back(i);
		}
		if (bestActions.empty()) {
			std::uniform_int_distribution<int> actionDist(0, numActions - 1);
			return actionDist(rng);
		}
		std::uniform_int_distribution<int> bestActionsDist(0, bestActions.size() - 1);
		return bestActions[bestActionsDist(rng)];
		/*
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
		*/
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
