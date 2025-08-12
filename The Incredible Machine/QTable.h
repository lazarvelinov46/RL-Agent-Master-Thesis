#pragma once

#include "stdafx.h"

#define WRONG_GEAR_PLACEMENT -3.0
#define WRONG_BELT_PLACEMENT -1.0
#define LOST_GAME -5.0
#define GEAR_ACTIVATED 1.0
#define WHEEL_ACTIVATED 2.0
#define WON_GAME 10.0

class QTable
{
private:
	std::vector<std::vector<double>> values;
	std::vector<std::vector<int>> visits;
	double alpha;
	double gamma;
	int numStates;
	int numActions;
	std::string filename;

	std::mt19937 rng;
public:
	QTable();
	QTable(int numberOfStates, int numberOfActions, std::string filename);
	

	double getQValue(int stateId, int actionId)const;

	void updateQValue(int stateId, int actionId, double reward, int nextStateId, bool nextIsTerminal=false);

	int getAction(int stateId,double epsilon);

	void printTable(const std::string& filename,int iteration);

	void saveQTableCSV(const std::string& filename);

	void setAlpha(double alpha) { this->alpha = alpha; }
	void setGamma(double gamma) { this->gamma = gamma; }
	void seedRng(unsigned int seed) { this->rng.seed(seed); }
};

