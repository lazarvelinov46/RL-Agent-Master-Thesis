#pragma once

#include "stdafx.h"

#define WRONG_GEAR_PLACEMENT -1.0
#define WRONG_BELT_PLACEMENT -1.0
#define LOST_GAME -5.0
#define GEAR_ACTIVATED 1.0
#define WHEEL_ACTIVATED 1.0
#define WON_GAME 10.0

class QTable
{
private:
	std::unordered_map<int, std::unordered_map<int, double>> values;
	double alpha;
	double gamma;
public:
	QTable();
	QTable(int numberOfStates, int numberOfActions);
	

	double getQValue(int stateId, int actionId)const;

	void updateQValue(int stateId, int actionId, double reward, int nextStateId);

	int getAction(int stateId,double epsilon);
};

