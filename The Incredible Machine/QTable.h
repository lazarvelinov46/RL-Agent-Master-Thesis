#pragma once

#include "stdafx.h"

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

