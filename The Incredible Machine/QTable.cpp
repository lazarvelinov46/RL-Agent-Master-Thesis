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
	double initVal = 0.5;
	this->values.assign(numberOfStates, std::vector<double>(numberOfActions, 0.5));
	this->visits.assign(numberOfStates, std::vector<int>(numberOfActions, 0));
	this->alpha = 0.2;
	this->gamma = 0.9;
	this->numStates = numberOfStates;
	this->numActions = numberOfActions;
	this->filename = filename;
	for (int i = 0;i < this->numActions;i++) {
		this->validActions.push_back(i);
	}
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
	int n = this->visits[stateId][actionId]++;
	double localAlphaFromVisits = 1.0 / (1.0 + (double)n);
	double localAlpha = std::min(this->alpha, localAlphaFromVisits);
	double qValue = this->getQValue(stateId, actionId);
	double maxNextQ = 0.0;
	if (!nextIsTerminal&&nextStateId>=0&&nextStateId<this->numStates) {
		maxNextQ = *std::max_element(this->values[nextStateId].begin(), this->values[nextStateId].end());
	}
	double newValue = qValue + localAlpha*(reward + gamma * maxNextQ - qValue);
	this->values[stateId][actionId] = newValue;
}

//maybe not all actions but best possible
int QTable::getAction(int stateId, double epsilon,std::unordered_set<int> forbiddenActions)
{
	if (stateId < 0 || stateId >= numStates)return 0;
	std::vector<int> allowedActions;
	allowedActions.reserve(this->numActions);
	for (int i = 0;i < this->numActions;i++) {
		if (forbiddenActions.find(i) == forbiddenActions.end()) {
			allowedActions.push_back(i);
		}
	}
	if (allowedActions.empty())return -1;
	std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
	double sample = uniformDist(rng);
	if (sample < epsilon) {
		std::uniform_int_distribution<int> actionDist(0, allowedActions.size() - 1);
		return allowedActions[actionDist(rng)];
	}
	else {
		//std::vector<double> row = this->values[stateId];
		double maxVal = -std::numeric_limits<double>::infinity();

		for (int a : allowedActions) {
			double val = this->values[stateId][a];
			if (val > maxVal)maxVal = val;
		}
		std::vector<int> bestActions;
		bestActions.reserve(allowedActions.size());
		/*
		if (allowedActions.size() < this->numActions) {
			std::cout << "DES" << std::endl;
		}
		*/
		for (int a:allowedActions) {
			if (std::abs(this->values[stateId][a] - maxVal)<1e-9)bestActions.push_back(a);
		}
		if (bestActions.empty()) {
			std::uniform_int_distribution<int> actionDist(0, allowedActions.size() - 1);
			return allowedActions[actionDist(rng)];
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

void QTable::saveQTableCSV(const std::string& filename,int iteration,double alpha,double epsilon) {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) return;

	outFile << "iteration," << iteration << "\n";
	outFile << "alpha," << alpha << "\n";
	outFile << "epsilon," << epsilon << "\n";
	outFile << "qtable\n";

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

void QTable::updateValidActions(int gearsPlaced, int beltsPlaced)
{
	this->validActions.clear();
	//3 is max number of gears
	if (gearsPlaced <3) {
		//3 is max num of belts
		if (beltsPlaced < 3) {
			for (int i = 0;i < this->numActions;i++) {
				this->validActions.push_back(i);
			}
		}
		else {
			//12 is number of belt actions
			for (int i = 0;i < this->numActions-12;i++) {
				this->validActions.push_back(i);
			}
		}
	}
	else {
		//3 is max num of belts
		if (beltsPlaced < 3) {
			for (int i = 19*14;i < this->numActions;i++) {
				this->validActions.push_back(i);
			}
		}
	}
}

std::pair<double, double> QTable::getQStats() const
{
	double qmax = -std::numeric_limits<double>::infinity();
	double qsum = 0;
	long count = 0;
	for (int i = 0; i < this->numStates; i++) {
		for (int j = 0; j < this->numActions; j++) {
			qsum += this->values[i][j];
			count++;
			if (this->values[i][j] > qmax)qmax = this->values[i][j];
		}
	}
	double qmean = (count > 0) ? (qsum / (double)count) : 0.0;
	if (qmax == -std::numeric_limits<double>::infinity())qmax = 0.0;
	return { qmax,qmean };
}

int QTable::getUniqueVisitsCount() const
{
	int count = 0;
	for (int i = 0; i < this->numStates; i++) {
		for (int j = 0; j < this->numActions; j++) {
			if (this->visits[i][j] > 0)count++;
		}
	}
	return count;
}

bool QTable::loadQTableCSV(std::istream &in)
{
	this->values.clear();
	this->visits.clear();
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty())continue;
		std::stringstream ss(line);
		std::vector<double> row;
		std::string cell;
		while (std::getline(ss, cell, ',')) {
			if(!cell.empty())row.push_back(std::stod(cell));
		}
		if(!row.empty())this->values.push_back(row);
	}
	if (this->values.empty())return false;
	this->numStates = this->values.size();
	this->numActions = values.empty() ? 0 : values[0].size();
	this->visits.assign(this->numStates, std::vector<int>(this->numActions, 0));
	std::cout << "Loaded QTable from " << filename << std::endl;
	return true;
}
